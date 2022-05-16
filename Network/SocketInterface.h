/****************************
*							*
*	Socket Interface.h		*
*							*
*	Created : 2022/05/09	*
*	Updated : 2022/05/16	*
*****************************/

#pragma once
#include "Endpoint.h"

/// <summary>
/// Socket Interface
/// </summary>
class SocketInterface
{
protected:
	SOCKET		fileDescriptor;
	Endpoint	address;

public:
	SocketInterface();
	virtual ~SocketInterface();

	// �̵� �����ڸ� �ϴ� Detach���� ����غ��� ���ؼ� ����
	SocketInterface(SocketInterface&& other) noexcept;
	SocketInterface& operator=(SocketInterface&& other) noexcept;

	// ���� ���� ����
	SocketInterface(const SocketInterface& other) = delete;
	SocketInterface& operator=(const SocketInterface& other) = delete;

	// Standard
	virtual bool	Open() abstract;
	virtual bool	Bind(Endpoint& target) abstract;
	virtual bool	Connect(Endpoint& target) abstract;

	virtual void	Close() abstract;
	virtual void	Shutdown(int option = SD_BOTH) abstract;

	// Socket control
	SOCKET			GetSocket();
	Endpoint&		GetEndpoint();
	bool			IsValid();

	void			Attachment(SOCKET socket);
	SOCKET			Detachment();
};