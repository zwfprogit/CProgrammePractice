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

int globVar = 5;

int main(void)
{
	pid_t pid;
	int var = 1,i;
	
	printf("fork is diffirent with vfork\n");
	
	//pid = fork();
	pid = vfork();
	
	switch(pid){
	case 0:
		i = 3;
		while(i-- > 0){
			printf("Child process is running\n");
			globVar++;
			var++;
			sleep(1);
		}
		printf("Child's globVar: %d var: %d\n",globVar,var);
		exit(0);//vfork这句很重要，让子进程退出才能执行父进程
		break;
	case -1:
		perror("Process creation failed\n");
		break;
	default:
		i = 5;
		while(i-- > 0){
			printf("Parent process is running\n");
			globVar++;
			var++;
			printf("local var: %d\n",var);
			sleep(1);
		}
		printf("Parent's globVar: %d var: %d\n",globVar,var);
		exit(0);
		break;
	}

	return 0;
}
/*
当使用fork创建子进程的时候，运行结果如下：
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter7 ProcessControl$ ./diffork
fork is diffirent with vfork
Parent process is running
Child process is running
Parent process is running
Child process is running
Parent process is running
Child process is running
Parent process is running
Child's globVar: 8 var: 4
Parent process is running
Parent's globVar: 10 var: 6

使用vfork创建子进程，程序运行结果如下：
fork is diffirent with vfork
Child process is running
Child process is running
Child process is running
Child's globVar: 8 var: 4
Parent process is running
local var: 32768
Parent process is running
local var: 32769
Parent process is running
local var: 32770
Parent process is running
local var: 32771
Parent process is running
local var: 32772
Parent's globVar: 13 var: 32772
可见，局部标量var的值在子进程结束到父进程并不连续。
修改如下，显示的让子进程退出，添加exit(0);结束子进程，打印如下：
fork is diffirent with vfork
Child process is running
Child process is running
Child process is running
Child's globVar: 8 var: 4
Parent process is running
local var: 5
Parent process is running
local var: 6
Parent process is running
local var: 7
Parent process is running
local var: 8
Parent process is running
local var: 9
Parent's globVar: 13 var: 9
*/














