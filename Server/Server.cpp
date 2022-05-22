/****************************
*							*
*	Server.cpp				*
*							*
*	Created : 2022/05/15	*
*	Updated : 2022/05/22	*
*****************************/

#include "Server.h"
#include <iostream>

Server::Server(Endpoint& target)
	: clientManager(nullptr)
	, listenSocket(nullptr)
	, listenEvent(nullptr)
{
	WSADATA w;

	if (WSAStartup(MAKEWORD(2, 2), &w) != 0)
		; // 抗寇 贸府

	if (CreateListen(target) != true)
		; // 抗寇 贸府
}

Server::Server(Endpoint&& target)
	: clientManager(nullptr)
	, listenSocket(nullptr)
	, listenEvent(nullptr)
{
	WSADATA w;

	if (WSAStartup(MAKEWORD(2, 2), &w) != 0)
		; // 抗寇 贸府

	if (CreateListen(target) != true)
		; // 抗寇 贸府
}

Server::~Server()
{
	Release();
	WSACleanup();
}

bool Server::CreateListen(Endpoint& target)
{
	listenSocket = new TCPSocket;

	if (listenSocket->Open() != true)
		return false; // 抗寇 贸府

	if (listenSocket->Bind(target) != true)
		return false; // 抗寇 贸府

	listenEvent = WSACreateEvent();

	WSAEventSelect(listenSocket->GetSocket(), listenEvent, FD_ACCEPT | FD_CLOSE);

	if (Listen() != true)
		; // 抗寇 贸府

	return true;
}

bool Server::CreateListen(Endpoint&& target)
{
	listenSocket = new TCPSocket;

	if (listenSocket->Open() != true)
		return false; // 抗寇 贸府

	if (listenSocket->Bind(target) != true)
		return false; // 抗寇 贸府

	listenEvent = WSACreateEvent();

	WSAEventSelect(listenSocket->GetSocket(), listenEvent, FD_ACCEPT | FD_CLOSE);

	if (Listen() != true)
		; // 抗寇 贸府

	return true;
}

bool Server::Listen()
{
	if (listenSocket->Listen() != true)
		return false; // 抗寇 贸府

	std::cout << "Listen.." << std::endl;

	return true;
}

bool Server::Accept()
{
	TCPSocket* newClient = new TCPSocket;
	SOCKET clientSocket = listenSocket->Accept(newClient->GetEndpoint());

	if (clientSocket == INVALID_SOCKET)
		return false; // 抗寇 贸府

	newClient->Attachment(clientSocket);

	if (clientManager->AddNewClient(newClient) != true)
		return false; // 抗寇 贸府

	return true;
}

void Server::Release()
{
	if (listenSocket != nullptr)
	{
		listenSocket->Close();
		WSACloseEvent(listenEvent);
		delete listenSocket;
	}
}

void Server::Update()
{
	WSANETWORKEVENTS netEvent;

	DWORD index = WaitForSingleObjectEx(listenEvent, 10, false);

	if (index == WSA_WAIT_TIMEOUT)
		return;

	WSAEnumNetworkEvents(listenSocket->GetSocket(), listenEvent, &netEvent);

	switch (netEvent.lNetworkEvents)
	{

	case FD_ACCEPT:
	{
		if (Accept() == true)
		{
			std::cout << "New Client!" << std::endl;
			std::cout << "Total Clients : " << clientManager->GetClientCount() << std::endl;
		}

		break;
	}

	}
}

void Server::SetClientManager(ClientManager* clientManager)
{
	this->clientManager = clientManager;
}