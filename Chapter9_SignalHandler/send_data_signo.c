/*例9-10演示介绍如何利用信号实现数据传递(send_data_signo.c)*/
/*例9-11演示接收数据(recv_data_signo.c)*/
//选项-d 后跟待传递的数据，选项-s 后跟待发送的信号，选项-p 后跟目的进程ID

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


/*自定义的错误处理函数*/
void my_err(const char * err_string,int line)
{
	fprintf(stderr,"line: %d ",line);
	perror(err_string);
	exit(1);
}


/*信号处理函数*/
void handler_sigint(int signo,siginfo_t *siginfo,void *pvoid)
{
	printf("recv SIGINT,the data value is: %d\r\n",siginfo->si_int);
}


int main(int argc,char **argv)
{
	union sigval value;
	int signum = SIGTERM;//默认发送SIGTERM
	pid_t pid;
	int i;
	
	/*初始化*/
	value.sival_int = 0;
	
	/*检查参数个数*/
	if(argc != 3 && argc != 5 && argc != 7){
		printf("./send_data_signo <-d data> <-s signum> [-p][PID]");
		exit(1);
	}
	
	/*从命令行参数解析出信号编号，PID以及待传递的数据*/
	for(i=1;i<argc;i++){
		if(!strcmp(argv[i],"-d")){
			value.sival_int = atoi(argv[i+1]);
			continue;
		}
		if(!strcmp(argv[i],"-s")){
			signum = atoi(argv[i+1]);
			continue;
		}
		if(!strcmp(argv[i],"-p")){
			pid = atoi(argv[i+1]);
			continue;
		}
	}
	
	/*利用sigqueue给pid发送信号signum，并携带数据value*/
	if(sigqueue(pid,signum,value) == -1){
		perror("sigqueue");
	}
	
	return 0;
}

/**
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter9 SignalHandler$ ./recv_data_signo &                  
[1] 20104
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter9 SignalHandler$ ./send_data_signo -s 2 -d 10 -p 20104
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter9 SignalHandler$ recv SIGINT,the data value is: 10

myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter9 SignalHandler$ ./send_data_signo -s 2 -p 20104      
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter9 SignalHandler$ recv SIGINT,the data value is: 0

myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter9 SignalHandler$ ./send_data_signo -p 20104     
[1]+  Terminated              ./recv_data_signo
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter9 SignalHandler$ 
*/













