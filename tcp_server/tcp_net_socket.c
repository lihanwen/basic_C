#include "tcp_net_socket.h"
int tcp_init(const char *ip,int port) // 初始化
{
	int sfd = socket(AF_INET, SOCK_STREAM, 0);//create Socket & apply for system
	if(sfd == -1)
	{
		perror("socket");
		exit(-1);
	}
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(struct sockaddr));
	serveraddr.sin_family 	   = AF_INET;
	serveraddr.sin_port 	   = htons(port);
	serveraddr.sin_addr.s_addr = inet_addr(ip);//或INADDR_ANY
	//bind new Socket with specified IP & port
	if(bind(sfd, (struct sockaddr*)&serveraddr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		close(sfd);
		exit(-1);
	}
	if(listen(sfd, 10) == -1) //监听 并 设置最大链接数为10
	{
		perror("listen");
		close(sfd);
		exit(-1);
	}
	return sfd;
}
int tcp_accept(int sfd) //server accept
{
	struct sockaddr_in clientaddr;
	memset(&clientaddr, 0, sizeof(struct sockaddr_in));
	int addrlen = sizeof(struct sockaddr_in);
	int new_fd = accept(sfd, (struct sockaddr_in*)&clientaddr, &addrlen);
	//sfd 接受客户端链接，并创建新的socket为new_fd，将请求链接的客户端的IP/Port保存在机构体clientaddr中
	if(new_fd == -1)
	{
		perror("accept");
		close(sfd);
		exit(-1);
	}
	printf("%s %d success connect..\n",inet_ntoa(clientaddr.sin_addr),\
									   ntohs(clientaddr.sin_port));
	return new_fd;									   
}
int tcp_connect(const char* ip, int port) //client connect
{
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	//register and apply for system new socket
	if(sfd == -1)
	{
		perror("socket");
		exit(-1);
	}	
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	serveraddr.sin_addr.s_addr = inet_addr(ip);
	//将sfd链接至指定的服务器网络地址serveraddr
	if(connect(sfd, (struct sockaddr*)&serveraddr, sizeof(struct sockaddr)) == -1)
	{
		perror("connect");
		close(sfd);
		exit(-1);
	}
	return sfd;
}
void signalhandler(void)
{
	sigset_t sigSet;
	sigemptyset(&sigSet);
	sigaddset(&sigSet, SIGINT);
	sigaddset(&sigSet, SIGQUIT);
	sigprocmask(SIG_BLOCK, &sigSet, NULL);
}

