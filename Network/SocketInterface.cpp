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
	// 이동!
	fileDescriptor = other.fileDescriptor;
	localAddr = other.localAddr;

	// 이동된 값 초기화
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
	// 사실 이렇게 해도 되는지 자신이 없다.
	// 이러면 주소도 같이 없어지는데, 소켓 없이 주소만 있는게 의미가 있나 싶은데
	// 바인드를 다시 한다? 는 것도 가능성이 없지는 않은 것 같고..
	// 아예 이동 연산하는 중에 주소를 초기화하지 않는 것도 괜찮지 않을까 생각이 든다.
	return std::move(*this).GetSocket();
}