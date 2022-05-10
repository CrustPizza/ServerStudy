/****************************
*							*
*	Socket Interface.h		*
*							*
*	Created : 2022/05/09	*
*	Updated : 2022/05/10	*
*****************************/

#pragma once
#include <WinSock2.h>

// Type Define
using WORD = unsigned short;
using BYTE = unsigned char;
using UINT = unsigned int;

/// <summary>
/// Socket Interface
/// </summary>
class SocketInterface
{
public:
	// 혹시 몰라서 직접 구조체/공용체를 선언하여 사용
	// 사용되는 코드를 보니 어차피 (SOCKADDR*)로 형변환하여 사용하는 걸로 보아
	// 직접 만들어서 사용해도 문제가 없을 것으로 생각된다.
	union InAddr
	{
		struct { BYTE byte1, byte2, byte3, byte4; };
		struct { BYTE network, host, logicalHost, impNumber; };
		struct { WORD highWord, lowWord; };
		UINT Address;
	};

	struct SocketAddrIn
	{
		WORD sin_family;
		WORD sin_port;
		InAddr sin_addr;
		BYTE sin_zero[8];
	};

private:
	SOCKET			hSocket;
	SocketAddrIn	localAddr;

public:
	SocketInterface();
	~SocketInterface();

	// 이동 생성자를 일단 Detach에서 사용해보기 위해서 만듦
	SocketInterface(SocketInterface&& other) noexcept;
	SocketInterface& operator=(SocketInterface&& other) noexcept;

	// 복사 생성 금지
	SocketInterface(const SocketInterface& other) = delete;
	SocketInterface& operator=(const SocketInterface& other) = delete;

	// Standard
	virtual bool	Open() abstract;
	virtual bool	Bind(SocketAddrIn& target) abstract;
	virtual bool	connect(SocketAddrIn& target) abstract;

	virtual void	Close() abstract;
	virtual void	Shutdown(int option = SD_BOTH) abstract;

	// Socket control
	SOCKET			GetSocket();
	bool			IsValid();

	void			Attachment(SOCKET socket);
	SOCKET			Detachment();
};