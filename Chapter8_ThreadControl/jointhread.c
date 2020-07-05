/*例8-3演示主线程通过pthread_join等待辅助线程结束*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

int *assistthread(void *arg)
{
	printf("I am helping to do some jobs\r\n");
	sleep(3);
	pthread_exit(0);
}

int main()
{
	pthread_t assistthid;//assist:辅助
	int status;
	
	if(pthread_create(&assistthid,NULL,(void *)assistthread,NULL) != 0){
		perror("pthread_create failed");
		exit(1);
	}
	//int pthread_join(pthread_t thread, void **retval);
	pthread_join(assistthid,(void *)&status);
	printf("assistthread's exit is caused %d\r\n",status);

	exit(0);
}

/**
编译程序并运行：
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter8 ThreadControl$ ./jointhread
I am helping to do some jobs
assistthread's exit is caused 0

从运行结果可以看出pthread_join会阻塞主线程，等待assistthread线程结束，pthread_exit
结束时退出码是0，pthread_join得出status也为0，两者一致。
*/