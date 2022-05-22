/****************************
*							*
*	Player Manager.cpp		*
*							*
*	Created : 2022/05/22	*
*	Updated : 2022/05/22	*
*****************************/

#include "PlayerManager.h"
#include "ClientManager.h"

PlayerManager::PlayerManager()
	: clientManager(nullptr)
{

}

PlayerManager::~PlayerManager()
{

}

void PlayerManager::Update()
{
	// Receive
	while (recvQueue.empty() != true)
	{
		UINT netID = recvQueue.front();
		recvQueue.pop();

		for (int i = 0; i < playerList.size(); i++)
		{
			if (playerList[i]->networkID == netID)
				playerList[i]->Recv(clientManager);
		}
	}

	// Send
	while (sendQueue.empty() != true)
	{
		UINT netID = sendQueue.front();
		sendQueue.pop();

		for (int i = 0; i < playerList.size(); i++)
		{
			if (playerList[i]->networkID == netID)
				playerList[i]->Send(clientManager);
		}
	}

	// Close
	while (closeQueue.empty() != true)
	{
		UINT netID = closeQueue.front();
		closeQueue.pop();

		for (int i = 0; i < playerList.size(); i++)
		{
			if (playerList[i]->networkID == netID)
				playerList[i]->Close(clientManager);
		}
	}
}

void PlayerManager::AddPlayer(UINT networkID)
{
	for (int i = 0; i < playerList.size(); i++)
	{
		if (playerList[i]->networkID == networkID)
			return; // 동일한 ID가 있으면 안되는데 있다면?
	}

	Player* newPlayer = new Player;

	newPlayer->networkID = networkID;

	playerList.push_back(newPlayer);
}

void PlayerManager::SubPlayer(UINT networkID)
{
	for (int i = 0; i < playerList.size(); i++)
	{
		// 맨 뒤의 원소를 지워질 위치에 덮어 씌운다.
		if (playerList[i]->networkID == networkID)
		{
			delete playerList[i];
			playerList[i] = playerList.back();
			playerList.pop_back();

			break;
		}
	}
}

void PlayerManager::SetClientManager(ClientManager* clientManager)
{
	this->clientManager = clientManager;
}

void PlayerManager::ReserveReceive(UINT networkID)
{
	recvQueue.push(networkID);
}

void PlayerManager::ReserveSend(UINT networkID)
{
	sendQueue.push(networkID);
}

void PlayerManager::ReserveClose(UINT networkID)
{
	closeQueue.push(networkID);
}