/****************************
*							*
*	main.cpp				*
*							*
*	Created : 2022/05/15	*
*	Updated : 2022/05/15	*
*****************************/

#include "Client.h"
#include <string>

#pragma comment(lib, "../Output/Network.lib")

#include <iostream>

int main(int argCount, const char* argVector[])
{
	// Path, IP, Port
	if (argCount != 3)
		return -1;

	std::string IPAddress = argVector[1];

	WORD port = std::stoi(argVector[2]);

	Client client;

	Sleep(3000);

	if (client.Connect(Endpoint(argVector[1], port)) != true)
		std::cout << "Connect Fail" << std::endl;
	else
		std::cout << "Connect Success" << std::endl;

	while (true)
		;

	return 0;
}