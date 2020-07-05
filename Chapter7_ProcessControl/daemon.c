#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>
#include <signal.h>

/*自定义的错误处理函数*/
void my_err(const char * err_string,int line)
{
	fprintf(stderr,"line: %d ",line);
	perror(err_string);
	exit(1);
}


int init_daemon(void)
{
	pid_t pid;
	int i;
	
	/*忽略终端I/O信号，STOP信号*/
	signal(SIGTTOU,SIG_IGN);
	signal(SIGTTIN,SIG_IGN);
	signal(SIGTSTP,SIG_IGN);
	signal(SIGHUP,SIG_IGN);
	
	pid = fork();
	if(pid > 0){
		exit(0);/*结束父进程，使得子进程成为后台进程*/
	}
	else if(pid < 0){
		return -1;
	}
	/*建立一个新的进程组，在这个新的进程组中，子进程成为这个进程组的首进程，以使该进程脱离所有终端*/
	setsid();
	/*再次新建一个子进程，退出父进程，保证该进程不是进程组长，同时让该进程无法再打开一个新终端*/
	pid = fork();
	if(pid > 0){
		exit(0);/*结束父进程*/
	}
	else if(pid < 0){
		return -1;
	}
	/*关闭所有所有从父进程继承的不再需要的文件描述符*/
	for(i=0;i<NOFILE;close(i++));
	/*改变工作目录，使得进程不与任何文件系统联系*/

	chdir("/");
	/*将文件屏蔽字设置为0*/
	umask(0);

	/*忽略SIGCHLD信号*/
	signal(SIGCHLD,SIG_IGN);
	return 0;
}


int main(void)
{
	time_t now;
	init_daemon();
	syslog(LOG_USER|LOG_INFO,"测试守护进程！\n");
	while(1){
		sleep(8);
		time(&now);
		syslog(LOG_USER|LOG_INFO,"系统时间：\t%s\t\t\n",ctime(&now));
	}
}

#if 0
/*
编译运行此程序。然后用ps -ef查看进程状态，该进程状态如下：
UID        PID  PPID  C STIME TTY          TIME CMD
myroot    1945     1  0 22:23 ?        00:00:00 ./daemon
从结果可以看出该进程具备守护进程的所有特征。
查看系统日志的结果片段如下：
cat /var/log/syslog
Jul 29 22:23:45 myroot daemon: 测试守护进程！
Jul 29 22:23:53 myroot daemon: 系统时间：#011Mon Jul 29 22:23:53 2019
Jul 29 22:24:01 myroot daemon: 系统时间：#011Mon Jul 29 22:24:01 2019
Jul 29 22:24:09 myroot daemon: 系统时间：#011Mon Jul 29 22:24:09 2019

注意：使用syslog函数前，首先需要配置/etc/syslog.conf文件，在改文件的最后
加入一行user.*/var/log/test.log。然后重新启动syslog服务，命令分别是：
/etc/init.d/syslog stop和/etc/init.d/syslog start。这样syslog的信息就会
写入test.log 
*/

#endif










