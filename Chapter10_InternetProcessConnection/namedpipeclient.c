/**
通过创建两个管道可以实现进程间的全双工通信，同样也可以通过创建两个FIFO来实现不同进程间的全双工通信。
下面通过例10-7和例10-8演示一个程序中两个进程间的聊天程序来说明使用有名管道进行全双工通信。
*/
/**
这里以一个聊天程序为例，进一步展示消息队列的应用。聊天两端分别为进程
server和client。
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define FIFO_READ	"writefifo"
#define FIFO_WRITE	"readfifo"
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
	int wfd,rfd;
	char buf[BUF_SIZE];
	int len;
	
	if(access(FIFO_WRITE,F_OK) == -1){//判断FIFO_WRITE是否存在，存在直接打开，不存在则创建
		umask(0);
		if(mkfifo(FIFO_WRITE,S_IFIFO|0666)){
			printf("Can't create FIFO [%s] because [%s]\n",FIFO_WRITE,strerror(errno));
			exit(1);
		}
	}
	umask(0);
	while((rfd = open(FIFO_READ,O_RDONLY)) == -1){
		sleep(1);
	}
	
	wfd = open(FIFO_WRITE,O_WRONLY);
	if(wfd == -1){
		printf("Fail to open FIFO [%s] error: [%s]\n",FIFO_WRITE,strerror(errno));
		exit(1);
	}
	
	while(1){
		len = read(rfd,buf,BUF_SIZE);
		if(len > 0){
			buf[len] = '\0';
			printf("Server: %s\n",buf);
		}
		printf("Client: ");
		fgets(buf,BUF_SIZE,stdin);
		buf[strlen(buf)-1] = '\0';
		if(strncmp(buf,"quit",4) == 0){
			close(wfd);
			unlink(FIFO_WRITE);
			close(rfd);
			exit(0);
		}
		write(wfd,buf,strlen(buf));	
	}
	
	return 0;
}


/**
服务端：
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter10 InternetProcessConnection$ ./namedpipeserver
Server: Hello
Client: World
Server:
客户端： 
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter10 InternetProcessConnection$ ./namedpipeclient
Server: Hello
Client: World
*/

/**
access函数按照实际用户ID和实际组进行访问权限测试。函数原型为：
#include <unistd.h>
int access(const char *pathname, int mode);
mode取值：
F_OK 测试文件是否存在
R_OK 测试读权限
W_OK 测试写权限
X_OK 测试执行权限
正确判断一个文件是否存在是用access函数，实现如下所示：
int is_file_exist(const char *pathname)
{
	if(pathname == NULL){
		return -1;
	}
	if(access(pathname,F_OK) == 0){
		return 0;
	}
	return -1;
}
*/










