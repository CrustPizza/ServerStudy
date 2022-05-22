/****************************
*							*
*	Player.cpp				*
*							*
*	Created : 2022/05/16	*
*	Updated : 2022/05/22	*
*****************************/

#include "stdafx.h"
#include "../Network/Protocol.h"

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
}

void Player::Release()
{

}

bool Player::Recv()
{
	char buffer[1024] = {};

	if (CLIENT->Recv(buffer, 1024) != true)
		return false;

	// �������� 2Byte
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
		// Ŭ���̾�Ʈ���� ó������ �ʴ� ���������� ���۵Ǵ� ���

		break;
	}

	}

	return true;
}

bool Player::Send()
{
	// � ��쿡 Send�� ���� ó���� �ϰԵ���?
	// TO DO

	return true;
}

bool Player::Close()
{
	// ������ �������� ���� ó��
	// TO DO

	return true;
}