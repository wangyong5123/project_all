#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<sys/un.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<assert.h>
#include<string.h>
#include<ctype.h>
#include<errno.h>

#define SERVER_PORT 9876 

#define RPMSG_HEADER_LEN 16
#define MAX_RPMSG_BUFF_SIZE (6144 - RPMSG_HEADER_LEN)
#define PAYLOAD_MIN_SIZE	1
#define PAYLOAD_MAX_SIZE	(MAX_RPMSG_BUFF_SIZE - 24)


static void sys_error(const char *str)
{
	perror(str);
	exit(1);
}

int main(int argc,char *argv[])
{
	int cfd = 0;
	int ret;
	char buffer[4096*1024];

	if(argc < 3)
	{
		printf("./a.out ip port\n");
		return -1;
	}

	printf("ip=%s,port=%d\n",argv[1],atoi(argv[2]));

	struct sockaddr_in serv_addr;
	socklen_t clit_addr_len,client_ip_len;
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[2]));

	inet_pton(AF_INET,argv[1],&serv_addr.sin_addr);

	cfd = socket(AF_INET,SOCK_STREAM,0);
	if(cfd == -1)
		sys_error("socket error");

    int opt = 1;
    setsockopt(cfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); 

	ret = connect(cfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if(ret != 0)
		sys_error("connect error\n");

	char tmp_buf[6104*2];
	memset(tmp_buf,0,sizeof(tmp_buf));
	int total_ret=0,index = 0;
	int seq,drop_count,exec_count,error;
	drop_count = seq = exec_count = error = 0;
	while(1){
        	
		ret = read(cfd,buffer,PAYLOAD_MAX_SIZE);
		if(1)
		{
			total_ret += ret;
			strcat(tmp_buf,buffer);		
			if(total_ret == 6104)
			{
				
				if(tmp_buf[1] != 1 && tmp_buf[2] !=2)
				{
					error++;
                    			memset(tmp_buf,0,sizeof(tmp_buf));
                    			total_ret = 0;
                    			exec_count++;
					continue;
				}

				if(seq + 1 != tmp_buf[0]&0XFF)
				{
					if(tmp_buf[0]&0XFF != 0)
						drop_count += tmp_buf[0]&0XFF - (seq + 1);
				}

				seq = tmp_buf[0]&0XFF;
				printf("read=%d,buffer=%x %x %x,drop_count=%d,exec_count=%d,error=%d\n",total_ret,tmp_buf[0]&0XFF,tmp_buf[1]&0XFF,tmp_buf[2]&0XFF,drop_count,exec_count,error);
				memset(tmp_buf,0,sizeof(tmp_buf));
				total_ret = 0;
				exec_count++;

			}
			else if (total_ret > 6104)
				total_ret = 0;

		}
		//else if(ret == 6104)
		else if(1)
		{
				if(buffer[1] != 1 && buffer[2] !=2)
				{
					error++;
					exec_count++;
					continue;
				}
				if(buffer[0]&0XFF != 0)
				{
					drop_count += buffer[0]&0XFF - (seq + 1);
				}

			exec_count++;
			seq = buffer[0]&0XFF;
			printf("read=%d,buffer=%x %x %x,drop_count=%d,exec_count=%d,error=%d\n",ret,buffer[0]&0XFF,buffer[1]&0XFF,buffer[2]&0XFF,drop_count,exec_count,error);
		}


		usleep(5);
	}
        close(cfd);
        return 0;
}

