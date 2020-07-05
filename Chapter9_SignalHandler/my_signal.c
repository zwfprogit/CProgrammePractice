/*例9-1演示signal函数的用法*/

/**
#include <signal.h>

typedef void (*sighandler_t)(int);

sighandler_t signal(int signum, sighandler_t handler);
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

/*信号处理函数*/
void handler_sigint(int signo)
{
	if(signo == SIGINT)
		printf("recv SIGINT\r\n");
}




int main()
{
	/*安装信号处理函数*/
	signal(SIGINT,handler_sigint);

	while(1);
	printf("exit main process\r\n");
	return 0;
}

/**
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter9 SignalHandler$ ./my_signal
(ALT+C)
recv SIGINT
(ALT+C)
recv SIGINT
(ALT+\)//发送SIGQUIT信号结束进程
Quit (core dumped)

也可以用kill +进程ID杀死进程

*/