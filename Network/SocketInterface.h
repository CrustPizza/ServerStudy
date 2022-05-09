/****************************
*							*
*	Socket Interface.h		*
*							*
*	Created : 2022/05/09	*
*	Updated : 2022/05/09	*
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
	enum class ProtocolType
	{
		NONE,
		TCP,
		UDP
	};

	// Ȥ�� ���� ���� ����ü/����ü�� �����Ͽ� ���
	// ���Ǵ� �ڵ带 ���� ������ (SOCKADDR*)�� ����ȯ�Ͽ� ����ϴ� �ɷ� ����
	// ���� ���� ����ص� ������ ���� ������ �����ȴ�.
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
	SocketAddrIn localAddr;

public:
	SocketInterface();
	~SocketInterface();

	// ���� ���� ����
	SocketInterface(const SocketInterface& other) = delete;
	SocketInterface& operator=(const SocketInterface& other) = delete;

	// Standard
	virtual bool	Open(ProtocolType type) abstract;
	virtual bool	Bind(SocketAddrIn& target) abstract;
	virtual bool	connect(SocketAddrIn& target) abstract;

	virtual void	Close() abstract;
	virtual void	Shutdown(int option = SD_BOTH) abstract;

	// Listen
	virtual	bool	Listen() abstract;
	virtual	SOCKET	Accept(SocketAddrIn& target) abstract;
};