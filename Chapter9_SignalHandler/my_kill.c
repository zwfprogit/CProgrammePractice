/*例9-5实现了自己的kill命令，但不支持-l选项(显示信号编号)*/

//为简化实现，本程序只支持按信号的编号而不是信号名发送信号
//感兴趣的读者可以按照自己的系统下的signal.h增加名字到编号的映射表。

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/*信号处理函数*/
void handler_sigint(int signo)
{
	if(signo == SIGINT)
		printf("recv SIGINT\r\n");
}




int main(int argc,char **argv)
{
	int i,j;
	int signum = SIGTERM;//默认发送SIGTERM
	pid_t pid;
	
	/*首先检查参数个数*/
	if(argc != 2 && argc != 4){
		printf("Usage: ./my_kill <-s signum> [PID]\r\n");
		exit(0);
	}
	/*从命令行参数解析出信号编号*/
	for(i=0;i < argc;i++){
		if(!strcmp(argv[i],"-s")){
			signum = atoi(argv[i+1]);
			break;
		}
	}
	/*解析进程号*/
	if(argc == 2){
		pid = atoi(argv[1]);
	}else{
		for(j=1;j < argc;j++){
			if(j != i && j != i+1){
				pid = atoi(argv[j]);
				printf("[PID]=%d i=%d j=%d\r\n",pid,i,j);
				break;
			}
		}
	}
	
	/*发送信号*/
	if(kill(pid,signum) < 0){
		perror("kill");
		exit(1);
	}

	printf("exit main process\r\n");
	return 0;
}

/**
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter9 SignalHandler$ ./my_signal
(ALT+C)
recv SIGINT
(ALT+C)
recv SIGINT
(ALT+\)//发送SIGQUIT信号结束进程
Quit (core dumped)

也可以用kill +进程ID杀死进程

*/