/*共享内存的应用实例*/
/*
	本例10-17通过读者写者问题(不考虑优先级)来演示共享内存和信号量如何配合使用。
这里的读者写者问题要求一个进程读共享内存的时候，其他进程不能写内存：当一个进
程写共享内存的时候，其他进程不能读内存。
	首先定义一个包含公用函数的头文件sharemem.h
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>

#define SHM_SIZE 1024

/*自定义的错误处理函数*/
void my_err(const char * err_string,int line)
{
	fprintf(stderr,"line: %d ",line);
	perror(err_string);
	exit(1);
}

/* arg for semctl system calls. */
union semun {
	int val;			/* value for SETVAL */
	struct semid_ds *buf;	/* buffer for IPC_STAT & IPC_SET */
	unsigned short  *array;	/* array for GETALL & SETALL */
};

/*创建信号量函数*/
int createsem(const char *pathname,int proj_id,int members,int init_val)
{
	key_t msgkey;
	int index,sid;
	union semun semopts;
	if((msgkey=ftok(pathname,proj_id)) == -1){
		my_err("createsem ftok",__LINE__);
	}
	//int semget(key_t key, int nsems, int semflg);
	if((sid = semget(msgkey,members,IPC_CREAT|0666)) == -1){
		my_err("createsem semget",__LINE__);
	}
	/*初始化操作*/
	semopts.val = init_val;
	for(index = 0;index < members;index++){
		//int semctl(int semid, int semnum, int cmd, ...);
		semctl(sid,index,SETVAL,semopts);
	}
	return (sid);
}



/*打开信号量函数*/
int opensem(const char *pathname,int proj_id)
{
	key_t msgkey;
	int sid;

	if((msgkey=ftok(pathname,proj_id)) == -1){
		my_err("opensem ftok",__LINE__);
	}
	//int semget(key_t key, int nsems, int semflg);
	if((sid = semget(msgkey,0,IPC_CREAT|0666)) == -1){
		my_err("opensem semget",__LINE__);
	}
	return (sid);
}

/*P操作函数*/
/**
struct sembuf {
	unsigned short  sem_num;// semaphore index in array 
	short		sem_op;		//semaphore operation
	short		sem_flg;	// operation flags 
};
*/
int sem_p(int semid,int index)
{
	struct sembuf buf = {0,-1,IPC_NOWAIT};

	if(index < 0){
		perror("index of array cannot equals a minus(减) value!");
		return -1;
	}
	buf.sem_num = index;
	//int semop(int semid, struct sembuf *sops, size_t nsops);
	if(semop(semid,&buf,1) == -1){
		perror("a wrong operation to semaphore occured!");
		return -1;
	}
	return 0;
}

/*V操作函数*/
int sem_v(int semid,int index)
{
	struct sembuf buf = {0,1,IPC_NOWAIT};

	if(index < 0){
		perror("index of array cannot equals a minus(减) value!");
		return -1;
	}
	buf.sem_num = index;
	//int semop(int semid, struct sembuf *sops, size_t nsops);
	if(semop(semid,&buf,1) == -1){
		perror("a wrong operation to semaphore occured!");
		return -1;
	}
	return 0;
}

/*删除信号集函数*/
int sem_delete(int semid)
{
	//int semctl(int semid, int semnum, int cmd, ...);
	return semctl(semid,0,IPC_RMID);
}


/*等待信号为1*/
int wait_sem(int semid,int index)
{
	while(semctl(semid,index,GETVAL,0) == 0){
		sleep(1);
	}
	return 1;
}


/*创建共享内存函数*/
int createshm(const char *pathname,int proj_id,size_t size)
{
	key_t shmkey;
	int index,shmid;
	union semun semopts;
	if((shmkey=ftok(pathname,proj_id)) == -1){
		perror("createshm ftok");
		return -1;
	}
	//int shmget(key_t key, size_t size, int shmflg);
	if((shmid = shmget(shmkey,size,IPC_CREAT|0666)) == -1){
		perror("createshm shmget");
		return -1;
	}
	
	return (shmid);
}




