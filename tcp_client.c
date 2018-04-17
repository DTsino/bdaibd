#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<unistd.h>

#define IP "127.0.0.1"  
#define SPORT  10086
#define CPORT  10011
#define SIZE 100

int main()
{
	//定义变量
	int socketID = 0;
	int ret = 0;
	struct sockaddr_in serverAddr;
	struct sockaddr_in myAddr;
	int addrLength = sizeof(struct sockaddr_in);
	char buf[SIZE] = "";
	
	//打开socket
	socketID = socket(AF_INET, SOCK_STREAM, 0);
	if (socketID < 0)
	{
		perror("socket error");
		return -1;
	}
	//绑定自己的ip和port
	memset(&myAddr, 0, addrLength);
	myAddr.sin_family = AF_INET;
	myAddr.sin_port = htons(CPORT);
	myAddr.sin_addr.s_addr = INADDR_ANY;
	ret = bind(socketID, (struct sockaddr *)&myAddr, addrLength);
	if (ret < 0)
	{
		perror("bind error");
		close(socketID);
		return -1;
	}
	printf("bind success\r\n");
	
	//设置服务器的ip&&port
	memset(&serverAddr, 0, addrLength);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SPORT);
	serverAddr.sin_addr.s_addr = inet_addr(IP);
	//发送连接请求
	ret = connect(socketID, (struct sockaddr *)&serverAddr, addrLength);
	//ret = connect(socketID, (struct sockaddr *)&serverAddr, addrLength);
	if (ret < 0)
	{
		perror("connect error");
		close(socketID);
		return -1;
	}
    printf("连接成功\n");
	while(1)
	{
		scanf("%s", buf);
		//发送数据
		ret = send(socketID, buf, strlen(buf), 0);
		if (ret > 0)
		{
			printf("send msg success!\r\n");
		}
		if (0 == strcmp(buf, "quit"))
			break;
	}
	//关闭socket
	close(socketID);
	return 0;
}
