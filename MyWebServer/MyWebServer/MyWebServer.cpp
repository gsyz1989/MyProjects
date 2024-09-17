// MyWebServer.cpp : 定义控制台应用程序的入口点。
//
#include<stdio.h>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

#define CHARMAXLEN 1024

void sendHtml(SOCKET c,char*filename);
int main()
{
	printf("欢迎使用顽石老师web服务器....\n");
	WSADATA wsdata;
	int isok=WSAStartup(MAKEWORD(2,2),&wsdata);
	if(isok==WSAEINVAL)
	{
		perror("socket请求失败...\n");
	}
	SOCKET server=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(server==INVALID_SOCKET)
	{
		perror("socket创建失败...\n");
	}
	struct sockaddr_in seraddr;
	seraddr.sin_family=AF_INET;
	seraddr.sin_port=htons(80);
	seraddr.sin_addr.s_addr=INADDR_ANY;
	isok=bind(server,(sockaddr*)&seraddr,sizeof(seraddr));
	if(isok==SOCKET_ERROR)
	{
		perror("BIND失败...\n");
	}
	listen(server,5);
	int k=0;
	while(1)
	{
		printf("等待连接中...\n");
		SOCKET client=accept(server,NULL,NULL);
		if(client==INVALID_SOCKET)
		{
			perror("accept失败...\n");
		}
		char buf[CHARMAXLEN]={0};
		int res=recv(client,buf,sizeof(buf),0);
		printf("%s共接收到%d数据%d\n",buf,res,k);
		k++;

		//char sdata[CHARMAXLEN]="<h1 style=\"color:red;\">hello world<h1>";
		//send(client,sdata,strlen(sdata),0);

		sendHtml(client,"./百度一下，你就知道.html");
		closesocket(client);
	}
	closesocket(server);
	WSACleanup();
	//getchar();
	return 0;
}

void sendHtml(SOCKET c,char*filename)
{
	FILE*pfile=fopen(filename,"r");
	if(pfile==NULL)
	{
		perror("文件打开失败!\n");
		return;
	}

	do 
	{
		char sdata[CHARMAXLEN]={0};
		fgets(sdata,CHARMAXLEN,pfile);
		send(c,sdata,strlen(sdata),0);
	} while (!feof(pfile));
	
	fclose(pfile);
}