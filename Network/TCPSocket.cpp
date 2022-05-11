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
    // Overlapped I/O를 사용하지 않을 예정이라서
    // 동기화 socket으로 생성..해보긴 하는데
    // WSASocket을 쓰지 않고 socket을 쓰는 것에 의미가 있을지를 모르겠다.
    fileDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (IsValid() != true)
    {
        // 예외 처리
        // TO DO

        return false;
    }

    return true;
}

bool TCPSocket::Bind(SocketAddrIn& target)
{
    // 서버에서 통신을 위해 Listen 소켓을 생성할 때 사용
    // 처음에 Client 입장에서 연결 요청을 어디로 보내야할지 알아야하니까
    // IP 주소와 Port 번호가 있는 소켓이 있어야 한다.
    if (bind(fileDescriptor, reinterpret_cast<SOCKADDR*>(&target), sizeof(SocketAddrIn)) == SOCKET_ERROR)
    {
        // 예외 처리
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
        // 예외 처리
        // TO DO

        // Practice에서는 WSAEWOULDBLOCK을 에러에서 예외 처리 하였는데
        // 비동기 소켓일 경우 연결 요청을 보내고 대기하지 않기 때문에
        // 실패한 것이 아니라서 에러를 예외 처리한 것
        // 하지만 지금은 동기화 소켓을 사용하니 false를 반환한다.

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
