/****************************
*							*
*	Endpoint.cpp			*
*							*
*	Created : 2022/05/11	*
*	Updated : 2022/05/11	*
*****************************/

#include "Endpoint.h"

Endpoint::Endpoint()
{
	ZeroMemory(&ipv4EndPoint, sizeof(SocketAddrIn));
	ipv4EndPoint.family = AF_INET;
}

Endpoint::Endpoint(const char* address, WORD port)
{
	ZeroMemory(&ipv4EndPoint, sizeof(SocketAddrIn));
	ipv4EndPoint.family = AF_INET;

	// h to n = Host to network
	// 주소를 문자열로 받기 때문에 예제에서 사용된 htonl을 쓰지않고 inet_pton 함수를 사용

	// 값을 그냥 넣지 않고 뭔가 변환하는 함수를 사용하는 이유?
	// 네트워크에서 통신할 때는 Big Endian 방식을 사용하는데,
	// 사용자 환경이 Little Endian인지 Big Endian인지 알 수 없다.
	// 그래서 사용자 환경에 맞게 값을 변환 해주는 함수이다.
	inet_pton(AF_INET, address, &ipv4EndPoint.address);
	ipv4EndPoint.port = htons(port);
}

Endpoint::~Endpoint()
{

}