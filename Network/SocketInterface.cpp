/****************************
*							*
*	Socket Interface.cpp	*
*							*
*	Created : 2022/05/09	*
*	Updated : 2022/05/10	*
*****************************/

#include "SocketInterface.h"
#include <iostream>

SocketInterface::SocketInterface()
{
	hSocket = INVALID_SOCKET;
	ZeroMemory(&localAddr, sizeof(SOCKADDR_IN));
}

SocketInterface::~SocketInterface()
{
	// ������ �Ҹ�ȴٸ� ������ �ݴ´�.
	if (IsValid() == true)
		closesocket(hSocket);
}

SocketInterface::SocketInterface(SocketInterface&& other) noexcept
{
	*this = std::move(other);
}

SocketInterface& SocketInterface::operator=(SocketInterface&& other) noexcept
{
	// �̵�!
	hSocket = other.hSocket;
	localAddr = other.localAddr;

	// �̵��� �� �ʱ�ȭ
	other.hSocket = INVALID_SOCKET;
	ZeroMemory(&other.localAddr, sizeof(SOCKADDR_IN));
}

SOCKET SocketInterface::GetSocket()
{
	return hSocket;
}

bool SocketInterface::IsValid()
{
	return hSocket != INVALID_SOCKET;
}

void SocketInterface::Attachment(SOCKET socket)
{
	hSocket = socket;
}

SOCKET SocketInterface::Detachment()
{
	// ��� �̷��� �ص� �Ǵ��� �ڽ��� ����.
	// �̷��� �ּҵ� ���� �������µ�, ���� ���� �ּҸ� �ִ°� �ǹ̰� �ֳ� ������
	// ���ε带 �ٽ� �Ѵ�? �� �͵� ���ɼ��� ������ ���� �� ����..
	// �ƿ� �̵� �����ϴ� �߿� �ּҸ� �ʱ�ȭ���� �ʴ� �͵� ������ ������ ������ ���.
	return std::move(*this).GetSocket();
}