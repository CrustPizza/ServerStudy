/****************************
*							*
*	main.cpp				*
*							*
*	Created : 2022/05/15	*
*	Updated : 2022/05/22	*
*****************************/

#include "stdafx.h"
#include <string>
#include <thread>

#pragma comment(lib, "../Output/Network.lib")

int main(int argCount, const char* argVector[])
{
	// Path, IP, Port == 3
	if (argCount != 3)
		return -1;

	// Game Process
	std::thread* gameThread = new std::thread([]()
		{
			GAMEPROCESS->Run();
		});

	// IP / Port
	std::string IPAddress = argVector[1];

	WORD port = std::stoi(argVector[2]);

	PlayerManager playerManager;

	// Client Socket
	std::thread* clientThread = new std::thread([&IPAddress, &port, &playerManager]()
		{
			if (CLIENT->Connect(Endpoint(IPAddress.c_str(), port)) == true)
			{
				std::cout << "Connect Success" << std::endl;
				CLIENT->SetPlayerManager(&playerManager);

				while (true)
				{
					CLIENT->Update();
					playerManager.Update();
				}
			}
			else
			{
				std::cout << "Connect Fail" << std::endl;
			}
		});

	// Thread 대기
	clientThread->join();
	gameThread->join();

	// Thread 해제
	delete clientThread;
	delete gameThread;

	return 0;
}