/*例10-10演示如何向消息队列发送消息*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE	256
#define PROJ_ID		32
#define PATH_NAME	"."

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
	/*填充消息结构，发送到消息队列*/
	msgbuffer.msgtype = 3;
	strcpy(msgbuffer.ctrlstring,"Hello,message queue");
	msglen=sizeof(struct mymsgbuf)-4;//要发送消息的大小，不包含消息类型占用的4个字节
	if((msgsnd(qid,&msgbuffer,msglen,0)) == -1){
		my_err("msgsnd",__LINE__);
	}
	
	exit(0);
}

/*
编译并运行这个程序后，就向消息队列放入了一条消息，可以通过命令ipcs查看。执行结果如下：
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter10 InternetProcessConnection$ ./sendmsg
msgqueue msgkey=0x200173d2
msgqueue qid=0

myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter10 InternetProcessConnection$ ipcs 

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x200173d2 0          myroot     660        260          1       
从输出结果可以看出，系统内部生成一个消息队列， 其中含有一条消息。  

注意：msglen=sizeof(struct mymsgbuf)-4;//要发送消息的大小，不包含消息类型占用的4个字节
否则:出错
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter10 InternetProcessConnection$ ./rcvmsg
msgqueue msgkey=0x200173d2
msgqueue qid=0
line: 49 msgrcv: Argument list too long
*/













