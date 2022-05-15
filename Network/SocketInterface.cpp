/****************************
*							*
*	Socket Interface.cpp	*
*							*
*	Created : 2022/05/09	*
*	Updated : 2022/05/11	*
*****************************/

#include "SocketInterface.h"
#include <iostream>

SocketInterface::SocketInterface()
{
	fileDescriptor = INVALID_SOCKET;
}

SocketInterface::~SocketInterface()
{

}

SocketInterface::SocketInterface(SocketInterface&& other) noexcept
{
	*this = std::move(other);
}

SocketInterface& SocketInterface::operator=(SocketInterface&& other) noexcept
{
	// �̵�!
	fileDescriptor = other.fileDescriptor;
	localAddr = other.localAddr;

	// �̵��� �� �ʱ�ȭ
	other.fileDescriptor = INVALID_SOCKET;
	ZeroMemory(&other.localAddr, sizeof(Endpoint));

	return *this;
}

SOCKET SocketInterface::GetSocket()
{
	return fileDescriptor;
}

bool SocketInterface::IsValid()
{
	return fileDescriptor != INVALID_SOCKET;
}

void SocketInterface::Attachment(SOCKET socket)
{
	fileDescriptor = socket;
}

SOCKET SocketInterface::Detachment()
{
	// ��� �̷��� �ص� �Ǵ��� �ڽ��� ����.
	// �̷��� �ּҵ� ���� �������µ�, ���� ���� �ּҸ� �ִ°� �ǹ̰� �ֳ� ������
	// ���ε带 �ٽ� �Ѵ�? �� �͵� ���ɼ��� ������ ���� �� ����..
	// �ƿ� �̵� �����ϴ� �߿� �ּҸ� �ʱ�ȭ���� �ʴ� �͵� ������ ������ ������ ���.
	return std::move(*this).GetSocket();
}