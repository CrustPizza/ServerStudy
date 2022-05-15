/****************************
*							*
*	Server.h				*
*							*
*	Created : 2022/05/15	*
*	Updated : 2022/05/15	*
*****************************/

#pragma once
#include "../Network/TCPSocket.h"
#include <list>

class Server
{
	struct ClientSocket
	{
		TCPSocket	socket;
		Endpoint	address;
	};

	TCPSocket* listenSocket;

	std::list<ClientSocket*> clientList;

public:
	Server(Endpoint& target);
	Server(Endpoint&& target);
	~Server();

	bool Listen();
};