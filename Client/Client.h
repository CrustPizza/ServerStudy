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
	TCPSocket*	clientSocket;
	WSAEVENT	eventHandle;
	bool		isLaunch;

public:
	Client();
	~Client();

	bool Connect(Endpoint& target);
	bool Connect(Endpoint&& target);
	void Disconnect();
	void EventLoop();
};