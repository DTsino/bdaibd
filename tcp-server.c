/*************************************************************************
	> File Name: tcp-server.c
	> Author: 
	> Mail: 
	> Created Time: Mon 09 Apr 2018 10:49:24 AM CST
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>

#define IP "127.0.0.1"
#define PORT 10086
#define SIZE 2048

int main()
{
    int socketid = 0;
    int newid = 0;
    int ret = 0;
    int lenth = 0;
    int maxfd=0;
    char buf[SIZE]= "";
    struct sockaddr_in addr;
    fd_set readfds;
    fd_set tmpfds;
    socketid=socket(AF_INET,SOCK_STREAM,0);
    if(socketid<0)
    {
        printf("socket error\n");
        return -1;
    }
    //绑定
    lenth=sizeof(addr);
    memset(&addr,0,lenth);
    addr.sin_family=AF_INET;
    addr.sin_port=htons(PORT);
    addr.sin_addr.s_addr=INADDR_ANY;
    if(0>bind(socketid,(struct sockaddr *)&addr,lenth))
    {
        printf("bind error");
        close(socketid);
        return -1;
    }
    //监听
    listen(socketid,5);
    //将socketid加入读文件描述集合中
    FD_ZERO(&readfds);
    FD_SET(socketid,&readfds);
    tmpfds=readfds;
    maxfd=socketid;
    while(1)
    {
        readfds=tmpfds;
        //printf("开始进入select模式，等待中..........\n");
        ret=select(maxfd+1,&readfds,NULL,NULL,NULL);
        if(ret<0)
        {
            perror("select error");
            break;
        }
        if(ret==0)
        {
            printf("等待超时");
            break;
        }
        int i;
        for(i=0;i<maxfd;i++)
        {
            if(i==socketid)
            {
                if(FD_ISSET(socketid,&readfds))
                {
                    newid=accept(socketid,(struct sockaddr*)&addr,&lenth);
                    if(newid<0)
                    {
                        perror("accept error");
                        break;
                    }
                    printf("accept ok newid is %d\n",newid);
                    FD_SET(newid,&tmpfds);//newid add readfds
                    if(maxfd < newid)
                    {
                        maxfd=newid;
                        printf("new client connect ok,port is %d\n",ntohs(addr.sin_port));
                    }
                }
            }
            else if (FD_ISSET(i,&readfds))//newid`s connect
            {
                ret=recv(i,buf,SIZE-1,0);
                if(ret > 0)
                {
                    printf("recv %d : %s\r\n",i,buf);
                }
                else if (0==ret)
                {
                    FD_CLR(i,&tmpfds);
                    close(i);
                }
                else
                {
                    perror("recv error");
                }
            }
        }
    }
    close(socketid);
    return 0;
}
