/****************************
*							*
*	TCP Socket.cpp			*
*							*
*	Created : 2022/05/09	*
*	Updated : 2022/05/15	*
*****************************/

#include "TCPSocket.h"

#define CLIENT_MAX 10000

TCPSocket::TCPSocket()
{

}

TCPSocket::~TCPSocket()
{
    // ������ �Ҹ�ȴٸ� ������ �ݴ´�.
    Close();
}

bool TCPSocket::Open()
{
    // Overlapped I/O�� ������� ���� �����̶�
    // ����ȭ socket���� ����..�غ��� �ϴµ�
    // WSASocket�� ���� �ʰ� socket�� ���� �Ϳ� �ǹ̰� �������� �𸣰ڴ�.
    fileDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (IsValid() != true)
    {
        // ���� ó��
        // TO DO

        return false;
    }

    return true;
}

bool TCPSocket::Bind(Endpoint& target)
{
    // �������� ����� ���� Listen ������ ������ �� ���
    // ó���� Client ���忡�� ���� ��û�� ���� ���������� �˾ƾ��ϴϱ�
    // IP �ּҿ� Port ��ȣ�� �ִ� ������ �־�� �Ѵ�.
    int error = bind(fileDescriptor, reinterpret_cast<SOCKADDR*>(&target.ipv4EndPoint), sizeof(Endpoint::SocketAddrIn));
    if (error == SOCKET_ERROR)
    {
        // ���� ó��
        // TO DO

        return false;
    }

    localAddr = localAddr;

    return true;
}

bool TCPSocket::Connect(Endpoint& target)
{
    int error = connect(fileDescriptor, reinterpret_cast<SOCKADDR*>(&target.ipv4EndPoint), sizeof(Endpoint::SocketAddrIn));
    if (error == SOCKET_ERROR)
    {
        // ���� ó��
        // TO DO

        // Practice������ WSAEWOULDBLOCK�� �������� ���� ó�� �Ͽ��µ�
        // �񵿱� ������ ��� ���� ��û�� ������ ������� �ʱ� ������
        // ������ ���� �ƴ϶� ������ ���� ó���� ��
        // ������ ������ ����ȭ ������ ����ϴ� false�� ��ȯ�Ѵ�.

        return false;
    }

    return true;
}

void TCPSocket::Close()
{
    if (IsValid() == true)
    {
        closesocket(fileDescriptor);
        fileDescriptor = INVALID_SOCKET;
    }
}

void TCPSocket::Shutdown(int option)
{
    // ������ ���� ���� ����ϰ� �ڵ��� �������� �ʴ´�.
    // �׸��� closesocket �Լ��� ���ο��� shutdown�� ȣ��ȴٰ� �Ѵ�.
    // ��, ������ �����Ұ� �ƴ� �̻� ���� �� �Լ��� ���� ȣ���� ���� ���ٴ� ��?
    shutdown(fileDescriptor, option);
}

int TCPSocket::Recv(char* buffer, int size, int& error)
{
    return 0;
}

int TCPSocket::Send(char* buffer, int size, int& error)
{
    return 0;
}

bool TCPSocket::Listen()
{
    if (listen(fileDescriptor, CLIENT_MAX) == SOCKET_ERROR)
        return false;

    return true;
}

SOCKET TCPSocket::Accept(Endpoint& target)
{
    int addressLength = sizeof(target.ipv4EndPoint);

    return accept(fileDescriptor, reinterpret_cast<SOCKADDR*>(&target.ipv4EndPoint), &addressLength);
}