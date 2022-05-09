/****************************
*							*
*	Socket Interface.cpp	*
*							*
*	Created : 2022/05/09	*
*	Updated : 2022/05/09	*
*****************************/

#include "SocketInterface.h"

SocketInterface::SocketInterface()
{
	hSocket = INVALID_SOCKET;
	ZeroMemory(&localAddr, sizeof(SOCKADDR_IN));
}

SocketInterface::~SocketInterface()
{

}