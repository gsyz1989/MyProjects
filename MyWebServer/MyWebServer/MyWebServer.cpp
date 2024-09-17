// MyWebServer.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include<stdio.h>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

#define CHARMAXLEN 1024

void sendHtml(SOCKET c,char*filename);
int main()
{
	printf("��ӭʹ����ʯ��ʦweb������....\n");
	WSADATA wsdata;
	int isok=WSAStartup(MAKEWORD(2,2),&wsdata);
	if(isok==WSAEINVAL)
	{
		perror("socket����ʧ��...\n");
	}
	SOCKET server=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(server==INVALID_SOCKET)
	{
		perror("socket����ʧ��...\n");
	}
	struct sockaddr_in seraddr;
	seraddr.sin_family=AF_INET;
	seraddr.sin_port=htons(80);
	seraddr.sin_addr.s_addr=INADDR_ANY;
	isok=bind(server,(sockaddr*)&seraddr,sizeof(seraddr));
	if(isok==SOCKET_ERROR)
	{
		perror("BINDʧ��...\n");
	}
	listen(server,5);
	int k=0;
	while(1)
	{
		printf("�ȴ�������...\n");
		SOCKET client=accept(server,NULL,NULL);
		if(client==INVALID_SOCKET)
		{
			perror("acceptʧ��...\n");
		}
		char buf[CHARMAXLEN]={0};
		int res=recv(client,buf,sizeof(buf),0);
		printf("%s�����յ�%d����%d\n",buf,res,k);
		k++;

		//char sdata[CHARMAXLEN]="<h1 style=\"color:red;\">hello world<h1>";
		//send(client,sdata,strlen(sdata),0);

		sendHtml(client,"./�ٶ�һ�£����֪��.html");
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
		perror("�ļ���ʧ��!\n");
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