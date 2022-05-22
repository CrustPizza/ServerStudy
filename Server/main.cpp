/****************************
*							*
*	main.cpp				*
*							*
*	Created : 2022/05/15	*
*	Updated : 2022/05/16	*
*****************************/

#include "Server.h"
#include "ClientManager.h"
#include "PlayerManager.h"
#include <string>

#pragma comment(lib, "../Output/Network.lib")

int main(int argCount, char* argVector[])
{
	// Path, IP, Port
	if (argCount != 3)
		return -1;

	std::string IPAddress = argVector[1];

	WORD port = std::stoi(argVector[2]);

	Server server(Endpoint(IPAddress.c_str(), port));
	ClientManager clientManager;
	PlayerManager playerManager;
	server.SetClientManager(&clientManager);
	clientManager.SetPlayerManager(&playerManager);
	playerManager.SetClientManager(&clientManager);

	while (true)
	{
		server.Update();
		clientManager.Update();
	}

	return 0;
}