/*例9-7演示定时器的用法*/


#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

// time_t time(time_t *tloc);
/*信号处理函数*/
void handler_sigtime(int signo)
{
	time_t t;
	time(&t);
	switch(signo){
	case SIGALRM:
		printf("[%ld]recv SIGALRM\n",t);
		break;
	case SIGPROF:
		printf("[%ld]recv SIGPROF\n",t);
		break;
	default:
		break;
	}
}


/**
struct itimerval {
	struct timeval it_interval; //Interval for periodic timer
	struct timeval it_value;    //Time until next expiration
};

struct timeval {
   time_t      tv_sec;         //seconds 
   suseconds_t tv_usec;        // microseconds
};
*/

/**
int getitimer(int which, struct itimerval *curr_value);
int setitimer(int which, const struct itimerval *new_value,
            struct itimerval *old_value);
*/

int main(void)
{	time_t t;
	struct itimerval value;
	/*安装信号处理程序*/
	signal(SIGALRM,handler_sigtime);
	signal(SIGPROF,handler_sigtime);
	
	/*初始化value结构*/
	value.it_value.tv_sec = 1;//第一次1s触发信号
	value.it_value.tv_usec = 0;
	value.it_interval.tv_sec = 5;//第二次开始每5s触发信号。
	value.it_value.tv_usec = 0;
	
	/*设置当时器*/
	time(&t);
	printf("[%ld]start setitimer\n",t);
	/**
	ITIMER_REAL:按实际时间计时，计时到达时将给进程发送alarm信号，相当于高精度的alarm函数。
	ITIMER_PROF：进程执行的时间以及内核因本进程而消耗的时间都计时。
	*/
	setitimer(ITIMER_REAL,&value,NULL);
	setitimer(ITIMER_PROF,&value,NULL);
	
	while(1);

	return 0;
}

/**
[Amyroot@myroot:~/Linux-C/LinuxC编程实战/Chapter9 SignalHandler$ ./test_setitimer
[1565408566]start setitimer
[1565408567]recv SIGALRM
[1565408568]recv SIGPROF
[1565408572]recv SIGALRM
[1565408577]recv SIGALRM
[1565408580]recv SIGPROF
[1565408582]recv SIGALRM
[1565408587]recv SIGALRM
从执行结果可以发现，开始SIGALRM先于SIGPROF，总体上SIGALRM的次数要多于SIGPROF的次数，
这符合预期，因为整个系统并不是只运行这个进程。
*/










