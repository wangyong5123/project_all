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
#include<signal.h>

#define MAX_CONNECT 1024

static void signal_cb(evutil_socket_t sig, short events, void *user_data);

// 读缓冲区回调
void read_cb(struct bufferevent *bev, void *arg)
{


    //char buf[1024] = {0};   
    //bufferevent_read(bev, buf, sizeof(buf));

    //bufferevent_write(bev, buf, strlen(buf)+1);

  struct evbuffer *input = bufferevent_get_input(bev);
  struct evbuffer *output = bufferevent_get_output(bev);

  evbuffer_add_buffer(output, input);


}
 
// 写缓冲区回调
void write_cb(struct bufferevent *bev, void *arg)
{
    //printf("我是写缓冲区的回调函数...您已发送\n"); 
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
    bufferevent_write(bev, buf, strlen(buf)+1);
}
 
int main(int argc, const char* argv[])
{
    struct sockaddr_in serv;
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(9876);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
 

    struct event_base* base;
    base = event_base_new();

    struct evconnlistener* listener;
    listener = evconnlistener_new_bind(base, cb_listener, base, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, 36, (struct sockaddr*)&serv, sizeof(serv));//参4代表端口复用并且关闭bufferevent时释放底层端口和套接字,36为listen的监听数

    event_base_dispatch(base);

    evconnlistener_free(listener);
    event_base_free(base);
 
    return 0;
}

static void signal_cb(evutil_socket_t sig, short events, void *user_data)
{
        struct event_base *base = user_data;
        struct timeval delay = { 2, 0 };

        printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");
        event_base_loopexit(base, &delay);
}

