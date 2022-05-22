/****************************
*							*
*	Player.cpp				*
*							*
*	Created : 2022/05/16	*
*	Updated : 2022/05/22	*
*****************************/

#include "stdafx.h"
#include "../Network/Protocol.h"
#include <conio.h>

Player::Player()
	: time(0)
	, prev(0)
{

}

Player::~Player()
{

}

void Player::Init()
{
	prev = GetTickCount64();
}

void Player::Update()
{
	//ULONGLONG curr = GetTickCount64();

	//time += curr - prev;
	//prev = curr;

	//if (time >= 1000)
	//{
	//	std::string message = "Hello";

	//	CLIENT->Send(Protocol::ECHO, message.c_str(), message.length());
	//	time -= 1000;
	//}

	if (_kbhit() != 0)
	{
		char input = _getch();

		if (input == '1')
		{
			CLIENT->Send(Protocol::ECHO, "EchoMessage", strlen("EchoMessage"));
			std::cout << "Send Echo Protocol" << std::endl;
		}

		if (input == '2')
		{
			CLIENT->Send(Protocol::BROADCAST, "BroadcastMessage", strlen("BroadcastMessage"));
			std::cout << "Send Broadcast Protocol" << std::endl;
		}
	}

	Recv();
}

void Player::Release()
{

}

bool Player::Recv()
{
	char buffer[1024] = {};

	if (CLIENT->Recv(buffer, 1024) != true)
		return false;

	// 프로토콜 2Byte
	Protocol protocol = static_cast<Protocol>((buffer[0] << 8) + buffer[1]);

	switch (protocol)
	{

	case Protocol::PRINT:
	{
		std::cout << &buffer[2] << std::endl;

		break;
	}

	default:
	{
		// 클라이언트에서 처리되지 않는 프로토콜이 전송되는 경우

		break;
	}

	}

	return true;
}

bool Player::Send()
{
	// 어떤 경우에 Send에 대한 처리를 하게될지?
	// TO DO

	return true;
}

bool Player::Close()
{
	// 연결이 끊어졌을 때의 처리
	// TO DO

	return true;
}