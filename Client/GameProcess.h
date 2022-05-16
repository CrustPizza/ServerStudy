/****************************
*							*
*	GameProcess.h			*
*							*
*	Created : 2022/05/16	*
*	Updated : 2022/05/16	*
*****************************/

#pragma once

class Player;

class GameProcess
{
	Player* player;

	static GameProcess gameInstance;

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
};