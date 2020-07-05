/*例8-2演示pthread_once函数的使用*/
/*在某些情况下，函数执行次数要被限制为一次，这种情况下就要用pthread_once函数。
通过例8-2说明。该实例中创建两个线程，连个线程分贝通过pthread_once调用同一个
函数，结果被调用的函数只被执行一次。
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

pthread_once_t once = PTHREAD_ONCE_INIT;

void run(void)
{
	printf("Function run is running in thread %lu\r\n",pthread_self());
}

int *thread1(void *arg)
{
	pthread_t thid;
	thid = pthread_self();
	printf("Current thread's ID is %lu\r\n",thid);
	pthread_once(&once,run);
	printf("thread1 ends\r\n");

	return NULL;
}

int *thread2(void *arg)
{
	pthread_t thid;
	thid = pthread_self();
	printf("Current thread's ID is %lu\r\n",thid);
	pthread_once(&once,run);
	printf("thread2 ends\r\n");

	return NULL;
}


int main()
{
	pthread_t thid1,thid2,thid3;
	
	printf("main process ID is %d\r\n",getpid());
	printf("main thread ID is %lu\r\n",pthread_self());
	
	if(pthread_create(&thid1,NULL,(void *)thread1,NULL) != 0){
		perror("pthread_create thread1 failed");
		exit(1);
	}
	if(pthread_create(&thid2,NULL,(void *)thread2,NULL) != 0){
		perror("pthread_create thread2 failed");
		exit(1);
	}

	sleep(3);
	printf("main thread exit!\r\n");

	exit(0);
}

/**
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter8 ThreadControl$ ./oncerun
main process ID is 2250
main thread ID is 139908120098560
Current thread's ID is 139908103374592
Function run is running in thread 139908103374592
thread2 ends
Current thread's ID is 139908111767296
thread1 ends
main thread exit!

从运行结果可以看出函数run在线程thread2运行了一次，线程thread1也调用了run函数，
但是并未执行。
*/