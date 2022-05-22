/****************************
*							*
*	Server.h				*
*							*
*	Created : 2022/05/15	*
*	Updated : 2022/05/22	*
*****************************/

#pragma once
#include "ClientManager.h"

class Server
{
	ClientManager* clientManager;

	TCPSocket*	listenSocket;
	WSAEVENT	listenEvent;

public:
	Server(Endpoint& target);
	Server(Endpoint&& target);
	~Server();

	bool CreateListen(Endpoint& target);
	bool CreateListen(Endpoint&& target);
	bool Listen();
	bool Accept();
	void Release();

	void Update();

	void SetClientManager(ClientManager* clientManager);
};