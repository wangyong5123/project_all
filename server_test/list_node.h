#ifndef LIST_NODE
#define LIST_NODE


typedef unsigned char BYTE;
typedef unsigned int DWORD;
typedef unsigned short WORD;

#define pri(fmt, ...)   printf("["__FILE__"] <%s>_<%d> " ,__FUNCTION__,__LINE__ );\
                                                printf(fmt, ##__VA_ARGS__);


typedef struct Node
{
        int index;
        struct bufferevent *bev;
        struct Node *next;
}Node;

typedef struct Node *LinkList;

void CreateListTail(LinkList *L, int n);
int ClearList(LinkList *L);
int GetElem(LinkList L, int i,struct bufferevent *e);
int ListInsert(LinkList *L, int i, struct bufferevent *e);
int ListDelete(LinkList *L, int i, struct bufferevent *e);
//void ListPrint(LinkList L);
int ListSeekNode(LinkList L,struct bufferevent *e);
int ListInsertTail(LinkList *L, struct bufferevent *e);
int list_test();

#endif

