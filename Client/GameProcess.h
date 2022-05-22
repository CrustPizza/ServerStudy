/****************************
*							*
*	GameProcess.h			*
*							*
*	Created : 2022/05/16	*
*	Updated : 2022/05/22	*
*****************************/

#pragma once

class Player;

class GameProcess
{
	static GameProcess gameInstance;

	Player* player;
	Player* remotePlayer;

	bool isLaunch;

private:
	GameProcess();
	~GameProcess();

public:
	static GameProcess* GetInstance();

	void Init();
	void Update();
	void Render();
	void Release();

	void Run();
	void Stop();

	Player* GetPlayer();
	Player* GetOpponent();
};