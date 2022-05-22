/****************************
*							*
*	Client Manager.cpp		*
*							*
*	Created : 2022/05/22	*
*	Updated : 2022/05/22	*
*****************************/

#include "ClientManager.h"
#include "PlayerManager.h"
#include <iostream>

#define InitNetworkID 10001

ClientManager::ClientManager()
	: playerManager(nullptr)
	, currentNewID(InitNetworkID)
{

}

ClientManager::~ClientManager()
{
	Release();
}

void ClientManager::Update()
{
	if (GetClientCount() == 0)
		return;

	WSANETWORKEVENTS netEvent;

	DWORD index = WSAWaitForMultipleEvents(eventList.size(), eventList.data(), false, 10, false);

	if (index == WSA_WAIT_TIMEOUT)
		return;

	WSAEnumNetworkEvents(clientList[index]->socket->GetSocket(), eventList[index], &netEvent);

	switch (netEvent.lNetworkEvents)
	{

	case FD_READ:
	{
		int recvSize = clientList[index]->socket->Recv();
		playerManager->ReserveReceive(clientList[index]->networkID);

		std::cout << "Receive Message" << std::endl;

		break;
	}

	case FD_WRITE:
	{
		clientList[index]->socket->Send();
		playerManager->ReserveSend(clientList[index]->networkID);

		std::cout << "Send Message" << std::endl;

		break;
	}

	case FD_CLOSE:
	{
		playerManager->ReserveClose(clientList[index]->networkID);

		break;
	}

	}
}

void ClientManager::Release()
{
	// Socket Clear
	for (int i = clientList.size() - 1; i >= 0; i--)
	{
		if (clientList[i] != nullptr)
		{
			clientList[i]->socket->Close();
			WSACloseEvent(eventList[i]);
			delete clientList[i];
		}
	}

	currentNewID = InitNetworkID;
	clientList.clear();
	eventList.clear();
}

bool ClientManager::Send(UINT networkID, Protocol protocol, const char* data, UINT size)
{
	if (size <= 0)
		return false; // 예외 처리

	Client* client = FindClient(networkID);

	if (client == nullptr)
		return false; // 예외 처리

	// 처음 2Byte Packet Size
	// 뒤의 2Byte Protocol
	std::string str = "    ";

	for (int i = 0; i < 2; i++)
	{
		str[1 - i] = (size >> (i * 8)) & 0xff;
		str[3 - i] = (static_cast<UINT>(protocol) >> (i * 8)) & 0xff;
	}

	str += data;

	return client->socket->SendStream(str.c_str(), size + 4);
}

bool ClientManager::Recv(UINT networkID, char* data, UINT size)
{
	if (size < 2)
		return false; // 예외 처리

	Client* client = FindClient(networkID);

	if (client == nullptr)
		return false; // 예외 처리

	// 2Byte를 읽어서 크기를 구한다.
	if (client->socket->RecvStream(data, 2) != true)
		return false; // 예외 처리

	// Packet Size 계산
	UINT packetSize = (data[0] << 8) + data[1] + 2;

	if (size < packetSize)
		return false; // 패킷 사이즈를 다 받아가지 못함

	return client->socket->RecvStream(data, packetSize);
}

void ClientManager::Close(UINT networkID)
{
	int index = FindClientIndex(networkID);

	if (index == -1)
		return; // 예외 처리

	// 해제
	returnedID.push(clientList[index]->networkID);
	clientList[index]->socket->Close();
	WSACloseEvent(eventList[index]);
	delete clientList[index];

	// 지워진 자리를 맨 뒤의 소켓을 옮겨 넣는다.
	clientList[index] = clientList.back();
	eventList[index] = eventList.back();
	clientList.pop_back();
	eventList.pop_back();

	std::cout << "Client Leave" << std::endl;
	std::cout << "Total Clients : " << GetClientCount() << std::endl;
}

bool ClientManager::Broadcast(Protocol protocol, const char* data, UINT size)
{
	for (int i = 0; i < clientList.size(); i++)
		Send(clientList[i]->networkID, protocol, data, size);

	return true;
}

void ClientManager::SetPlayerManager(PlayerManager* playerManager)
{
	this->playerManager = playerManager;
}

bool ClientManager::AddNewClient(TCPSocket* socket)
{
	Client* newClient = new Client;

	// 반환된 ID가 있는지 확인하고 있다면 반환된 ID부터 사용
	if (returnedID.empty() == true)
	{
		newClient->networkID = currentNewID++;
	}
	else
	{
		newClient->networkID = returnedID.front();
		returnedID.pop();
	}

	newClient->socket = socket;
	newClient->state = ClientState::CONNECT;

	WSAEVENT eventHandle = WSACreateEvent();

	WSAEventSelect(newClient->socket->GetSocket(), eventHandle, FD_READ | FD_WRITE | FD_CLOSE);

	clientList.push_back(newClient);
	playerManager->AddPlayer(newClient->networkID);
	eventList.push_back(eventHandle);

	return true;
}

UINT ClientManager::GetClientCount()
{
	return clientList.size();
}

ClientManager::Client* ClientManager::FindClient(UINT networkID)
{
	for (int i = 0; i < clientList.size(); i++)
	{
		if (clientList[i]->networkID == networkID)
			return clientList[i];
	}

	return nullptr;
}

int ClientManager::FindClientIndex(UINT networkID)
{
	for (int i = 0; i < clientList.size(); i++)
	{
		if (clientList[i]->networkID == networkID)
			return i;
	}

	return -1;
}