#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#include "my_recv.h"

#define SERV_PORT 4507	//服务器端口
#define LISTENQ 12	//连接请求的最大长度

#define USERNAME 0 // 接收到的是用户名
#define PASSWORD 1 // 接收到的是密码

struct userinfo{
	char username[32];
	char password[32];
};

struct userinfo users[]={
	{"linux","unix"},
	{"4507","4508"},
	{" "," "}// 以只含一个空格的字符串作为数组的结束标志
};



//查找用户名是否存在，存在返回该用户的下标，不存在返回-1，出错返回-2
int find_name(const char *name){
	int i;
	if(name == NULL){
		printf("in find_name,NLL pointer\r\n");
		return -2;
	}
	for(i = 0;users[i].username[0] != ' ';i++){
		if(strcmp(users[i].username,name) == 0){
			return i;
		}
	}
	return -1;
}

//发送数据
void send_data(int conn_fd,const char *string){
	if(send(conn_fd,string,strlen(string),0) < 0){
		my_err("send",__LINE__);
	}
}

int main()
{
	int sockfd,connfd;
	int optval;
	int get_optval;
	socklen_t get_optval_len = sizeof(int);
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	socklen_t cli_len;
	pid_t pid;
	ssize_t ret;
	char recv_buffer[128];
	int flag_recv = USERNAME;//标志接收到的是用户名还是密码
	int name_num;
	
	// 创建一个TCP套接字
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0){
		my_err("socket",__LINE__);
	}
	
	// 设置该套接字使之可以重新绑定端口
	optval = 1;
	if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(void*)&optval,sizeof(int)) < 0){
		my_err("setseckopt",__LINE__);
	}
	//获取套接字的类型
	//if(getsockopt(sockfd, SOL_SOCKET, SO_TYPE,(void *)&get_optval, &get_optval_len) < 0){
	//	my_err("getsockopt",__LINE__);				  
	//}
	//printf("getsockopt SO_TYPE %d\r\n",get_optval);//SOCK_STREAM == 1
	
	//初始化服务器地址结构
	memset(&serv_addr,0x00,sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	printf("serv_addr.sin_addr.s_addr %s\r\n",inet_ntoa(serv_addr.sin_addr));
	
	// 将套接字绑定到本端口
	if(bind(sockfd,(const struct sockaddr*)&serv_addr,sizeof(struct sockaddr_in)) < 0){
		my_err("bind",__LINE__);		
	}
	
	//将套接字转化为监听套接字
	if(listen(sockfd,LISTENQ) < 0){
		my_err("listen",__LINE__);
	}
	
	cli_len = sizeof(struct sockaddr_in);
	while(1){
		//通过accep接收客户端请求，并返回连接套接字用于收发数据
		connfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);
		if(connfd < 0){
			my_err("accept",__LINE__);
			exit(1);
		}
		printf("accept a new client,ip:%s\r\n",inet_ntoa(cli_addr.sin_addr));
		// 创建一个子进程处理刚刚接收的连接请求
		if((pid = fork()) == 0){// 子进程
			while(1){
				if((ret = recv(connfd, (void *)recv_buffer, sizeof(recv_buffer), 0)) < 0){
					my_err("recv",__LINE__);
				}
				recv_buffer[ret-1] = '\0';//将数据结束标志'\n'换成字符串结束标志
				if(flag_recv == USERNAME){//接收到的是用户名
					name_num = find_name(recv_buffer);
					switch(name_num){
					case -1:
						send_data(connfd,"n\n");
						break;
					case -2:
						exit(1);
						break;
					default:
						send_data(connfd,"y\n");
						flag_recv = PASSWORD;
						break;
					}
				}else if(flag_recv == PASSWORD){
					if(strcmp(users[name_num].password,recv_buffer) == 0){
						send_data(connfd,"y\n");
						send_data(connfd,"Welcom login my tcp server\n");
						printf("%s login\n",users[name_num].username);
						break;//跳出while循环
					}
					else{
						send_data(connfd,"n\n");
					}
				}
				close(sockfd);
				close(connfd);
				exit(0);
			}
		}else{//父进程关闭刚刚接收的连接请求，执行accept等待其他连接请求
			close(connfd);
		}
	}
	
	return 0;
}




















