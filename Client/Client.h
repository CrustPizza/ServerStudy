/****************************
*							*
*	Client.h				*
*							*
*	Created : 2022/05/15	*
*	Updated : 2022/05/16	*
*****************************/

#pragma once
#include "../Network/TCPSocket.h"

class Client
{
	static Client clientInstance;
	TCPSocket*	clientSocket;
	WSAEVENT	eventHandle;
	bool		isLaunch;

private:
	Client();
	~Client();

public:
	static Client* GetInstance();

	bool Connect(Endpoint& target);
	bool Connect(Endpoint&& target);
	void Disconnect();
	void EventLoop();

	bool Send(const char* data, UINT size);
	bool Recv(char* data, UINT size);
};