/****************************
*							*
*	GameProcess.h			*
*							*
*	Created : 2022/05/16	*
*	Updated : 2022/05/22	*
*****************************/

#include "stdafx.h"
#include <conio.h>

GameProcess GameProcess::gameInstance;

GameProcess::GameProcess()
	: player(nullptr)
	, remotePlayer(nullptr)
	, isLaunch(true)
{

}

GameProcess::~GameProcess()
{

}

GameProcess* GameProcess::GetInstance()
{
	return &gameInstance;
}

void GameProcess::Init()
{
	std::cout << "1 - Echo / 2 - Broadcast" << std::endl;

	player = new Player;
	player->Init();
}

void GameProcess::Update()
{
	if (_kbhit() != 0)
	{
		char input = _getch();

		// 에코 프로토콜 전송
		if (input == '1')
		{
			CLIENT->Send(Protocol::ECHO, "Echo Message", strlen("Echo Message"));
			std::cout << "Send Echo Protocol" << std::endl;
		}

		// 브로드캐스트 프로토콜 전송
		if (input == '2')
		{
			CLIENT->Send(Protocol::BROADCAST, "Broadcast Message", strlen("Broadcast Message"));
			std::cout << "Send Broadcast Protocol" << std::endl;
		}
	}

	player->Update();
}

void GameProcess::Render()
{

}

void GameProcess::Release()
{
	if (player != nullptr)
		delete player;

	if (remotePlayer != nullptr)
		delete remotePlayer;
}

void GameProcess::Run()
{
	Init();

	while (isLaunch)
	{
		Update();
		Render();
	}

	Release();
}

void GameProcess::Stop()
{
	isLaunch = false;
}

Player* GameProcess::GetPlayer()
{
	return player;
}

Player* GameProcess::GetOpponent()
{
	return remotePlayer;
}