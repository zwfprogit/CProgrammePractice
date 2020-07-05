/*
孤儿进程：如果一个子进程的父进程先于子进程结束，子进程就成了一个孤儿进程，它由init
进程收养，成为init进程的子进程，例7-3演示这种情况的用法
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/*自定义的错误处理函数*/
void my_err(const char * err_string,int line)
{
	fprintf(stderr,"line: %d ",line);
	perror(err_string);
	exit(1);
}

int main(void)
{
	pid_t pid;
	
	printf("Process Creation Study\n");
	
	pid = fork();
	
	switch(pid){
	case 0:
		while(1){
			printf("A background process,PID: %d ParentID: %d\n",getpid(),getppid());
			sleep(3);
		}
		break;
	case -1:
		perror("Process creation failed\n");
		break;
	default:
		printf("I am parent process,my pid is %d\n",getpid());
		exit(0);
		break;
	}

	return 0;
}

/*
解析：
Process Creation Study
A background process,PID: 2947 ParentID: 2946
I am parent process,my pid is 2946
此后子进程就成了孤儿进程，由init进程收养，可以看到此时子进程的父进程ID变为1
A background process,PID: 2947 ParentID: 1
A background process,PID: 2947 ParentID: 1
A background process,PID: 2947 ParentID: 1
A background process,PID: 2947 ParentID: 1
*/
















