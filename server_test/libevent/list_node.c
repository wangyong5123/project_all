#include <stdio.h>                                                                                   
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include"list_node.h"

/* 创建一个链表 */
void CreateListTail(LinkList *L, int n)
{
	LinkList p;
	LinkList r;
	int i;
	srand(time(0));
	
	*L = (LinkList)malloc(sizeof(Node));
	r = *L;
	for(i=0;i<n;i++)
	{
		p=(Node*)malloc(sizeof(Node));
		p->bev = NULL;//rand()%100+1;
		r->next = p;
		r = p;
	}
	printf("create list ok\n");
	r->next = NULL;
}
/* 删除一个链表*/
int ClearList(LinkList *L)
{
	LinkList p;
	LinkList q;
	p=(*L)->next;
	while(p)
	{
		q=p->next;
		free(p);
		p=q;
	}
	(*L)->next = NULL;
	return 0;
}
	
/* 读取第i个元素 */
int GetElem(LinkList L, int i,struct bufferevent *e)
{
	int j;
	/* 声明一个节点 */
	LinkList p;
	/* 初始化 */
	p = L->next; //指向头结点
	j = 1;
	
	while(p && j<i)
	{
		p = p->next; //指向下一个节点
		++j;
	}
	if(!p || j>i)
		return -1;
	e = p->bev;
	return 0;
}
	
/*在链表L中的i位置，插入元素e */
int ListInsert(LinkList *L, int i, struct bufferevent *e)
{
	int j;
	LinkList p;
	LinkList s;
	p = *L;
	j = 1;
	while(p && j<i)
	{
		p = p->next; //指向下一个节点
		++j;
	}
	if(!p || j>i)
		return -1;
	s = (LinkList)malloc(sizeof(Node));
	
	s->bev = e;
	s->next = p->next;
	p->next = s;
	printf("add a node e=%x \n",e);
	return 0;
}

int ListInsertTail(LinkList *L, struct bufferevent *e)
{
        int j;
        LinkList p;
        LinkList s;
        p = *L;
        while(p)
        {
		printf("###########2=%x\n",p->bev);
                p = p->next; //指向下一个节点
        }
        s = (LinkList)malloc(sizeof(Node));
        s->bev = e;

	printf("############5\n");
        p->next = s;

	p=s;
	p->next=NULL;
        //printf("add a node e=%x \n",e);
        return 0;
}


/* 删除链表L的第i个数据元素，并返回其值e */
int ListDelete(LinkList *L, int i, struct bufferevent *e)
{
	int j;
	LinkList p;
	LinkList q;
	p = *L;
	j = 1;
	while(p && j<i)
	{
		p = p->next; //指向下一个节点
		++j;
	}
	if(!p || j>i)
		return -1;
	
	q = p->next;
	p->next = q->next;
	
	e = q->bev;

	printf("delete=%x\n",e);

	free(q);
	
	return 0;
	
}

/* 按照顺序打印链表L*/
int ListSeekNode(LinkList L,struct bufferevent *e)
{
	/* 声明一个节点 */
	printf("#############2\n");
	LinkList p;
	int index=0;
	p = L->next; //指向头结点
	printf("list seek\n");
	while(p)
	{

		index++;
		printf("%x e=%x\n", p->bev,e);
		if(p->bev == e)
		{
			printf("seek delete node pos=%d\n",index);	
			return index;
		}
		p = p->next; //指向下一个节点
	}
	return index;
	
}


int list_test()
{
	/*
	int tmp;
	LinkList test;
	CreateListTail(&test, 5);
	ListPrint(test);
	
	ListDelete(&test, 2, &tmp);
	ListPrint(test);
	
	ListInsert(&test, 4, 6);
	ListPrint(test);
	
	GetElem(test,3,&tmp);
	pri("tmp : %d \n", tmp);

	ClearList(&test);
	*/
	return 0;
}

