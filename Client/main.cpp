/****************************
*							*
*	main.cpp				*
*							*
*	Created : 2022/05/15	*
*	Updated : 2022/05/16	*
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

	// Client Socket
	std::thread* clientThread = new std::thread([&IPAddress, &port]()
		{
			if (CLIENT->Connect(Endpoint(IPAddress.c_str(), port)) == true)
			{
				std::cout << "Connect Success" << std::endl;

				CLIENT->EventLoop();
			}
			else
			{
				std::cout << "Connect Fail" << std::endl;
			}
		});

	// Thread ���
	clientThread->join();
	gameThread->join();

	// Thread ����
	delete clientThread;
	delete gameThread;

	return 0;
}