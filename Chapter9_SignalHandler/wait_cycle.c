/*信号应用于事件通知*/
/*实际应用中，进程常常需要等待某一事件的发生，一般可以通过检测某一全局变量来
判断事件是否发生。有3种方法可以实现这一要求。
第一种方法：程序不停的循环检测全局变量，这样可以满足要求，但是非常占用CPU资源，如例9-12所示。
第二种方法：进程使用pause()挂起，等待信号的触发，事件发生时向进程发送信号，对应信号处理函数改变全局变量的值，
信号处理函数返回后进程检测该全局变量，满足要求即可知道事件已发生。如例9-13所示。
第三种方法：原理与第二种方法一直，不过使用的是函数sigsuspend。如例9-14
*/

/*例9-12演示程序不停地循环检测全局变量，判断某个时间发生*/


/**
#include <signal.h>

typedef void (*sighandler_t)(int);

sighandler_t signal(int signum, sighandler_t handler);
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#define UNHAPPEN	0
#define HAPPENED	1

/*定义全局变量以标识事件是否发生*/
int flag_happen;

/*信号处理函数*/
void handler_sigint(int signo)
{
	flag_happen = HAPPENED;
	printf("recv SIGINT\r\n");
}




int main()
{
	/*安装信号处理函数*/
	if(signal(SIGINT,handler_sigint) == SIG_ERR){
		perror("signal");
		exit(1);
	}

	while(1){
		if(flag_happen == HAPPENED){
			printf("event happend\n");
			/*... you can do something else here ...*/
			break;
		}
	}
	printf("exit main process\r\n");
	return 0;
}

/**
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter9 SignalHandler$ ./wait_cycle
^Crecv SIGINT
event happend
exit main process

*/