#ifndef __MY_RECV_H__
#define __MY_RECV_H__

#define BUFFERSIZE 1024
void my_err(const char *e,int line);
int my_recv(int conn_fd,char *data_buf,int len);


#endif