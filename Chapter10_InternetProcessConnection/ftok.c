/*例10-9是一个获取消息队列键值的例子*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>


int main()
{
	int i;
	for(i=0;i<=5;i++){
		printf("key[%d]=%d\r\n",i,ftok(".",i));
	}
	exit(0);
	return 0;
}
/*
key[0]=95186
key[1]=16872402
key[2]=33649618
key[3]=50426834
key[4]=67204050
key[5]=83981266
*/