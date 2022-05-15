/****************************
*							*
*	Endpoint.h				*
*							*
*	Created : 2022/05/11	*
*	Updated : 2022/05/11	*
*****************************/

#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32")

// Type Define
using WORD = unsigned short;
using BYTE = unsigned char;
using UINT = unsigned int;

/// <summary>
/// ������ Endpoint�� ó�� ����� �� ���� �����µ�
/// 
/// ���� ���þ��� '�ܸ�'�̾���.. 
/// </summary>
class Endpoint
{
public:
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
		WORD	family;
		WORD	port;
		InAddr	address;
		BYTE	padding[8]; // ������� �ʰ� SOCKADDR�� ����Ʈ�� ���߱� ���� ������� ��..
	};

public:
	SocketAddrIn	ipv4EndPoint; // 255.255.255.255 4Byte �ּ� ���

public:
	Endpoint();
	Endpoint(const char* address, WORD port);
	~Endpoint();
};