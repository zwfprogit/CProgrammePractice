/*例10-10演示如何向消息队列发送消息*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

void getmsgattr(int msgid,struct msqid_ds msg_info)
{
	//int msgctl(int msqid, int cmd, struct msqid_ds *buf);
	if(msgctl(msgid,IPC_STAT,&msg_info)){
		my_err("grtmsgattr msgctl",__LINE__);
		return;
	}
	printf("**** information of message queue: %d ***\n",msgid);
	printf("last msgsnd to msg time is: %s\n",ctime(&msg_info.msg_stime));
	printf("last msgrcv time from msg is: %s\n",ctime(&msg_info.msg_rtime));
	printf("last change msg time is: %s\n",ctime(&msg_info.msg_ctime));
	printf("current number of bytes on queue is: %lu\n",msg_info.msg_cbytes);
	printf("number of messages in queue is: %lu\n",msg_info.msg_qnum);
	printf("max number of bytes on queue is: %lu\n",msg_info.msg_qbytes);
	printf("pid of last msgsnd is: %d\n",msg_info.msg_lspid);
	printf("pid of last msgrcv is: %d\n",msg_info.msg_lrpid);

	printf("msg uid is: %d\n",msg_info.msg_perm.uid);
	printf("msg gid is: %d\n",msg_info.msg_perm.gid);
	printf("********information end!******** %d\n",msgid);
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
	struct msqid_ds msg_attr;
	
	
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
	getmsgattr(qid,msg_attr);/*输出消息队列的属性*/
	/*填充消息结构，发送到消息队列*/
	msgbuffer.msgtype = 2;
	strcpy(msgbuffer.ctrlstring,"A other message");
	msglen=sizeof(struct mymsgbuf)-4;//要发送消息的大小，不包含消息类型占用的4个字节
	if((msgsnd(qid,&msgbuffer,msglen,0)) == -1){
		my_err("msgsnd",__LINE__);
	}
	printf("msgsnd ===\r\n");
	getmsgattr(qid,msg_attr);/*再输出消息队列的属性*/
	
	/*设置消息队列的属性*/
	msg_attr.msg_perm.uid = 3;
	msg_attr.msg_perm.gid = 2;
	if(msgctl(qid,IPC_SET,&msg_attr) == -1){
		my_err("modify msgctl",__LINE__);
	}
	printf("msgctl  IPC_SET ===\r\n");
	getmsgattr(qid,msg_attr);/*修改后再输出消息队列的属性*/
	if(msgctl(qid,IPC_RMID,NULL) == -1){
		my_err("delete msgctl",__LINE__);
	}
	printf("msgctl  IPC_RMID ===\r\n");
	getmsgattr(qid,msg_attr);/*删除后再输出消息队列的属性*/
	
	exit(0);
}

/*
编译并运行这个程序后，就向消息队列放入了一条消息，可以通过命令ipcs查看。执行结果如下：
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter10 InternetProcessConnection$ ./opmsg
msgqueue msgkey=0x200173d2
msgqueue qid=0
**** information of message queue: 0 ***
last msgsnd to msg time is: Sat Aug  3 09:45:45 2019

last msgrcv time from msg is: Sat Aug  3 09:07:17 2019

last change msg time is: Sat Aug  3 08:36:55 2019

current number of bytes on queue is: 260
number of messages in queue is: 1
max number of bytes on queue is: 16384
pid of last msgsnd is: 2896
pid of last msgrcv is: 2583
msg uid is: 1000
msg gid is: 1000
********information end!******** 0
msgsnd ===
**** information of message queue: 0 ***
last msgsnd to msg time is: Sat Aug  3 10:00:15 2019

last msgrcv time from msg is: Sat Aug  3 09:07:17 2019

last change msg time is: Sat Aug  3 08:36:55 2019

current number of bytes on queue is: 520
number of messages in queue is: 2
max number of bytes on queue is: 16384
pid of last msgsnd is: 3104
pid of last msgrcv is: 2583
msg uid is: 1000
msg gid is: 1000
********information end!******** 0
line: 83 modify msgctl: Operation not permitted
注意：line: 83 modify msgctl: Operation not permitted

修改队列权限试试：
qid=msgget(msgkey,IPC_CREAT|0660)
改：
qid=msgget(msgkey,IPC_CREAT|0666)
然并卵：
直接用root权限运行

myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter10 InternetProcessConnection$ sudo ./opmsg
msgqueue msgkey=0x200173d2
msgqueue qid=32768
**** information of message queue: 32768 ***
last msgsnd to msg time is: Thu Jan  1 08:00:00 1970

last msgrcv time from msg is: Thu Jan  1 08:00:00 1970

last change msg time is: Sat Aug  3 10:14:23 2019

current number of bytes on queue is: 0
number of messages in queue is: 0
max number of bytes on queue is: 16384
pid of last msgsnd is: 0
pid of last msgrcv is: 0
msg uid is: 0
msg gid is: 0
********information end!******** 32768
msgsnd ===
**** information of message queue: 32768 ***
last msgsnd to msg time is: Sat Aug  3 10:14:23 2019

last msgrcv time from msg is: Thu Jan  1 08:00:00 1970

last change msg time is: Sat Aug  3 10:14:23 2019

current number of bytes on queue is: 260
number of messages in queue is: 1
max number of bytes on queue is: 16384
pid of last msgsnd is: 3283
pid of last msgrcv is: 0
msg uid is: 0
msg gid is: 0
********information end!******** 32768
msgctl  IPC_SET ===
**** information of message queue: 32768 ***
last msgsnd to msg time is: Sat Aug  3 10:14:23 2019

last msgrcv time from msg is: Thu Jan  1 08:00:00 1970

last change msg time is: Sat Aug  3 10:14:23 2019

current number of bytes on queue is: 260
number of messages in queue is: 1
max number of bytes on queue is: 6299704
pid of last msgsnd is: 3283
pid of last msgrcv is: 0
msg uid is: 3
msg gid is: 2
********information end!******** 32768
msgctl  IPC_RMID ===
line: 26 grtmsgattr msgctl: Invalid argument
*/













