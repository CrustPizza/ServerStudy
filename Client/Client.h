/****************************
*							*
*	Client.h				*
*							*
*	Created : 2022/05/15	*
*	Updated : 2022/05/15	*
*****************************/

#pragma once
#include "../Network/TCPSocket.h"

class Client
{
	TCPSocket* clientSocket;

public:
	Client();
	~Client();

	bool Connect(Endpoint& target);
	bool Connect(Endpoint&& target);
	void Disconnect();
};