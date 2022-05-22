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

class PlayerManager
{
private:
	// 예약된 작업
	std::queue<UINT> recvQueue;
	std::queue<UINT> sendQueue;
	std::queue<UINT> closeQueue;

public:
	PlayerManager();
	~PlayerManager();

	void Update();

	void ReserveReceive(UINT networkID);
	void ReserveSend(UINT networkID);
	void ReserveClose(UINT networkID);
};