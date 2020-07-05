/**
信号量一般用于处理访问临界资源的同步问题。下面通过例10-15和例10-16中的semserver.c和semclient.c程序来演示信号量如何控制
对资源的访问。semserver创建一个信号集，并对信号量循环减1，相当于分配资源。semclient执行检查信号量，如果其值大于0代表有
资源可以用，继续执行，如果小于0代表资源已经分配完毕，进程semserver退出。
*/
/**
例10-15 semserver.c
*/


#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_RESOURCE	5


int main(void)
{
	key_t key;
	int semid;
	struct sembuf sbuf = {0,-1,IPC_NOWAIT};
	 union semun {
	   int              val;    /* Value for SETVAL */
	   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	   unsigned short  *array;  /* Array for GETALL, SETALL */
	   struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
    };
	union semun semopts;
	
	if((key=ftok(".",'s')) == -1){
		perror("ftok error!");
		exit(1);
	}
	//int semget(key_t key, int nsems, int semflg);
	if((semid = semget(key,3,IPC_CREAT|0666)) == -1){
		perror("semget error!");
		exit(1);
	}
	printf("semid: %d\n",semid);
	semopts.val = MAX_RESOURCE;
	//int semctl(int semid, int semnum, int cmd, ...);
	if(semctl(semid,0,SETVAL,semopts) == -1){
		perror("semctl error!\n");
		exit(1);
	}
	
	printf("semopts.val: %d\n",semctl(semid,0,GETVAL,semopts));
	
	while(1){
		if(semop(semid,&sbuf,1) == -1){
			perror("semop error!");
			exit(1);
		}
		printf("sem_num: %hu sem_op: %hd sem_flg: %d\n",sbuf.sem_num,sbuf.sem_op,sbuf.sem_flg);
		pause();
	}

	return 0;
}