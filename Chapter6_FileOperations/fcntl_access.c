/**/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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
	int ret;
	int access_mode;
	
	if((fd = open("example_64",O_CREAT|O_RDWR|O_TRUNC,S_IRWXU)) == -1){
		my_err("open",__LINE__);
	}
	
	/*设置文件打开方式*/
	if((ret = fcntl(fd,F_SETFL,O_APPEND)) < 0){
		my_err("open",__LINE__);
	}
	printf("ret %08x O_ACCMODE= %08x\n",ret,O_ACCMODE);
	printf("O_RDONLY %08x O_WRONLY %08x O_RDWR= %08x\n",O_RDONLY,O_WRONLY,O_RDWR);
	access_mode = ret & O_ACCMODE;
	if(access_mode == O_RDONLY){
		printf("example_64 access mode:read only\n");
	}else if(access_mode == O_WRONLY){
		printf("example_64 access mode:write only\n");
	}else if(access_mode == O_RDWR){
		printf("example_64 access mode:read+write\n");
	}
	
	printf("O_APPEND %08x\n",O_APPEND);
	if(ret & O_APPEND){
		printf(",append");
	}
	printf("O_NONBLOCK %08x\n",O_NONBLOCK);
	if(ret & O_NONBLOCK){
		printf(",nonblock");
	}
	
	printf("O_SYNC %08x\n",O_SYNC);
	if(ret & O_SYNC){
		printf(",sync");
	}
	printf("\n");

	return 0;
}