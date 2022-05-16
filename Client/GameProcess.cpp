/****************************
*							*
*	GameProcess.h			*
*							*
*	Created : 2022/05/16	*
*	Updated : 2022/05/16	*
*****************************/

#include "stdafx.h"

GameProcess GameProcess::gameInstance;

GameProcess::GameProcess()
	: player(nullptr)
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
	player = new Player;
	player->Init();
}

void GameProcess::Update()
{
	player->Update();
}

void GameProcess::Render()
{

}

void GameProcess::Release()
{
	if (player != nullptr)
		delete player;
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