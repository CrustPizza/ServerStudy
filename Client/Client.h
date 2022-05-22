/****************************
*							*
*	Client.h				*
*							*
*	Created : 2022/05/15	*
*	Updated : 2022/05/22	*
*****************************/

#pragma once
#include "../Network/TCPSocket.h"
#include "../Network/Protocol.h"

class PlayerManager;

class Client
{
	PlayerManager* playerManager;

	static Client clientInstance;

	TCPSocket*	clientSocket;
	WSAEVENT	eventHandle;

private:
	Client();
	~Client();

public:
	static Client* GetInstance();

	bool Connect(Endpoint& target);
	bool Connect(Endpoint&& target);
	void Disconnect();
	void Update();

	bool Send(Protocol protocol, const char* data, UINT size);
	bool Recv(char* data, UINT size);

	void SetPlayerManager(PlayerManager* playerManager);
};