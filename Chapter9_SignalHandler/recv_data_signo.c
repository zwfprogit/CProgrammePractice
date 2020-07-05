/*例9-10演示介绍如何利用信号实现数据传递(send_data_signo.c)*/
/*例9-11演示接收数据(recv_data_signo.c)*/
//选项-d 后跟待传递的数据，选项-s 后跟待发送的信号，选项-p 后跟目的进程ID

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>


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
/**
struct sigaction {
	void     (*sa_handler)(int);
	void     (*sa_sigaction)(int, siginfo_t *, void *);
	sigset_t   sa_mask;
	int        sa_flags;
	void     (*sa_restorer)(void);
};
*/

int main()
{
	struct sigaction act;
	
	/*赋值act结构*/
	act.sa_sigaction = handler_sigint;
	act.sa_flags = SA_SIGINFO;//指定使用3个参数的信号处理函数
	
	/*安装信号处理函数*/
	//int sigaction(int signum, const struct sigaction *act,struct sigaction *oldact);
	sigaction(SIGINT,&act,NULL);
	
	while(1);
	
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













