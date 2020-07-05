/*例8-1演示线程的创建过程*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

int *thread(void *arg)
{
	pthread_t newthid;
	newthid = pthread_self();
	printf("this is a new thread,thread ID is %lu\r\n",newthid);

	return NULL;
}

int main()
{
	pthread_t thid;
	
	printf("main process ID is %d\r\n",getpid());
	printf("main thread ID is %lu\r\n",pthread_self());
	
	if(pthread_create(&thid,NULL,(void *)thread,NULL) != 0){
		perror("pthread_create failed");
		exit(1);
	}
	sleep(1);

	exit(0);
}

/**
编译程序并运行：
gcc -o createthread createthread.c -lpthread
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter8 ThreadControl$ ./createthread
main process ID is 2110
main thread ID is 139904530368256
this is a new thread,thread ID is 139904522036992
*/