/*例9-3演示setjmp和longjmp函数的用法*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <setjmp.h>

jmp_buf env;	//保存待跳转位置的栈信息

/*信号SIGRTMIN+15处理函数*/
void handler_sigrtmin15(int signo)
{
	printf("recv SIGRTMIN+15\r\n");
	longjmp(env,1);//返第二个回到env处，注意第二个参数的值
}

/*信号SIGRTMAX-10处理函数*/
void handler_sigrtmax10(int signo)
{
	printf("recv SIGRTMAX-10\r\n");
	longjmp(env,2);//返第二个回到env处，注意第二个参数的值
}



int main()
{
	/*设置返回点*/
	switch(setjmp(env)){
	case 0:
		break;
	case 1:
		printf("return from SIGRTMIN+15\r\n");
		break;
	case 2:
		printf("return from SIGRTMAX-10\r\n");
		break;
	default:
		break;
	}
	
	
	/*捕捉信号，安装信号处理函数*/
	signal(SIGRTMIN+15,handler_sigrtmin15);
	signal(SIGRTMAX-10,handler_sigrtmax10);
	while(1);
	printf("exit main process\r\n");
	return 0;
}

/**
在第一个终端执行：
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter9 SignalHandler$ ./wrong_return
另一个终端执行命令：
myroot@myroot:~$ kill -s SIGRTMIN+15 2129
myroot@myroot:~$ kill -s SIGRTMAX-10 2129
第一个终端结果如下：
recv SIGRTMIN+15
return from SIGRTMIN+15
recv SIGRTMAX-10
return from SIGRTMAX-10
程序执行结果完全符合预期，但是不是就没有问题呢？在第二个终端继续使用kill
发送信号试试看：
myroot@myroot:~$ kill -s SIGRTMIN+15 2129
myroot@myroot:~$ kill -s SIGRTMIN+15 2129
myroot@myroot:~$ kill -s SIGRTMAX-10 2129
myroot@myroot:~$ kill -s SIGRTMAX-10 2129
从运行结果可以发现，第一个终端的结果没有任何变化，然而信号有确实发送给
目标进程了，这是为什么呢？因为用setjmp和longjmp跳出信号处理程序不会自动将
信号屏蔽码修改会原来的屏蔽码，从而引起该信号永久屏蔽。
*/

















