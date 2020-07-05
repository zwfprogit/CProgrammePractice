/*例9-9演示sigsuspend函数的用法*/

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
void handler_sigint(int signo)
{
	printf("recv SIGINT\r\n");
}


int main()
{
	sigset_t newmask,oldmask,zeromask;//定义信号集
	/*安装信号处理函数*/
	if(signal(SIGINT,handler_sigint) == SIG_ERR){
		my_err("signal",__LINE__);
	}
	
	/*初始化信号集newmask并将SIGINT添加进去*/
	sigemptyset(&newmask);
	sigemptyset(&zeromask);
	sigaddset(&newmask,SIGINT);
	
	/*屏蔽信号SIGINT*/
	if(sigprocmask(SIG_BLOCK,&newmask,&oldmask) == -1){
		my_err("sigprocmask",__LINE__);
	}else{
		printf("SIGINT blocked\r\n");
	}

	/*临界区*/
	/*使用sigsuspend取消所有信号的屏蔽并等待信号的触发*/
	if(sigsuspend(&zeromask) != -1){//sigsuspend总是返回-1
		my_err("sigsuspend",__LINE__);
	}else{
		printf("recv a signo,return from sigsuspend\r\n");
	}
	
	/*
	//若果使用sigprocmask加上pause可能会出现错误
	if(sigprocmask(SIG_SETMASK,&oldmask,NULL) == -1){
		my_err("sigprocmask",__LINE__);
	}
	pause();
	*/
	
	/*解除对SIGINT的屏蔽*/
	if(sigprocmask(SIG_SETMASK,&oldmask,NULL) == -1){
		my_err("sigprocmask",__LINE__);
	}else{
		printf("SIGINT unblocked\r\n");
	}
	
	while(1);
	printf("exit main process\r\n");
	return 0;
}

/**
终端1：
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter9 SignalHandler$ ./sig_suspend
SIGINT blocked

终端2：
myroot@myroot:~$ ps -a
  PID TTY          TIME CMD
 3220 pts/9    00:00:00 sig_suspend
 3221 pts/10   00:00:00 ps
myroot@myroot:~$ kill -s SIGRTMIN 3220

终端1：
Real-time signal 0
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter9 SignalHandler$ 
由于所有实时信号的默认响应都是终止进程，因此程序在收到信号SIGRTMIN之后，解除
sigsuspend()的挂起状态，并将信号屏蔽字修改为第一次调用sigprocmask()前的状态
*/













