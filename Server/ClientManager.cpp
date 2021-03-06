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
		return false; // 抗寇 贸府

	Client* client = FindClient(networkID);

	if (client == nullptr)
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

	return client->socket->SendStream(str.c_str(), size + 4);
}

bool ClientManager::Recv(UINT networkID, char* data, UINT size)
{
	if (size < 2)
		return false; // 抗寇 贸府

	Client* client = FindClient(networkID);

	if (client == nullptr)
		return false; // 抗寇 贸府

	// 2Byte甫 佬绢辑 农扁甫 备茄促.
	if (client->socket->RecvStream(data, 2) != true)
		return false; // 抗寇 贸府

	// Packet Size 拌魂
	UINT packetSize = (data[0] << 8) + data[1] + 2;

	if (size < packetSize)
		return false; // 菩哦 荤捞令甫 促 罐酒啊瘤 给窃

	return client->socket->RecvStream(data, packetSize);
}

void ClientManager::Close(UINT networkID)
{
	int index = FindClientIndex(networkID);

	if (index == -1)
		return; // 抗寇 贸府

	// 秦力
	returnedID.push(clientList[index]->networkID);
	clientList[index]->socket->Close();
	WSACloseEvent(eventList[index]);
	delete clientList[index];

	// 瘤况柳 磊府甫 盖 第狼 家南阑 颗败 持绰促.
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

	// 馆券等 ID啊 乐绰瘤 犬牢窍绊 乐促搁 馆券等 ID何磐 荤侩
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