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
		; // ���� ó��

	clientSocket = new TCPSocket;

	if (clientSocket->Open() != true)
		; // ���� ó��
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