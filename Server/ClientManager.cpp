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

		std::cout << "Receive Message" << std::endl;

		char buffer[1024] = {};
		clientList[index]->socket->RecvStream(buffer, recvSize);

		std::cout << "Echo : " << &buffer[2] << std::endl;

		clientList[index]->socket->SendStream(buffer, recvSize);

		break;
	}

	case FD_WRITE:
	{
		clientList[index]->socket->Send();

		std::cout << "Send Message" << std::endl;

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

void ClientManager::Close(int index)
{
	if (index >= clientList.size())
		return;

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
	eventList.push_back(eventHandle);

	return true;
}

UINT ClientManager::GetClientCount()
{
	return clientList.size();
}

void ClientManager::SetPlayerManager(PlayerManager* playerManager)
{
	this->playerManager = playerManager;
}