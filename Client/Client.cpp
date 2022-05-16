/****************************
*							*
*	Client.cpp				*
*							*
*	Created : 2022/05/15	*
*	Updated : 2022/05/16	*
*****************************/

#include "stdafx.h"

Client Client::clientInstance;

Client::Client()
	: eventHandle(nullptr)
	, isLaunch(true)
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
	if (clientSocket != nullptr)
		Disconnect();

	WSACleanup();
}

Client* Client::GetInstance()
{
	return &clientInstance;
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

		WSAEnumNetworkEvents(clientSocket->GetSocket(), eventHandle, &netEvent);

		switch (netEvent.lNetworkEvents)
		{

		case FD_READ:
		{
			clientSocket->Recv();

			break;
		}

		case FD_WRITE:
		{
			clientSocket->Send();

			break;
		}

		case FD_CLOSE:
		{
			std::cout << "Disconnected" << std::endl;
			isLaunch = false;
			GAMEPROCESS->Stop();

			break;
		}

		}
	}
}

bool Client::Send(const char* data, UINT size)
{
	if (size <= 0)
		return false; // 예외 처리

	// 2Byte를 Packet Size에 할당
	std::string str = "  ";

	for (int i = 0; i < 2; i++)
		str[1 - i] = (size >> (i * 8)) & 0xff;

	str += data;

	return clientSocket->SendStream(str.c_str(), size + 2);
}

bool Client::Recv(char* data, UINT size)
{
	if (size < 2)
		return false; // 예외 처리

	// Packet Size 확인을 위해 2Byte 읽어오기
	if (clientSocket->RecvStream(data, 2) != true)
		return false; // 예외 처리

	// Packet Size 계산
	UINT packetSize = (data[0] << 8) + data[1];

	return clientSocket->RecvStream(data, packetSize);
}