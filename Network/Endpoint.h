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
/// 예전에 Endpoint를 처음 들었을 때 뭔지 몰랐는데
/// 
/// 많이 들어봤었던 '단말'이었다.. 
/// </summary>
class Endpoint
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
		WORD	family;
		WORD	port;
		InAddr	address;
		BYTE	padding[8]; // 사용하지 않고 SOCKADDR과 바이트를 맞추기 위한 변수라고 함..
	};

public:
	SocketAddrIn	ipv4EndPoint; // 255.255.255.255 4Byte 주소 사용

public:
	Endpoint();
	Endpoint(const char* address, WORD port);
	~Endpoint();
};