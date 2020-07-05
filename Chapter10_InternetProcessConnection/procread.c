/*例10-5和10-6演示有名管道在无亲缘关系的进程间如何进行通信，
这个实例包括两个程序，程序名为procread.c和procwrite.c*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>


#define FIFO_NAME	"myfifo"
#define BUF_SIZE	1024

/*自定义的错误处理函数*/
void my_err(const char * err_string,int line)
{
	fprintf(stderr,"line: %d ",line);
	perror(err_string);
	exit(1);
}



int main()
{
	int fd;
	char buf[BUF_SIZE];
	
	umask(0);
	fd=open(FIFO_NAME,O_RDONLY);
	if(fd== -1){
		my_err("open",__LINE__);
	}
	read(fd,buf,BUF_SIZE);
	printf("Read content:%s\n",buf);
	close(fd);

	return 0;
}
/*
编译后首先运行procwrite(运行后处于阻塞状态)，打开另一个终端运行procread。运行结果如下：
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter10 InternetProcessConnection$ ./procwrite
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter10 InternetProcessConnection$ ./procread
Read content:Hello procread,I come from process named procwrite!
*/




