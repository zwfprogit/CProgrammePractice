/*父进程和子进程交替执行*/
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
	char *msg;
	int k;
	
	printf("Process Creation Study\n");
	
	pid = fork();
	switch(pid){
	case 0:
		msg = "Child process is running";
		k=3;
		break;
	case -1:
		perror("Process creation failed\n");
		break;
	default:
		msg = "Parent process is running";
		k=5;
		break;
	}
	
	while(k>0){
		printf("k===%d\n",k);
		puts(msg);
		sleep(1);
		k--;
	}

	exit(0);
}