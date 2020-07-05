/*例10-1，创建管道，以及对管道读写是如何进行的*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
/*自定义的错误处理函数*/
void my_err(const char * err_string,int line)
{
	fprintf(stderr,"line: %d ",line);
	perror(err_string);
	exit(1);
}
/*子进程读写管道*/
void child_rw_pipe(int readfd,int writefd){
	char *message1="from child process!\n";
	write(writefd,message1,strlen(message1)+1);
	char message2[100];
	read(readfd,message2,100);
	printf("child process read from pipe: %s\r\n",message2);
}

/*父进程读写管道*/
void parent_rw_pipe(int readfd,int writefd){
	char *message1="from parent process!\n";
	write(writefd,message1,strlen(message1)+1);
	char message2[100];
	read(readfd,message2,100);
	printf("parent process read from pipe: %s\r\n",message2);
}



int main()
{
	/*fd0:管道读端，fd1:管道写端*/
	int pipefd1[2], pipefd2[2];
	pid_t pid;
	int stat_val;
	
	if(pipe(pipefd1)){//创建管道
		my_err("pipe1",__LINE__);
	}
	if(pipe(pipefd2)){//创建管道
		my_err("pipe2",__LINE__);
	}
	printf("realize full_duplex communication:\r\n");
	pid = fork();
	switch(pid){
	case -1:
		printf("fork error\r\n");
		exit(1);
	case 0:
		/*子进程关闭pipefd1[0]读端,关闭pipefd2[1]写端*/
		//close(pipefd1[0]);
		//close(pipefd2[1]);
		//child_rw_pipe(pipefd2[0],pipefd1[1]);
		/*子进程关闭pipefd1[1]写端,关闭pipefd2[1]读端*/
		close(pipefd1[1]);
		close(pipefd2[0]);
		child_rw_pipe(pipefd1[0],pipefd2[1]);
		exit(0);
	default:
		/*父进程关闭pipefd1[1]写端，关闭pipefd2[0]读端*/
		//close(pipefd1[1]);
		//close(pipefd2[0]);
		//parent_rw_pipe(pipefd1[0],pipefd2[1]);
		/*父进程关闭pipefd2[1]写端，关闭pipefd1[0]读端*/
		close(pipefd1[0]);
		close(pipefd2[1]);
		parent_rw_pipe(pipefd2[0],pipefd1[1]);
		wait(&stat_val);
		exit(0);
	}
	return 0;
}

/*
运行结果：
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter10 InternetProcessConnection$ ./dual_pipe
realize full_duplex communication:
parent process read from pipe: from child process!

child process read from pipe: from parent process!
*/


