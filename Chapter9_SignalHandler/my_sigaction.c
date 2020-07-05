/*例9-2演示sigaction函数的用法*/
/*Linux 下signal函数室友sigaction函数实现的*/

/**
#include <signal.h>

int sigaction(int signum, const struct sigaction *act,struct sigaction *oldact);

struct sigaction {
	void     (*sa_handler)(int);
	void     (*sa_sigaction)(int, siginfo_t *, void *);
	sigset_t   sa_mask;
	int        sa_flags;
	void     (*sa_restorer)(void);
};
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int temp = 0;

/*信号处理函数*/
void handler_sigint(int signo)
{
	printf("recv SIGINT\r\n");
	sleep(5);
	temp+=1;
	printf("the value of temp is: %d\r\n",temp);
	printf("in handler_sigint，aftre sleep\r\n");
}




int main()
{
	struct sigaction act;
	
	/*赋值act结构*/
	act.sa_handler = handler_sigint;
	act.sa_flags = SA_NOMASK;
	/*安装信号处理函数*/
	sigaction(SIGINT,&act,NULL);

	while(1);
	printf("exit main process\r\n");
	return 0;
}

/**
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter9 SignalHandler$ ./my_sigaction
(CTRL+C)快速按两次
^Crecv SIGINT
^Crecv SIGINT
the value of temp is: 1
in handler_sigint，aftre sleep
the value of temp is: 2
in handler_sigint，aftre sleep
(按下Ctrl+\)
^\Quit (core dumped)
*/