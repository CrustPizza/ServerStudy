/****************************
*							*
*	Player Manager.cpp		*
*							*
*	Created : 2022/05/22	*
*	Updated : 2022/05/22	*
*****************************/

#include "stdafx.h"
#include "PlayerManager.h"

PlayerManager::PlayerManager()
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

		GAMEPROCESS->GetPlayer()->Recv();
	}

	// Send
	while (sendQueue.empty() != true)
	{
		UINT netID = sendQueue.front();
		sendQueue.pop();

		GAMEPROCESS->GetPlayer()->Send();
	}

	// Close
	while (closeQueue.empty() != true)
	{
		UINT netID = closeQueue.front();
		closeQueue.pop();

		GAMEPROCESS->GetPlayer()->Close();
	}
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