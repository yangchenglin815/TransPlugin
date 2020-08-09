// netudp.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "netudp.h"

#pragma comment(lib,"ws2_32.lib")

static uint32_t st_client_sock = 0;
static sockaddr_in st_addr;


BOOL NetUdpClientInit(const char* server_ip,uint16_t port )
{

	st_client_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	st_addr.sin_family = AF_INET;
	st_addr.sin_port = htons(port);
	//INADDR_ANY;
	st_addr.sin_addr.s_addr = inet_addr(server_ip);
	//setsockopt(st_client_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char *)&st_addr, sizeof(st_addr));

	int ret = bind(st_client_sock,(SOCKADDR*)&st_addr,sizeof(sockaddr_in));
	if(ret)
	{
		DWORD err = GetLastError();
		if(err != 10049)
		{
			assert(0);
			printf("NetUdpClientInit err:%d\n",err);
			return 0;
		}
	}
	return 1;
}

BOOL NetUdpClientUninit()
{
	closesocket(st_client_sock);
	return 1;
}


int NetUdpClientSend(const char* buff,int len)
{
	//char recv_buff[100]={0};
	//int recv_len = recv(st_client_sock,recv_buff,sizeof(buff),0);
	int send_len = sendto(st_client_sock,buff,len,0,(SOCKADDR*)&st_addr,sizeof(SOCKADDR));
	return send_len;
}

