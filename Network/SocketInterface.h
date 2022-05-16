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

	// 이동 생성자를 일단 Detach에서 사용해보기 위해서 만듦
	SocketInterface(SocketInterface&& other) noexcept;
	SocketInterface& operator=(SocketInterface&& other) noexcept;

	// 복사 생성 금지
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