
/**例10-3，10-4：在父进程创建子进程后向子进程传递参数*/
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

int main(int argc,char *argv[])
{
	int n;
	char buf[1024];
	while(1){
		/**从标准输入STDIN_FILENO=0读取父进程传入的数据*/
		if((n = read(STDIN_FILENO,buf,1024))>0){
			buf[n]='\0';
			printf("ctrlprocess receive: %s\n",buf);
			if(!strcmp(buf,"exit")){
				exit(0);
			}
			if(!strcmp(buf,"getpid")){
				printf("My pid: %d\n",getpid());
				exit(0);
			}
		}
	}
	return 0;
}
/**
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter10 InternetProcessConnection$ ./monitor getpid
ctrlprocess receive: getpid
My pid: 1968
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter10 InternetProcessConnection$ ./monitor exit
ctrlprocess receive: exit
*/




