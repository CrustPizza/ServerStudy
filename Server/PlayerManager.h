/****************************
*							*
*	Player Manager.h		*
*							*
*	Created : 2022/05/22	*
*	Updated : 2022/05/22	*
*****************************/

#pragma once
#include <queue>

class ClientManager;

class PlayerManager
{
	enum class PlayerState
	{

	};

private:
	ClientManager* clientManager;

public:
	PlayerManager();
	~PlayerManager();

	void SetClientManager(ClientManager* clientManager);
};