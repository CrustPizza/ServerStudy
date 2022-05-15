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
    // 변수가 소멸된다면 소켓을 닫는다.
    Close();
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

bool TCPSocket::Bind(Endpoint& target)
{
    // 서버에서 통신을 위해 Listen 소켓을 생성할 때 사용
    // 처음에 Client 입장에서 연결 요청을 어디로 보내야할지 알아야하니까
    // IP 주소와 Port 번호가 있는 소켓이 있어야 한다.
    int error = bind(fileDescriptor, reinterpret_cast<SOCKADDR*>(&target.ipv4EndPoint), sizeof(Endpoint::SocketAddrIn));
    if (error == SOCKET_ERROR)
    {
        // 예외 처리
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
    if (IsValid() == true)
    {
        closesocket(fileDescriptor);
        fileDescriptor = INVALID_SOCKET;
    }
}

void TCPSocket::Shutdown(int option)
{
    // 연결을 끊기 위해 사용하고 핸들은 해제되지 않는다.
    // 그리고 closesocket 함수의 내부에서 shutdown이 호출된다고 한다.
    // 즉, 소켓을 재사용할게 아닌 이상 내가 이 함수를 따로 호출할 일은 없다는 뜻?
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