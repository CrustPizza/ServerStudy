/****************************
*							*
*	Player Manager.h		*
*							*
*	Created : 2022/05/22	*
*	Updated : 2022/05/22	*
*****************************/

#pragma once
#include <queue>
#include <vector>
#include "Player.h"

class ClientManager;

class PlayerManager
{
private:
	ClientManager* clientManager;

	std::vector<Player*> playerList;

	// 예약된 작업
	std::queue<UINT> recvQueue;
	std::queue<UINT> sendQueue;
	std::queue<UINT> closeQueue;

public:
	PlayerManager();
	~PlayerManager();

	void Update();

	void AddPlayer(UINT networkID);
	void SubPlayer(UINT networkID);

	void SetClientManager(ClientManager* clientManager);

	void ReserveReceive(UINT networkID);
	void ReserveSend(UINT networkID);
	void ReserveClose(UINT networkID);
};