/*例9-6用函数alarm实现了网络命令ping的功能*/

/**
如果之前已经调用过alarm，则返回之前设置的定时器剩余时间；如果之前没有设置定时器，则返回0.
*/


/*模拟ping 程序的框架*/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void send_ip(void)
{
	/*发送回请求报文，这里只是打印消息*/
	printf("send a icmp echo request packet\r\n");
}

void recv_ip(void)
{
	/*挂起在套接字上等待数据并解析报文，这里只是使用死循环*/
	printf("recv_ip\r\n");
	while(1);
}

/*信号处理函数*/
void handler_sigalarm(int signo)
{
	send_ip();
	alarm(2);
}




int main(int argc,char **argv)
{
	/*安装信号处理程序*/
	signal(SIGALRM,handler_sigalarm);
	
	/*触发一个SIGALRM信号给本进程*/
	raise(SIGALRM);
	//alarm(1);//也可以实现
	recv_ip();
	return 0;
}

/**
myroot@myroot:~/Linux-C/LinuxC编程实战/Chapter9 SignalHandler$ ./simulate_ping
send a icmp echo request packet
recv_ip
send a icmp echo request packet
send a icmp echo request packet
send a icmp echo request packet
send a icmp echo request packet
send a icmp echo request packet
send a icmp echo request packet
send a icmp echo request packet
send a icmp echo request packet
send a icmp echo request packet

1，调用raise(SIGALRM)触发一个SIGALRM信号给本进程
2，handler_sigalarm处理SIGALRM信号，同时又设置了一个alarm定时器，2s超时将产生SIGALRM信号，
handler_sigalarm在此处理信号，依次周期进行。
*/










