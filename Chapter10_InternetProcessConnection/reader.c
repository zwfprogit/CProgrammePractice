/*例10-18和例10-19是writer和reader程序，两个程序在进入共享内存区之前，
首先检查信号集中信号的值是否为1(相当于是否能进入共享内存区)，如果不为
1，如果不为1，调用sleep()进入睡眠状态知道信号的值变为1.进入共享内存区
后，将信号的值减1(相当于加锁)，这样实现了互斥的访问共享资源。在退出共享
内存时，将信号值加1(相当于解锁)。
*/

#include "sharemem.h"


int main()
{
	int semid,shmid;
	char *shmaddr;
	char write_str[SHM_SIZE];
	
	if((shmid = createshm(".",'m',SHM_SIZE)) == -1){
		my_err("createshm",__LINE__);	
	}
	//void *shmat(int shmid, const void *shmaddr, int shmflg);
	if((shmaddr = shmat(shmid,(char *)0,0)) == (char *)-1){
		my_err("shmat attach shared memory error!",__LINE__);	
	}
	
	if((semid = opensem(".",'s')) == -1){
		my_err("opensem",__LINE__);	
	}
	
	while(1){
		printf("reader: ");
		wait_sem(semid,0);	/*等待信号值为1*/
		sem_p(semid,0); /*P操作*/
		
		printf("%s\n",shmaddr);
		sleep(10);	/*使writer处于阻塞状态*/
		sem_v(semid,0);/*v操作*/
		sleep(10);	/*等待writer进行读操作*/
	}
}


/**
编译两个程序：注意其中的-I引用的是sharemem.h所在目录
gcc -o writer -I./ writer.c
gcc -o reader -I./ reader.c
同时在连个终端运行writer和reader，在writer端输入hello,reader!等一会儿，
看到reader端输出后，在writer端输入new information。执行结果如下

myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter10 InternetProcessConnection$ ./writer
writer: hello,reader!
writer: new information
writer: 

myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter10 InternetProcessConnection$ ./reader
reader: hello,reader!
reader: new information
*/