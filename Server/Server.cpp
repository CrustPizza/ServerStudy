/****************************
*							*
*	Server.cpp				*
*							*
*	Created : 2022/05/15	*
*	Updated : 2022/05/16	*
*****************************/

#include "Server.h"
#include <conio.h>

#include <iostream>

Server::Server(Endpoint& target)
	: isLaunch(true)
{
	WSADATA w;

	if (WSAStartup(MAKEWORD(2, 2), &w) != 0)
		; // 예외 처리

	if (CreateListen(target) != true)
		; // 예외 처리
}

Server::Server(Endpoint&& target)
	: isLaunch(true)
{
	WSADATA w;

	if (WSAStartup(MAKEWORD(2, 2), &w) != 0)
		; // 예외 처리

	if (CreateListen(target) != true)
		; // 예외 처리
}

Server::~Server()
{
	Release();
	WSACleanup();
}

bool Server::CreateListen(Endpoint& target)
{
	TCPSocket* listenSocket = new TCPSocket;

	if (listenSocket->Open() != true)
		return false; // 예외 처리

	if (listenSocket->Bind(target) != true)
		return false; // 예외 처리

	WSAEVENT eventHandle = WSACreateEvent();

	WSAEventSelect(listenSocket->GetSocket(), eventHandle, FD_ACCEPT | FD_CLOSE);

	socketList.push_back(listenSocket);
	eventList.push_back(eventHandle);

	return true;
}

bool Server::CreateListen(Endpoint&& target)
{
	TCPSocket* listenSocket = new TCPSocket;

	if (listenSocket->Open() != true)
		return false; // 예외 처리

	if (listenSocket->Bind(target) != true)
		return false; // 예외 처리

	WSAEVENT eventHandle = WSACreateEvent();

	WSAEventSelect(listenSocket->GetSocket(), eventHandle, FD_ACCEPT | FD_CLOSE);

	socketList.push_back(listenSocket);
	eventList.push_back(eventHandle);

	return true;
}

bool Server::Listen()
{
	if (socketList[0]->Listen() != true)
		return false; // 예외 처리

	return true;
}

bool Server::Accept()
{
	TCPSocket* newClient = new TCPSocket;
	SOCKET clientSocket = socketList[0]->Accept(newClient->GetEndpoint());

	if (clientSocket == INVALID_SOCKET)
		return false; // 예외 처리

	WSAEVENT eventHandle = WSACreateEvent();

	newClient->Attachment(clientSocket);

	WSAEventSelect(newClient->GetSocket(), eventHandle, FD_READ | FD_WRITE | FD_CLOSE);

	socketList.push_back(newClient);
	eventList.push_back(eventHandle);

	return true;
}

void Server::Close(int index)
{
	if (index >= socketList.size())
		return;

	// 해제
	socketList[index]->Close();
	WSACloseEvent(eventList[index]);
	delete socketList[index];

	// 지워진 자리를 맨 뒤의 소켓을 옮겨 넣는다.
	socketList[index] = socketList.back();
	eventList[index] = eventList.back();
	socketList.pop_back();
	eventList.pop_back();
}

void Server::Release()
{
	// Socket Clear
	for (int i = 0; i < socketList.size(); i++)
	{
		if (socketList[i] != nullptr)
		{
			socketList[i]->Close();
			WSACloseEvent(eventList[i]);
			delete socketList[i];
		}
	}

	socketList.clear();
}

void Server::EventLoop()
{
	WSANETWORKEVENTS netEvent;

	if (Listen() != true)
		; // 예외 처리

	std::cout << "Listen.." << std::endl;

	while (isLaunch)
	{
		if (_kbhit() != 0)
		{
			char input = _getch();

			if (input == 'Q' || input == 'q')
				break;
		}

		int index = WSAWaitForMultipleEvents(eventList.size(), eventList.data(), false, 10, false);

		if (index == WSA_WAIT_TIMEOUT)
			continue;

		WSAEnumNetworkEvents(socketList[index]->GetSocket(), eventList[index], &netEvent);

		switch (netEvent.lNetworkEvents)
		{

		case FD_ACCEPT:
		{
			if (Accept() == true)
			{
				std::cout << "New Client!" << std::endl;
				std::cout << "Total Clients : " << GetClientCount() << std::endl;
			}

			break;
		}

		case FD_READ:
		{

			break;
		}

		case FD_WRITE:
		{

			break;
		}

		case FD_CLOSE:
		{
			Close(index);

			std::cout << "Client Leave" << std::endl;
			std::cout << "Total Clients : " << GetClientCount() << std::endl;

			break;
		}

		}
	}

	Release();
}

int Server::GetClientCount()
{
	return socketList.size() - 1;
}