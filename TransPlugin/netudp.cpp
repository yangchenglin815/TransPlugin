// netudp.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "netudp.h"
#include "logging.h"

#pragma comment(lib,"ws2_32.lib")

static SOCKET st_client_sock = 0;
static sockaddr_in st_addr;
static sockaddr_in st_addr_des;


BOOL NetUdpClientInit(const char* server_ip,uint16_t port )
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	// 启动socket api
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		Log::Info("WSAStartup failed.");
		return false;
	}

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		Log::Info("WSACleanup failed.");
		return false;
	}

	st_client_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == st_client_sock)
	{
		err = WSAGetLastError();
		Log::Info("Create socket failed.");
		return false;
	}
	bool bOpt = true;
	setsockopt(st_client_sock, SOL_SOCKET, SO_BROADCAST, (char *)&bOpt, sizeof(bOpt));
	memset(&st_addr, 0, sizeof(struct sockaddr_in));
	st_addr.sin_family = AF_INET;
	//INADDR_ANY;
	st_addr.sin_addr.s_addr = 0;
	st_addr.sin_port = htons(port);

	memset(&st_addr_des, 0, sizeof(struct sockaddr_in));
	st_addr_des.sin_family = AF_INET;
	//INADDR_ANY;
	st_addr_des.sin_addr.s_addr = INADDR_BROADCAST;
	st_addr_des.sin_port = htons(port);

	err = bind(st_client_sock, (SOCKADDR*)&st_addr, sizeof(st_addr));
	if (SOCKET_ERROR == err)
	{
		err = WSAGetLastError();
		Log::Info("Bind failed.");
		return false;
	}
	return 1;
}

BOOL NetUdpClientUninit()
{
	closesocket(st_client_sock);
	Log::Info("Close socket.");
	return 1;
}


int NetUdpClientSend(const char* buff,int len)
{
	int send_len = sendto(st_client_sock, buff, len, 0, (SOCKADDR*)&st_addr_des, sizeof(st_addr_des));
	if (send_len < 0)
	{
		DWORD err = GetLastError();
		Log::Info("Send data failed.");
	}
	return send_len;
}

