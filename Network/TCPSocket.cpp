/****************************
*							*
*	TCP Socket.cpp			*
*							*
*	Created : 2022/05/09	*
*	Updated : 2022/05/16	*
*****************************/

#include "TCPSocket.h"

#define CLIENT_MAX 10000

TCPSocket::TCPSocket()
    : sendQueue(nullptr)
    , recvQueue(nullptr)
{
    sendQueue = new StreamQueue<4096>;
    recvQueue = new StreamQueue<4096>;
}

TCPSocket::~TCPSocket()
{
    if (sendQueue != nullptr)
        delete sendQueue;

    if (recvQueue != nullptr)
        delete recvQueue;

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

    address = target;

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

    address = target;

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

int TCPSocket::Send()
{
	// SendBuffer의 남은 저장 공간
	int remainSize = sendBuffer.size - sendBuffer.count;

    int dataSize = 0;

	{
        std::lock_guard<recursive_mutex> lock(sendMutex);

		// Queue에서 보낼 데이터의 크기
		dataSize = sendQueue->GetDataSize() <= remainSize ? sendQueue->GetDataSize() : remainSize;

		// Buffer에 데이터 저장
		if (dataSize > 0)
		{
			sendQueue->Read(sendBuffer.buffer, dataSize);
			sendBuffer.count += dataSize;
		}
    }

	// Buffer에 저장된 데이터 크기 구하기
	dataSize = sendBuffer.count - sendBuffer.done;

	// SocketBuffer에 데이터를 보낸다.
	DWORD sendSize = send(fileDescriptor, &sendBuffer.buffer[sendBuffer.done], dataSize, 0);

	if (sendSize <= 0)
		return sendSize; // 예외 처리

	// 전송 크기를 비교해서 Buffer가 비워졌다면 초기화한다.
	if (sendBuffer.done + sendSize == sendBuffer.count)
	{
		sendBuffer.done = 0;
		sendBuffer.count = 0;
	}
	else
	{
		sendBuffer.done += sendSize;
	}

	return sendSize;
}

int TCPSocket::Recv()
{
    // RecvBuffer의 남은 저장 공간
    int remainSize = recvBuffer.size - recvBuffer.count;

    if (remainSize < 0)
        return remainSize; // 예외 처리

    // SocketBuffer에서 데이터를 읽어오고 읽은 크기를 반환 받는다.
    DWORD recvSize = recv(fileDescriptor, &recvBuffer.buffer[recvBuffer.count], remainSize, 0);

    if (recvSize <= 0)
        return recvSize; // 예외 처리
    
    // 반환 크기를 저장한다.
    recvBuffer.count += recvSize;

    // 저장된 데이터 크기를 구한다.
    int dataSize = recvBuffer.count - recvBuffer.done;

	{
        std::lock_guard<recursive_mutex> lock(recvMutex);

		// Recv Stream Queue의 남은 저장 공간을 확인해서 저장할 데이터 공간을 작은 수로 맞춘다.
		dataSize = recvQueue->GetRemainSize() < dataSize ? recvQueue->GetRemainSize() : dataSize;

		// Stream Queue에 저장
		recvQueue->Write(&recvBuffer.buffer[recvBuffer.done], dataSize);
    }

    //  저장 크기를 비교해서 Buffer가 비워졌다면 초기화한다.
    if (recvBuffer.done + dataSize == recvBuffer.count)
    {
        recvBuffer.done = 0;
        recvBuffer.count = 0;
    }
    else
    {
        recvBuffer.done += dataSize;
    }

    return recvSize;
}

bool TCPSocket::SendStream(const char* data, UINT size)
{
    std::lock_guard<recursive_mutex> lock(sendMutex);

    if (sendQueue->GetRemainSize() < size)
        return false; // 예외 처리

    sendQueue->Write(data, size);
    Send();

	return true;
}

bool TCPSocket::RecvStream(char* data, UINT size)
{
	std::lock_guard<recursive_mutex> lock(recvMutex);

	if (recvQueue->GetDataSize() < size)
		return false; // 예외 처리

	recvQueue->Read(data, size);

	return true;
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

    SOCKET newClient = accept(fileDescriptor, reinterpret_cast<SOCKADDR*>(&target.ipv4EndPoint), &addressLength);

    return newClient;
}