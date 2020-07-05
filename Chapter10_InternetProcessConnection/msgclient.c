/*例10-13,10-14演示消息队列的应用实例*/
/*
这里以一个聊天程序为例，进一步展示消息队列的应用。聊天两端分别为进程
server和client。
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE	256
#define PROJ_ID		32
#define PATH_NAME	"/tmp"
#define SERVER_MSG	1
#define CLIENT_MSG	2

/*自定义的错误处理函数*/
void my_err(const char * err_string,int line)
{
	fprintf(stderr,"line: %d ",line);
	perror(err_string);
	exit(1);
}

int main()
{
	/*用户自定义消息缓冲*/
	struct mymsgbuf{
		long msgtype;
		char ctrlstring[BUF_SIZE];
	}msgbuffer;
	int qid;/*消息队列标识符*/
	int msglen;
	key_t msgkey;
	
	
	/*获取消息队列唯一键值*/
	if((msgkey = ftok(PATH_NAME,PROJ_ID)) == -1){
		my_err("ftok",__LINE__);
	}
	printf("msgqueue msgkey=0x%x\r\n",msgkey);
	
	/*创建消息队列*/
	if((qid=msgget(msgkey,IPC_CREAT|0660)) == -1){
		my_err("msgget",__LINE__);
	}
	printf("msgqueue qid=%d\r\n",qid);
	while(1){
		if((msgrcv(qid,&msgbuffer,BUF_SIZE,SERVER_MSG,0)) == -1){
			my_err("msgrcv",__LINE__);
		}
		printf("Server: %s\r\n",msgbuffer.ctrlstring);
		
		printf("Client: ");
		/*填充消息结构，发送到消息队列*/
		fgets(msgbuffer.ctrlstring,BUF_SIZE,stdin);
		if(strncmp("exit",msgbuffer.ctrlstring,4) == 0){
			break;
		}
		msgbuffer.ctrlstring[strlen(msgbuffer.ctrlstring)-1] = '\0';
		msgbuffer.msgtype = CLIENT_MSG;
		if((msgsnd(qid,&msgbuffer,strlen(msgbuffer.ctrlstring)+1,0)) == -1){
			my_err("msgsnd",__LINE__);
		}
	}
	
	exit(0);
}

/*
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter10 InternetProcessConnection$ ./server
msgqueue msgkey=0x20010002
msgqueue qid=98304
Server: Hello,Client
Client: Hello,Server
Server: exit

myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter10 InternetProcessConnection$ ./client
msgqueue msgkey=0x20010002
msgqueue qid=98304
Server: Hello,Client
Client: Hello,Server
line: 52 msgrcv: Identifier removed
*/
