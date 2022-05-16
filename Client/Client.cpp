/****************************
*							*
*	Client.cpp				*
*							*
*	Created : 2022/05/15	*
*	Updated : 2022/05/15	*
*****************************/

#include "Client.h"

Client::Client()
{
	WSADATA w;

	if (WSAStartup(MAKEWORD(2, 2), &w) != 0)
		; // 예외 처리

	clientSocket = new TCPSocket;

	if (clientSocket->Open() != true)
		; // 예외 처리
}

Client::~Client()
{
	WSACleanup();

	if (clientSocket != nullptr)
		Disconnect();
}

bool Client::Connect(Endpoint& target)
{
	return clientSocket->Connect(target);
}

bool Client::Connect(Endpoint&& target)
{
	return clientSocket->Connect(target);
}

void Client::Disconnect()
{
	clientSocket->Close();
	delete clientSocket;
	clientSocket = nullptr;
}