#pragma once
BOOL NetUdpClientInit(const char* server_ip,uint16_t port);

BOOL NetUdpClientUninit();

int NetUdpClientSend(const char* buff,int len);
