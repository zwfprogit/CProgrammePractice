/*例6-6 采用ioctl获取网络设备信息，在编写网络相关程序时可能会用到*/
// 示例ioctl的使用
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <unistd.h>

/*自定义的错误处理函数*/
void my_err(const char * err_string,int line)
{
	fprintf(stderr,"line: %d ",line);
	perror(err_string);
	exit(1);
}

unsigned char g_eth_name[16]; 
unsigned char g_macaddr[6];
unsigned int g_ipaddr;
unsigned int g_broadcast_ipaddr;
unsigned int g_subnetmask;
#if 0
/* for compatibility with glibc net/if.h */
#if __UAPI_DEF_IF_IFREQ
struct ifreq {
#define IFHWADDRLEN     6
	union
	{
		char    ifrn_name[IFNAMSIZ];            /* if name, e.g. "en0" */
	} ifr_ifrn;

	union {
		/*
		struct sockaddr {
			unsigned short    sa_family;    // 2 bytes address family, AF_xxx
			char              sa_data[14];     // 14 bytes of protocol address
		};
		*/
		struct  sockaddr ifru_addr;
		struct  sockaddr ifru_dstaddr;
		struct  sockaddr ifru_broadaddr;
		struct  sockaddr ifru_netmask;
		struct  sockaddr ifru_hwaddr;
		short   ifru_flags;
		int     ifru_ivalue;
		int     ifru_mtu;
		struct  ifmap ifru_map;
		char    ifru_slave[IFNAMSIZ];   /* Just fits the size */
		char    ifru_newname[IFNAMSIZ];
		void *  ifru_data;
		struct  if_settings ifru_settings;
	} ifr_ifru;
};
#endif /* __UAPI_DEF_IF_IFREQ */
#endif
/*初始化网络，获取当前网络设备信息*/
void init_net(void)
{
	int i;
	int sockfd;
	struct sockaddr_in sin;
	/*linux/if.h:struct ifreq
	*/
	struct ifreq ifr;
	
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd == -1){
		my_err("socket",__LINE__);
	}
	strcpy(g_eth_name,"enp0s3");//网卡名enp0s3
	strcpy(ifr.ifr_ifrn.ifrn_name,g_eth_name);
	printf("eth name:\t%s\n",g_eth_name);
	
	/*获取并打印网卡地址*/
	//linux/sockios.h:#define SIOCGIFHWADDR   0x8927          /* Get hardware address  
	if(ioctl(sockfd,SIOCGIFHWADDR,&ifr) < 0){
		my_err("ioctl",__LINE__);
	}
	memcpy(g_macaddr,ifr.ifr_ifru.ifru_hwaddr.sa_data,6);
	printf("Local mac:\t");
	for(i = 0;i < 5;i++){
		printf("%.2x:",g_macaddr[i]);
	}
	printf("%.2x:\n",g_macaddr[i]);
	
	// 获取并打印IP地址
	//SIOCGIFADDR     0x8915          /* get PA address   
	if(ioctl(sockfd,SIOCGIFADDR,&ifr) < 0){
		my_err("ioctl",__LINE__);
	}
	memcpy(&sin,&ifr.ifr_ifru.ifru_addr,sizeof(struct sockaddr_in));
	g_ipaddr = sin.sin_addr.s_addr;
	printf("Local enp0s3\t%s\n",inet_ntoa(sin.sin_addr));

	// 获取并打印广播地址
	//#define SIOCGIFBRDADDR  0x8919          /* get broadcast PA address     */
	if(ioctl(sockfd,SIOCGIFBRDADDR,&ifr) < 0){
		my_err("ioctl",__LINE__);
	}
	memcpy(&sin,&ifr.ifr_ifru.ifru_broadaddr,sizeof(struct sockaddr_in));
	g_broadcast_ipaddr = sin.sin_addr.s_addr;
	printf("broadcast enp0s3\t%s\n",inet_ntoa(sin.sin_addr));
	
	// 获取并打印子网掩码
	//#define SIOCGIFNETMASK  0x891b          /* get network PA mask   
	if(ioctl(sockfd,SIOCGIFNETMASK,&ifr) < 0){
		my_err("ioctl",__LINE__);
	}
	memcpy(&sin,&ifr.ifr_ifru.ifru_netmask,sizeof(struct sockaddr_in));
	g_subnetmask = sin.sin_addr.s_addr;
	printf("subnetmask enp0s3\t%s\n",inet_ntoa(sin.sin_addr));
	
	close(sockfd);
}



int main()
{
	/*initialize...*/
	init_net();

	/*do something*/
	return 0;
}