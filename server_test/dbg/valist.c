#include<stdio.h>
#include<time.h>
#define MODULE_NAME "test"
#define CCT (+8)
#define WARING " waring"


#ifdef __DEBUG
#define DBG(fmt, args...) fprintf(stderr, "[" MODULE_NAME "]" "Debug:" fmt, ## args)
#else
#define DBG(fmt, args...)
#endif


#define SYS_LOG(fmt, args...) \
do{ \
\
    time_t timep;\
    struct tm *p;\
    time(&timep);\
    p=gmtime(&timep);\
    fprintf(stderr, "%d-%d-%d %d:%d:%d ",p->tm_year+1900, (1+p->tm_mon), p->tm_mday, (p->tm_hour+CCT)%24, p->tm_min, p->tm_sec);\
    fprintf(stderr, "module [" MODULE_NAME"] level [waring] " fmt, ## args); \
\
}while(0)


void main()
{
	SYS_LOG("fun[%s]\n",__FUNCTION__);

	DBG("good\n");

	printf("%s\n",__FUNCTION__);
}


