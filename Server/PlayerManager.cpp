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

void PlayerManager::SetClientManager(ClientManager* clientManager)
{
	this->clientManager = clientManager;
}