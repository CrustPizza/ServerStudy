/****************************
*							*
*	TCP Socket.h			*
*							*
*	Created : 2022/05/09	*
*	Updated : 2022/05/11	*
*****************************/

#include "TCPSocket.h"

TCPSocket::TCPSocket()
{

}

TCPSocket::~TCPSocket()
{

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

bool TCPSocket::Bind(SocketAddrIn& target)
{
    // �������� ����� ���� Listen ������ ������ �� ���
    // ó���� Client ���忡�� ���� ��û�� ���� ���������� �˾ƾ��ϴϱ�
    // IP �ּҿ� Port ��ȣ�� �ִ� ������ �־�� �Ѵ�.
    if (bind(fileDescriptor, reinterpret_cast<SOCKADDR*>(&target), sizeof(SocketAddrIn)) == SOCKET_ERROR)
    {
        // ���� ó��
        // TO DO

        return false;
    }

    localAddr = localAddr;

    return true;
}

bool TCPSocket::Connect(SocketAddrIn& target)
{
    if (connect(fileDescriptor, reinterpret_cast<SOCKADDR*>(&target), sizeof(SocketAddrIn)) == SOCKET_ERROR)
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

}

void TCPSocket::Shutdown(int option)
{

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
    return false;
}

SOCKET TCPSocket::Accept(SocketAddrIn& target)
{
    return SOCKET();
}
