/*fork创建进程*/
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
	
	if(pid == 0){
		printf("Child Process is running,CurPid is %d,ParentPid is %d\n",getpid(),getppid());
	}else if(pid > 0){
		printf("Parent Process is running,ChildPid is %d,ParentPid is %d\n",pid,getpid());
	}else if(pid < 0){
		my_err("Process Creation Failed",__LINE__);
	}
	getchar();

	return 0;
}