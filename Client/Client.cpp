/****************************
*							*
*	Client.cpp				*
*							*
*	Created : 2022/05/15	*
*	Updated : 2022/05/22	*
*****************************/

#include "stdafx.h"

Client Client::clientInstance;

Client::Client()
	: playerManager(nullptr)
	, eventHandle(nullptr)
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

void Client::Update()
{
	WSANETWORKEVENTS netEvent;
	DWORD index = WaitForSingleObjectEx(eventHandle, 10, false);

	if (index == WSA_WAIT_TIMEOUT)
		return;

	WSAEnumNetworkEvents(clientSocket->GetSocket(), eventHandle, &netEvent);

	switch (netEvent.lNetworkEvents)
	{

	case FD_READ:
	{
		clientSocket->Recv();
		playerManager->ReserveReceive(index);

		break;
	}

	case FD_WRITE:
	{
		clientSocket->Send();
		playerManager->ReserveSend(index);

		break;
	}

	case FD_CLOSE:
	{
		playerManager->ReserveClose(index);
		std::cout << "Disconnected" << std::endl;

		break;
	}

	}
}

bool Client::Send(Protocol protocol, const char* data, UINT size)
{
	if (size <= 0)
		return false; // 抗寇 贸府

	// 贸澜 2Byte Packet Size
	// 第狼 2Byte Protocol
	std::string str = "    ";

	for (int i = 0; i < 2; i++)
	{
		str[1 - i] = (size >> (i * 8)) & 0xff;
		str[3 - i] = (static_cast<UINT>(protocol) >> (i * 8)) & 0xff;
	}

	str += data;

	return clientSocket->SendStream(str.c_str(), size + 4);
}

bool Client::Recv(char* data, UINT size)
{
	if (size < 2)
		return false; // 抗寇 贸府

	// Packet Size 犬牢阑 困秦 2Byte 佬绢坷扁
	if (clientSocket->RecvStream(data, 2) != true)
		return false; // 抗寇 贸府

	// Packet Size 拌魂
	UINT packetSize = (data[0] << 8) + data[1] + 2;

	if (size < packetSize)
		return false; // 菩哦 荤捞令甫 促 罐酒啊瘤 给窃

	return clientSocket->RecvStream(data, packetSize);
}

void Client::SetPlayerManager(PlayerManager* playerManager)
{
	this->playerManager = playerManager;
}