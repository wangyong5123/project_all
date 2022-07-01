//server.c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <pthread.h>
#include<signal.h>
#include"list_node.h"

#define MAX_CONNECT 1024

LinkList test;
typedef struct EVENT_BUF{
	int index;
	struct bufferevent *bev;

}EVENT_BUF;

int g_index = 0;
struct EVENT_BUF event_buf[2];

static void signal_cb(evutil_socket_t sig, short events, void *user_data);

void send_cb(evutil_socket_t fd, short what, void *arg); 

int i=0;
int ListNodeSend(LinkList L,char *buf,int len)
{
        LinkList p;
        int index=-1;
        p = L->next; //指向头结点
        while(p)
        {
	   if(p->bev == 0)
	   {
		   p=p->next;
		   continue;
	   }
	   
	   printf("send msg= %x \n",p->bev);
    	   bufferevent_write(p->bev, buf, len);
           p = p->next; //指向下一个节点
        }
}


// 读缓冲区回调
void read_cb(struct bufferevent *bev, void *arg)
{

    char buf[1024] = {0};   
    bufferevent_read(bev, buf, sizeof(buf));
    printf("client say: %s\n", buf);
 //   printf("total=%d\n",g_index);
    ListNodeSend(test,buf,strlen(buf)+1); 
}
 
// 写缓冲区回调
void write_cb(struct bufferevent *bev, void *arg)
{
    printf("我是写缓冲区的回调函数...您已发送\n"); 
}
 
// 事件
void event_cb(struct bufferevent *bev, short events, void *arg)
{
    if (events & BEV_EVENT_EOF)
    {
        printf("connection closed\n");  
    }
    else if(events & BEV_EVENT_ERROR)   
    {
        printf("some other error\n");
    }
    
    struct bufferevent *tmp_bev;
    
    int index_node = -1;
    index_node = ListSeekNode(test,bev);
    if(index_node !=-1)
    {
    	ListDelete(&test, index_node, tmp_bev);
	g_index = g_index - 1;
    }


    printf("buffevent release g_index=%d...\n",g_index);
    bufferevent_free(bev);    
}
 
void cb_listener(
        struct evconnlistener *listener, 
        evutil_socket_t fd, 
        struct sockaddr *addr, 
        int len, void *ptr)
{
   printf("connect new client\n");
 
   struct event_base* base = (struct event_base*)ptr;
   
   //3 新连接到来，开始通信
   //1) 先添加bufferevent事件
   struct bufferevent *bev;
   bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
 
   //2) 给bufferevent缓冲区设置三种回调
   bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);
   //3) 启动bufferevent的读监听(因为bufferevent读监听事件是禁用的)
   bufferevent_enable(bev, EV_READ);

   g_index++;
   ListInsert(&test,g_index,bev);


   printf("g_total=%d\n",g_index);
    //4 创建event普通事件(参2为int型句柄,需与FILE*类型的stdin区分)
    struct event* ev = event_new(base, STDIN_FILENO, 
                                 EV_READ | EV_PERSIST, 
                                 send_cb, bev);

	//5 添加事件到base
    event_add(ev, NULL);
}

void send_cb(evutil_socket_t fd, short what, void *arg)
{
    char buf[1024] = {0}; 
    struct bufferevent* bev = (struct bufferevent*)arg;
    printf("请输入要发送的数据: \n");
    read(fd, buf, sizeof(buf));
    strcpy(buf,"hello");
    	bufferevent_write(bev, buf, strlen(buf)+1);
}
 
int main(int argc, const char* argv[])
{
    // init server 
    struct sockaddr_in serv;
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(9876);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    evthread_use_pthreads();
//    struct event *signal_event;
    printf("create list\n");
    CreateListTail(&test, 1);
    g_index = 1;
 	//1 创建base
    struct event_base* base;
    base = event_base_new();

	//2 创建listener(相当于socket,bind,listen,accept)，并设置监听回调
    struct evconnlistener* listener;//BEV_OPT_THREADSAFE
    listener = evconnlistener_new_bind(base, cb_listener, base, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, 36, (struct sockaddr*)&serv, sizeof(serv));//参4代表端口复用并且关闭bufferevent时释放底层端口和套接字,36为listen的监听数

	//6 循环监听事件==>epoll_wait+while(1)
    event_base_dispatch(base);

   printf("##############wy\n");

 	//7 释放相关资源
    evconnlistener_free(listener);
    ClearList(&test);
    event_base_free(base);
 
    return 0;
}

static void signal_cb(evutil_socket_t sig, short events, void *user_data)
{
        struct event_base *base = user_data;
        struct timeval delay = { 2, 0 };

        printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");
        ClearList(&test);
        event_base_loopexit(base, &delay);
}

