#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#define IP "192.168.0.113"
#define PORT 10086
#define SIZE 100

int main()
{
	int socketID = 0;
	int newID = 0;
	int ret = 0;
	int addrLength = 0;
	char buf[SIZE] = "";
	struct sockaddr_in addr;
	fd_set readFds;
	fd_set tmpFds;
	int maxFd = 0;
	//socket
	socketID = socket(AF_INET, SOCK_STREAM, 0);
	if (socketID < 0)
	{
		perror("socket error");
		return -1;
	}
	//bind
	addrLength = sizeof(addr);
	memset(&addr, 0, addrLength);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = INADDR_ANY;
	if (0 > bind(socketID, (struct sockaddr *)&addr, addrLength))
	{
		perror("bind error");
		close(socketID);
		return -1;
	}
	//listen
	listen(socketID, 5);
	//readfds, maxfd, tmpfds
	FD_ZERO(&readFds);
	FD_SET(socketID, &readFds);
	maxFd = socketID;
	tmpFds = readFds;//每次承接总fds表的tmp变量
	while(1)
	{
		readFds = tmpFds;
		printf("socket/bind/listen ok, and call select \r\n");
		ret = select(maxFd + 1, &readFds, NULL, NULL, NULL);//查看多路复用的可读性
		if (ret < 0)
		{
			perror("select error");
			break;
		}
		else if ( 0 == ret )
		{
			printf("select time out \r\n");
			break;
		}
		
		int i = 0;
		for (i = 0; i <= maxFd; i++)
		{
			//判断 哪个文件可读
			if (i == socketID)
			{
				if (FD_ISSET(socketID, &readFds))//查看该socketid是否在readfds内部
				{
					newID = accept(socketID, (struct sockaddr *)&addr, &addrLength);
					if (newID < 0)
					{
						perror("accept error");
						break;
					}
					FD_SET(newID, &tmpFds);
					if (maxFd < newID)
						maxFd = newID;
					printf("new client connect ok, port=%d\r\n", ntohs(addr.sin_port));
				}
			}//if 		
			else if (FD_ISSET(i, &readFds))
			{
				//recv -- 0
				ret = recv(i, buf, SIZE - 1, 0);
				if (ret > 0)
				{
					printf("recv %d : %s\r\n", i, buf);
				}
				else if ( 0 == ret)
				{
					FD_CLR(i, &tmpFds);
					close(i);				
				}
				else 
				{
					perror("recv error\r\n");
				}
			}//else if 			
		}//for 
	}//while
	//close
	close(socketID);
	return 0;
}


