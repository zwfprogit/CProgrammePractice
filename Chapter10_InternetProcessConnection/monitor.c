
/**例10-3，10-4：在父进程创建子进程后向子进程传递参数*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
/*自定义的错误处理函数*/
void my_err(const char * err_string,int line)
{
	fprintf(stderr,"line: %d ",line);
	perror(err_string);
	exit(1);
}

int main(int argc,char *argv[],char **environ)
{
	int fd[2];
	pid_t pid;
	int stat_val;
	
	if(argc < 2){
		printf("wrong parameters\n");
		exit(0);
	}
	
	if(pipe(fd)){//创建管道
		my_err("pipe",__LINE__);
	}

	pid = fork();
	switch(pid){
	case -1:
		perror("fork fail!\r\n");
		exit(1);
	case 0:
		close(0);	//关闭标准输入STDIN_FILENO=0
		dup(fd[0]);	//复制管道输入端到标准输入
		execve("ctrlprocess",(void*)argv,environ);
		exit(0);
	default:
		/*父进程关闭fd[0],读管道*/
		close(fd[0]);
		/*将命令行第一个参数写进管道*/
		write(fd[1],argv[1],strlen(argv[1]));
		break;
	}
	wait(&stat_val);

	return 0;
}
/**
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter10 InternetProcessConnection$ ./monitor getpid
ctrlprocess receive: getpid
My pid: 1968
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter10 InternetProcessConnection$ ./monitor exit
ctrlprocess receive: exit
*/




