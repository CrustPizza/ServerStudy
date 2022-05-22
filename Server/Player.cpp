/****************************
*							*
*	Player.cpp				*
*							*
*	Created : 2022/05/22	*
*	Updated : 2022/05/22	*
*****************************/

#include "Player.h"
#include "ClientManager.h"
#include "../Network/Protocol.h"

Player::Player()
	: networkID(0)
	, state(PlayerState::NONE)
{

}

Player::~Player()
{

}

bool Player::Recv(ClientManager* clientManager)
{
	char buffer[1024] = {};

	if (clientManager->Recv(networkID, buffer, 1024) != true)
		return false;

	// 프로토콜 2Byte
	Protocol protocol = static_cast<Protocol>((static_cast<BYTE>(buffer[0]) << 8) + static_cast<BYTE>(buffer[1]));

	switch (protocol)
	{

	case Protocol::ECHO:
	{
		std::string echo = "Echo : ";
		echo += &buffer[2];
		clientManager->Send(networkID, Protocol::PRINT, echo.c_str(), echo.length());

		break;
	}
	
	case Protocol::BROADCAST:
	{
		std::string broadCast = "BroadCast : ";
		broadCast += &buffer[2];
		clientManager->Broadcast(Protocol::PRINT, broadCast.c_str(), broadCast.length());

		break;
	}

	default:
	{
		// 서버에서 처리되지 않는 프로토콜이 전송되는 경우

		break;
	}

	}

	return true;
}

bool Player::Send(ClientManager* clientManager)
{
	// 어떤 경우에 Send에 대한 처리를 하게될지?
	// TO DO

	return true;
}

bool Player::Close(ClientManager* clientManager)
{
	// 연결이 끊어졌을 때의 처리
	// TO DO

	clientManager->Close(networkID);

	return true;
}