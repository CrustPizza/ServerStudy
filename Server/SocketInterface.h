/****************************
*							*
*	Socket Interface.h		*
*							*
*	Created : 2022/05/09	*
*	Updated : 2022/05/09	*
*****************************/

#pragma once
#include <WinSock2.h>

class SocketInterface
{
	SOCKET hSocket;
	SOCKADDR_IN localAddr;

protected:
	SocketInterface();
	~SocketInterface();
};