/*例8-4演示如何创建和使用私有数据*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

pthread_key_t key;

void *thread2(void *arg)
{
	int tsd = 5;
	printf("thread2 %lu is running\r\n",pthread_self());
	pthread_setspecific(key,(void*)&tsd);
	printf("thread2 %lu is return %d\r\n",pthread_self(),(int)(*((int*)pthread_getspecific(key))));
}

void *thread1(void *arg)
{
	int tsd = 0;
	pthread_t thid;
	printf("thread1 %lu is running\r\n",pthread_self());
	pthread_setspecific(key,(void*)&tsd);
	pthread_create(&thid,NULL,thread2,NULL);
	sleep(1);
	printf("thread1 %lu is return %d\r\n",pthread_self(),(int)(*((int*)pthread_getspecific(key))));
}

int main()
{
	pthread_t thid;

	printf("main thread begins running\r\n");
	pthread_key_create(&key,NULL);
	if(pthread_create(&thid,NULL,thread1,NULL) != 0){
		perror("pthread_create failed");
		exit(1);
	}
	sleep(3);
	pthread_key_delete(key);
	printf("main thread exit\r\n");

	exit(0);
}

/**
编译程序并运行：
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter8 ThreadControl$ ./tsd
main thread begins running
thread1 140514736940800 is running
thread2 140514728548096 is running
thread2 140514728548096 is return 5
thread1 140514736940800 is return 0
main thread exit
*/