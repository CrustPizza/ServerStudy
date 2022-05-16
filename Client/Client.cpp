/****************************
*							*
*	Client.cpp				*
*							*
*	Created : 2022/05/15	*
*	Updated : 2022/05/16	*
*****************************/

#include "Client.h"

#include <iostream>

Client::Client()
	: eventHandle(nullptr)
	, isLaunch(true)
{
	WSADATA w;

	if (WSAStartup(MAKEWORD(2, 2), &w) != 0)
		; // 抗寇 贸府

	clientSocket = new TCPSocket;

	if (clientSocket->Open() != true)
		; // 抗寇 贸府
}

Client::~Client()
{
	if (clientSocket != nullptr)
		Disconnect();

	WSACleanup();
}

bool Client::Connect(Endpoint& target)
{
	if (clientSocket->Connect(target) != true)
		return false;

	eventHandle = WSACreateEvent();

	WSAEventSelect(clientSocket->GetSocket(), eventHandle, FD_WRITE | FD_READ | FD_CLOSE);

	return true;
}

bool Client::Connect(Endpoint&& target)
{
	if (clientSocket->Connect(target) != true)
		return false;

	eventHandle = WSACreateEvent();

	WSAEventSelect(clientSocket->GetSocket(), eventHandle, FD_WRITE | FD_READ | FD_CLOSE);

	return true;
}

void Client::Disconnect()
{
	clientSocket->Close();
	WSACloseEvent(eventHandle);
	delete clientSocket;
	clientSocket = nullptr;
}

void Client::EventLoop()
{
	WSANETWORKEVENTS netEvent;

	while (isLaunch)
	{
		int index = WSAWaitForMultipleEvents(1, &eventHandle, false, 10, false);

		if (index == WSA_WAIT_TIMEOUT)
			continue;

		WSAEnumNetworkEvents(clientSocket->GetSocket(), &eventHandle, &netEvent);

		switch (netEvent.lNetworkEvents)
		{

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
			Disconnect();
			isLaunch = false;

			std::cout << "Disconnected" << std::endl;

			break;
		}

		}
	}
}