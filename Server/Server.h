/****************************
*							*
*	Server.h				*
*							*
*	Created : 2022/05/15	*
*	Updated : 2022/05/16	*
*****************************/

#pragma once
#include "../Network/TCPSocket.h"
#include <vector>

class Server
{
	// 0¹øÀº Listen
	std::vector<TCPSocket*>	socketList;
	std::vector<WSAEVENT>	eventList;

	bool isLaunch;

public:
	Server(Endpoint& target);
	Server(Endpoint&& target);
	~Server();

	bool CreateListen(Endpoint& target);
	bool CreateListen(Endpoint&& target);
	bool Listen();
	bool Accept();
	void Close(int index);
	void Release();

	void EventLoop();

	int GetClientCount();
};