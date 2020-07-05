/*例9-4演示sigsetjmp和siglongjmp函数的用法*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <setjmp.h>

#define ENV_UNSAVE	0
#define ENV_SAVED	1

int flag_saveenv = ENV_UNSAVE;
jmp_buf env;	//保存待跳转位置的栈信息

/*信号SIGRTMIN+15处理函数*/
void handler_sigrtmin15(int signo)
{
	if(flag_saveenv == ENV_UNSAVE){
		return;
	}
	printf("recv SIGRTMIN+15\r\n");
	sleep(5);
	printf("in handler_sigrtmin15,after sleep\r\n");
	siglongjmp(env,1);//返第二个回到env处，注意第二个参数的值
}

/*信号SIGRTMAX-10处理函数*/
void handler_sigrtmax10(int signo)
{
	if(flag_saveenv == ENV_UNSAVE){
		return;
	}
	printf("recv SIGRTMAX-10\r\n");
	//sleep(5);
	printf("in handler_sigrtmax10,after sleep\r\n");
	siglongjmp(env,2);//返第二个回到env处，注意第二个参数的值
}



int main()
{
	/*设置返回点*/
	switch(sigsetjmp(env,1)){// sigsetjmp第二个参数只要非0即可
	case 0:
		printf("sigsetjmp(env,1) return 0\r\n");
		flag_saveenv = ENV_SAVED;
		break;
	case 1:
		printf("return from SIGRTMIN+15\r\n");
		break;
	case 2:
		printf("return from SIGRTMAX-10\r\n");
		break;
	default:
		printf("return else\r\n");
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
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter9 SignalHandler$ ./right_return
sigsetjmp(env,1) return 0
另一个终端执行命令：
myroot@myroot:~$ kill -s SIGRTMIN+15 2023
myroot@myroot:~$ kill -s SIGRTMIN+15 2023
myroot@myroot:~$ kill -s SIGRTMIN+15 2023
myroot@myroot:~$ kill -s SIGRTMIN+15 2023
第一个终端结果如下：
recv SIGRTMIN+15
in handler_sigrtmin15,after sleep
recv SIGRTMIN+15
in handler_sigrtmin15,after sleep
recv SIGRTMIN+15
in handler_sigrtmin15,after sleep
recv SIGRTMIN+15
in handler_sigrtmin15,after sleep
return from SIGRTMIN+15
程序执行结果只打印出一条"return from SIGRTMIN+15"。分析过程如下：

1，连续调用kill命令发送了4个信号

2，第一个信号被响应，调用信号处理程序handler_sigrtmin15，并自动屏蔽信号
SIGRTMIN+15，因此其他三个信号进入未决信号队列。

3，等待5s后调用siglongjmp，该函数将SIGRTMIN+15信号的屏蔽字去除，然后准备返回。
如果不加sleep(5),基本上在你发送下一个信号的时候就能返回。

4，此时由于未决信号队列非空，先响应未决信号，再次调用handler_sigrtmin15。

5，依次类推，知道第四个信号被响应后，未决信号队列为空，此时成功跳转至sigsetjmp处
打印出提示信息。
*/

















