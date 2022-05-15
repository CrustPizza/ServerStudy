/****************************
*							*
*	Server.cpp				*
*							*
*	Created : 2022/05/15	*
*	Updated : 2022/05/15	*
*****************************/

#include "Server.h"

Server::Server(Endpoint& target)
{
	WSADATA w;

	if (WSAStartup(MAKEWORD(2, 2), &w) != 0)
		; // 抗寇 贸府

	listenSocket = new TCPSocket;
	
	if (listenSocket->Open() != true)
		; // 抗寇 贸府

	if (listenSocket->Bind(target) != true)
		; // 抗寇 贸府
}

Server::Server(Endpoint&& target)
{
	WSADATA w;

	if (WSAStartup(MAKEWORD(2, 2), &w) != 0)
		; // 抗寇 贸府

	listenSocket = new TCPSocket;

	if (listenSocket->Open() != true)
		; // 抗寇 贸府

	if (listenSocket->Bind(target) != true)
		; // 抗寇 贸府
}

Server::~Server()
{
	WSACleanup();

	// Listen Socket
	if (listenSocket != nullptr)
		delete listenSocket;

	// Client Socket
	for (auto iter : clientList)
	{
		if (iter != nullptr)
			delete iter;
	}
}

bool Server::Listen()
{
	if (listenSocket->Listen() != true)
		return false; // 抗寇 贸府

	ClientSocket* newClient = new ClientSocket;

	newClient->socket.Attachment(listenSocket->Accept(newClient->address));

	clientList.push_back(newClient);

	return true;
}