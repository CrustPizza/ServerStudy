/****************************
*							*
*	Player.h				*
*							*
*	Created : 2022/05/16	*
*	Updated : 2022/05/22	*
*****************************/

#pragma once

class Player
{
	enum class PlayerState
	{

	};

	ULONGLONG time;
	ULONGLONG prev;

public:
	Player();
	~Player();

	void Init();
	void Update();
	void Release();

	bool Recv();
	bool Send();
	bool Close();
};