/*例9-8演示sigprocmask和sigpending函数的用法*/

/**
typedef struct {
    unsigned long sig[_NSIG_WORDS];
} sigset_t;
*/

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
	sigset_t newmask,oldmask,pendmask;//定义信号集
	/*安装信号处理函数*/
	if(signal(SIGINT,handler_sigint) == SIG_ERR){
		my_err("signal",__LINE__);
	}
	
	//睡眠10s
	sleep(10);
	
	/*初始化信号集newmask并将SIGINT添加进去*/
	sigemptyset(&newmask);
	sigaddset(&newmask,SIGINT);
	
	/*屏蔽信号SIGINT*/
	if(sigprocmask(SIG_BLOCK,&newmask,&oldmask) == -1){
		my_err("sigprocmask",__LINE__);
	}else{
		printf("SIGINT blocked\r\n");
	}

	sleep(10);
	
	/*获取未决信号队列*/
	if(sigpending(&pendmask) == -1){
		my_err("sigpending",__LINE__);
	}
	
	/*检查未决信号队列里面是否有SIGINT*/
	switch(sigismember(&pendmask,SIGINT)){
	case 0:
		printf("SIGINT is not in pending queue\r\n");
		break;
	case 1:
		printf("SIGINT is in pending queue\r\n");
		break;
	case -1:
		my_err("sigismember",__LINE__);
		break;
	default:
		break;
	}
	
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
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter9 SignalHandler$ ./sig_mask
^Crecv SIGINT	//此时还没有阻塞SIGINT信号
SIGINT blocked	//阻塞SIGINT信号
^C^C^C^C^C^C^CSIGINT is in pending queue	//SIGINT信号被添加到挂起队列中
recv SIGINT		//先于SIGINT unblocked打印出来，这是因为第二次调用sigprocmask解除对
SIGINT信号的阻塞以后，进程未决信号队列非空，首先执行信号处理函数，然后在执行printf("SIGINT unblocked\r\n");
SIGINT unblocked
c^Crecv SIGINT
^Crecv SIGINT
^Crecv SIGINT
^Crecv SIGINT
^Crecv SIGINT
^Crecv SIGINT
^Crecv SIGINT
^Crecv SIGINT
^Crecv SIGINT

*/