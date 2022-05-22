/****************************
*							*
*	Player.h				*
*							*
*	Created : 2022/05/22	*
*	Updated : 2022/05/22	*
*****************************/

#pragma once

class ClientManager;

using UINT = unsigned int;

class Player
{
	enum class PlayerState
	{
		NONE,
		WAITING,
		MATCHING,
		PLAYING,
	};

public:
	UINT networkID;
	PlayerState state;

public:
	Player();
	~Player();

	bool Recv(ClientManager* clientManager);
	bool Send(ClientManager* clientManager);
	bool Close(ClientManager* clientManager);
};