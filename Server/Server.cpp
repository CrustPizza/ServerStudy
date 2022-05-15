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
		; // ���� ó��

	listenSocket = new TCPSocket;
	
	if (listenSocket->Open() != true)
		; // ���� ó��

	if (listenSocket->Bind(target) != true)
		; // ���� ó��
}

Server::Server(Endpoint&& target)
{
	WSADATA w;

	if (WSAStartup(MAKEWORD(2, 2), &w) != 0)
		; // ���� ó��

	listenSocket = new TCPSocket;

	if (listenSocket->Open() != true)
		; // ���� ó��

	if (listenSocket->Bind(target) != true)
		; // ���� ó��
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
		return false; // ���� ó��

	ClientSocket* newClient = new ClientSocket;

	newClient->socket.Attachment(listenSocket->Accept(newClient->address));

	clientList.push_back(newClient);

	return true;
}