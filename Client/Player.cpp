/****************************
*							*
*	Player.cpp				*
*							*
*	Created : 2022/05/16	*
*	Updated : 2022/05/16	*
*****************************/

#include "stdafx.h"

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
	ULONGLONG curr = GetTickCount64();

	time += curr - prev;
	prev = curr;

	if (time >= 1000)
	{
		CLIENT->Send("Hello", strlen("Hello") + 1);
		time -= 1000;
	}

	char buffer[1024] = {};

	if (CLIENT->Recv(buffer, 1024) != false)
	{
		std::cout << buffer << std::endl;
	}
}

void Player::Release()
{

}