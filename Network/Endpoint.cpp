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
	// �ּҸ� ���ڿ��� �ޱ� ������ �������� ���� htonl�� �����ʰ� inet_pton �Լ��� ���

	// ���� �׳� ���� �ʰ� ���� ��ȯ�ϴ� �Լ��� ����ϴ� ����?
	// ��Ʈ��ũ���� ����� ���� Big Endian ����� ����ϴµ�,
	// ����� ȯ���� Little Endian���� Big Endian���� �� �� ����.
	// �׷��� ����� ȯ�濡 �°� ���� ��ȯ ���ִ� �Լ��̴�.
	inet_pton(AF_INET, address, &ipv4EndPoint.address);
	ipv4EndPoint.port = htons(port);
}

Endpoint::~Endpoint()
{

}