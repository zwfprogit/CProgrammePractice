
/*例10-1，创建管道，以及对管道读写是如何进行的*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
/*自定义的错误处理函数*/
void my_err(const char * err_string,int line)
{
	fprintf(stderr,"line: %d ",line);
	perror(err_string);
	exit(1);
}
/*读管道*/
void read_from_pipe(int fd){
	char message[100];
	read(fd,message,100);
	printf("read from pipe: %s\r\n",message);
}

/*写管道*/
void write_from_pipe(int fd){
	const char *message = "Hello,pipe\n";
	write(fd,message,strlen(message)+1);
}


int main()
{
	int pipefd[2];
	pid_t pid;
	
	if(pipe(pipefd)){//创建管道
		my_err("pipe",__LINE__);
	}

	pid = fork();
	switch(pid){
	case -1:
		printf("fork error\r\n");
		exit(1);
	case 0:
		/*子进程关闭pipefd[1],读管道*/
		close(pipefd[1]);
		read_from_pipe(pipefd[0]);
		exit(0);
	default:
		/*父进程关闭pipefd[0],写管道*/
		close(pipefd[0]);
		write_from_pipe(pipefd[1]);
		exit(0);
	}
	return 0;
}
/*
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter10 InternetProcessConnection$ ./pipe
read from pipe: Hello,pipe

注意：必须在系统调用fork()之前调用pipe(),否则子进程将不会继承管道的文件描述符
*/




