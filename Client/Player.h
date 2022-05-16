/****************************
*							*
*	Player.h				*
*							*
*	Created : 2022/05/16	*
*	Updated : 2022/05/16	*
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
};