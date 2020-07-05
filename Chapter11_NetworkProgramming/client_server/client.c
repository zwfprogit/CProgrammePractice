#include<stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#include "my_recv.h"


#define INVALID_USERINFO 	'n' // 用户信息无效
#define VALID_USERINFO 		'y' // 用户信息有效

// 获取用户输入存入到buf,buf的长度为len，用户数据以'\n'为结束标志
int get_userinfo(char *buf,int len){
	int i;
	int c;
	
	if(buf == NULL){
		return -1;
	}
	i = 0;
	while(((c = getchar()) != '\n') && (c != EOF) && (i < len - 2)){
		buf[i++] = c;
	}
	buf[i++] = '\n';
	buf[i++] = '\0';

	return 0;
}

// 输入用户名，然后通过fd发送出去
void input_userinfo(int conn_fd,const char *string){
	char input_buf[32];
	char recv_buf[BUFFERSIZE];
	int flag_userinfo = INVALID_USERINFO;

	//输入用户信息知道正确为止
	do{
		printf("%s:",string);
		if(get_userinfo(input_buf,sizeof(input_buf)) < 0){
			printf("error return from get_userinfo\n");
			exit(1);
		}
		if(send(conn_fd,input_buf,strlen(input_buf),0) < 0){
			my_err("send",__LINE__);
		}
		
		// 从套接字上读取一次数据
		if(my_recv(conn_fd,recv_buf,sizeof(recv_buf)) < 0){
			printf("my_recv data: %s\r\n",recv_buf);
			printf("data is too long\n");
			exit(1);
		}
		
		if(recv_buf[0] == VALID_USERINFO){
			flag_userinfo = VALID_USERINFO;
		}else{
			printf("%s error,input again\n",string);
			flag_userinfo = INVALID_USERINFO;
		}	
	}while(flag_userinfo == INVALID_USERINFO);
}


int main(int argc,char **argv)
{
	int sockfd;
	struct sockaddr_in serv_addr;
	int i;
	int ret;
	int serv_port;
	char recv_buf[BUFFERSIZE];
	
	// 检查参数个数
	if(argc != 5){
		printf("Usage:[-p] [serv_port] [-a] [serv_address]\n");
		exit(1);
	}
	
	// 初始化服务器端地址结构
	memset(&serv_addr,0x00,sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	// 从命令行获取服务器端的端口与地址
	for(i = 1;i < argc;i++){
		if(strcmp("-p",argv[i]) == 0){
			serv_port = atoi(argv[i+1]);
			if(serv_port < 0 || serv_port > 65535){
				printf("invalid serv_addr.sin_port\n");
				exit(1);
			}else{
				serv_addr.sin_port = htons(serv_port);
			}
			continue;
		}
		if(strcmp("-a",argv[i]) == 0){
			if(inet_aton(argv[i+1],&serv_addr.sin_addr) == 0){
				printf("invalid server ip address\n");
				exit(1);
			}
			continue;
		}	
	}
	// 检测是否少输入了某项参数
	if(serv_addr.sin_port == 0 || serv_addr.sin_addr.s_addr == 0){
		printf("Usage:[-p] [serv_addr.sin_port] [-a] [serv_address]\n");
		exit(1);
	}
	
	// 创建一个TCP套接字
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0){
		my_err("socket",__LINE__);
	}
	
	// 向服务器发送连接请求
	if(connect(sockfd, (const struct sockaddr *)&serv_addr,sizeof(struct sockaddr_in)) < 0){
		my_err("connect",__LINE__);
	}
	
	// 输入用户名和密码
	input_userinfo(sockfd,"username");
	input_userinfo(sockfd,"password");
	
	// 读取欢迎信息并打印出来
	if((ret = my_recv(sockfd,recv_buf,sizeof(recv_buf))) < 0){
		printf("data is too long\n");
		exit(1);
	}
	for(i = 0;i < ret;i++){
		printf("%c",recv_buf[i]);
	}
	printf("\n");
	
	close(sockfd);

	return 0;
}










