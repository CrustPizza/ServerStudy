/****************************
*							*
*	TCP Socket.h			*
*							*
*	Created : 2022/05/09	*
*	Updated : 2022/05/10	*
*****************************/

#pragma once
#include "SocketInterface.h"

class TCPSocket : public SocketInterface
{

public:
	// Standard
	bool	Open() override;
	bool	Bind(SocketAddrIn& target) override;
	bool	connect(SocketAddrIn& target) override;

	void	Close() override;
	void	Shutdown(int option = SD_BOTH) override;

	// Input / Output
	int		Recv(char* buffer, int size, int& error);
	int		Send(char* buffer, int size, int& error);

	// Listen
	bool	Listen();
	SOCKET	Accept(SocketAddrIn& target);
};