/**
信号量一般用于处理访问临界资源的同步问题。下面通过例10-15和例10-16中的semserver.c和semclient.c程序来演示信号量如何控制
对资源的访问。semserver创建一个信号集，并对信号量循环减1，相当于分配资源。semclient执行检查信号量，如果其值大于0代表有
资源可以用，继续执行，如果小于0代表资源已经分配完毕，进程semserver退出。
*/
/**
例10-16 semclient.c
*/


#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
	key_t key;
	int semid,semval;

	
	if((key=ftok(".",'s')) == -1){
		perror("ftok error!\n");
		exit(1);
	}
	//int semget(key_t key, int nsems, int semflg);
	if((semid = semget(key,1,IPC_CREAT|0666)) == -1){
		perror("semget error!\n");
		exit(1);
	}

	
	while(1){
		//int semctl(int semid, int semnum, int cmd, ...);
		if((semval = semctl(semid,0,GETVAL,0)) == -1){
			perror("semctl error!\n");
			exit(1);
		}
		if(semval > 0){
			printf("Still %d resource can be used\n",semval);
		}else{
			printf("No more resource can be used!\n");
			break;
		}
		sleep(3);
	}

	return 0;
}