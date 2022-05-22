/****************************
*							*
*	Client Manager.h		*
*							*
*	Created : 2022/05/22	*
*	Updated : 2022/05/22	*
*****************************/

#pragma once
#include "../Network/TCPSocket.h"
#include "../Network/Protocol.h"
#include <vector>
#include <queue>

class PlayerManager;

class ClientManager
{
	enum class ClientState
	{
		NONE,
		CONNECT,
		DISCONNECT,
	};

	struct Client
	{
		UINT networkID;
		TCPSocket* socket;
		ClientState state;

		Client() : networkID(0), socket(nullptr), state(ClientState::NONE) {}
	};

private:
	PlayerManager* playerManager;

	UINT currentNewID;
	std::queue<UINT> returnedID;

	std::vector<Client*>	clientList;
	std::vector<WSAEVENT>	eventList;

public:
	ClientManager();
	~ClientManager();

	void Update();
	void Release();

	bool Send(UINT networkID, Protocol protocol, const char* data, UINT size);
	bool Recv(UINT networkID, char* data, UINT size);
	bool Broadcast(Protocol protocol, const char* data, UINT size);
	void Close(UINT networkID);

	void SetPlayerManager(PlayerManager* playerManager);

	bool	AddNewClient(TCPSocket* socket);
	UINT	GetClientCount();

private:
	Client* FindClient(UINT networkID);
	int		FindClientIndex(UINT networkID);
};