/*互斥锁应用实例*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex; //互斥锁
pthread_cond_t cond; //条件变量

 
// 线程一
void *thread1(void *arg)
{
	pthread_cleanup_push((void*)pthread_mutex_unlock,(void*)&mutex);
	while(1){
		printf("thread1 exe pthread_mutex_lock==1\r\n");
		pthread_mutex_lock(&mutex);
		printf("thread1 exe pthread_mutex_lock==2\r\n");
		/*pthread_cond_wait(pthread_cond_t *cond,pthread_mutex_t *mutex)*/
		/*pthread_cond_wait:函数释放有mutex指向的互斥锁，同时使当前线程关于cond指向的条件变量阻塞，
		知道条件被信号唤醒。
		*/
		printf("thread1 exe pthread_cond_wait==1\r\n");
		pthread_cond_wait(&cond,&mutex);
		printf("thread1 exe pthread_cond_wait==2\r\n");
		printf("thread1 applied the condition\r\n");
		printf("thread1 exe pthread_mutex_unlock==1\r\n");
		pthread_mutex_unlock(&mutex);
		printf("thread1 exe pthread_mutex_unlock==2\r\n");
		sleep(4);
	}
	pthread_cleanup_pop(0);
}

 

// 线程二

void *thread2(void *arg)
{
	while(1){
		printf("thread2 exe pthread_mutex_lock==1\r\n");
		pthread_mutex_lock(&mutex);
		printf("thread2 exe pthread_mutex_lock==2\r\n");
		/*pthread_cond_wait(pthread_cond_t *cond,pthread_mutex_t *mutex)*/
		/*pthread_cond_wait:函数释放有mutex指向的互斥锁，同时使当前线程关于cond指向的条件变量阻塞，
		知道条件被信号唤醒。
		*/
		printf("thread2 exe pthread_cond_wait==1\r\n");
		pthread_cond_wait(&cond,&mutex);
		printf("thread2 exe pthread_cond_wait==2\r\n");
		printf("thread2 applied the condition\r\n");
		printf("thread2 exe pthread_mutex_unlock==1\r\n");
		pthread_mutex_unlock(&mutex);
		printf("thread2 exe pthread_mutex_unlock==2\r\n");
		sleep(1);
	}
}


int main(void)
{
	pthread_t tid1, tid2,tid3;
	printf("pthread condition variable study\r\n");
	pthread_mutex_init(&mutex, NULL); //初始化互斥锁
	pthread_cond_init(&cond,NULL);

	// 创建 2 个线程
	pthread_create(&tid1, NULL, thread1, NULL);
	pthread_create(&tid2, NULL, thread2, NULL);

	do{
		printf("pthread_cond_signal release the condition\r\n");
		pthread_cond_signal(&cond);
		sleep(1);
	} while(1);
	
	sleep(5);
	pthread_exit(0);

	return 0;
}

/**
编译程序并运行：
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter8 ThreadControl$ ./condition
pthread condition variable study
pthread_cond_signal release the condition
thread1 exe pthread_mutex_lock==1
thread1 exe pthread_mutex_lock==2	//thread1启动，成功执行pthread_mutex_lock，获得锁
thread1 exe pthread_cond_wait==1	//thread1调用pthread_cond_wait释放由pthread_mutex_lock指向的锁，同时使当前线程关于条件变量阻塞，知道条件被信号唤醒
thread2 exe pthread_mutex_lock==1	
thread2 exe pthread_mutex_lock==2	//thread2启动，成功执行pthread_mutex_lock，获得锁(因thread1的锁被pthread_cond_wait释放然后关于条件变量阻塞)
thread2 exe pthread_cond_wait==1	//thread2调用pthread_cond_wait释放由pthread_mutex_lock指向的锁，同时使当前线程关于条件变量阻塞，知道条件被信号唤醒
pthread_cond_signal release the condition	//pthread_cond_signal激活一个条件成立的线程，如果多个线程等待，按入队顺序激活其中一个，由于thread1条件先成立，激活后先运行 thread1
thread1 exe pthread_cond_wait==2
thread1 applied the condition
thread1 exe pthread_mutex_unlock==1
thread1 exe pthread_mutex_unlock==2	//thread1解锁
pthread_cond_signal release the condition //继续释放条件成立的线程
thread2 exe pthread_cond_wait==2
thread2 applied the condition
thread2 exe pthread_mutex_unlock==1
thread2 exe pthread_mutex_unlock==2
pthread_cond_signal release the condition	//因为thread1被挂起4s，所以释放后thread2继续运行
thread2 exe pthread_mutex_lock==1
thread2 exe pthread_mutex_lock==2
thread2 exe pthread_cond_wait==1
pthread_cond_signal release the condition
thread2 exe pthread_cond_wait==2
thread2 applied the condition
thread2 exe pthread_mutex_unlock==1
thread2 exe pthread_mutex_unlock==2
*/