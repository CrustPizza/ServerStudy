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

    address = target;

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
    // ������ ���� ���� ����ϰ� �ڵ��� �������� �ʴ´�.
    // �׸��� closesocket �Լ��� ���ο��� shutdown�� ȣ��ȴٰ� �Ѵ�.
    // ��, ������ �����Ұ� �ƴ� �̻� ���� �� �Լ��� ���� ȣ���� ���� ���ٴ� ��?
    shutdown(fileDescriptor, option);
}

int TCPSocket::Send()
{
	// SendBuffer�� ���� ���� ����
	int remainSize = sendBuffer.size - sendBuffer.count;

    int dataSize = 0;

	{
        std::lock_guard<recursive_mutex> lock(sendMutex);

		// Queue���� ���� �������� ũ��
		dataSize = sendQueue->GetDataSize() <= remainSize ? sendQueue->GetDataSize() : remainSize;

		// Buffer�� ������ ����
		if (dataSize > 0)
		{
			sendQueue->Read(sendBuffer.buffer, dataSize);
			sendBuffer.count += dataSize;
		}
    }

	// Buffer�� ����� ������ ũ�� ���ϱ�
	dataSize = sendBuffer.count - sendBuffer.done;

	// SocketBuffer�� �����͸� ������.
	DWORD sendSize = send(fileDescriptor, &sendBuffer.buffer[sendBuffer.done], dataSize, 0);

	if (sendSize <= 0)
		return sendSize; // ���� ó��

	// ���� ũ�⸦ ���ؼ� Buffer�� ������ٸ� �ʱ�ȭ�Ѵ�.
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
    // RecvBuffer�� ���� ���� ����
    int remainSize = recvBuffer.size - recvBuffer.count;

    if (remainSize < 0)
        return remainSize; // ���� ó��

    // SocketBuffer���� �����͸� �о���� ���� ũ�⸦ ��ȯ �޴´�.
    DWORD recvSize = recv(fileDescriptor, &recvBuffer.buffer[recvBuffer.count], remainSize, 0);

    if (recvSize <= 0)
        return recvSize; // ���� ó��
    
    // ��ȯ ũ�⸦ �����Ѵ�.
    recvBuffer.count += recvSize;

    // ����� ������ ũ�⸦ ���Ѵ�.
    int dataSize = recvBuffer.count - recvBuffer.done;

	{
        std::lock_guard<recursive_mutex> lock(recvMutex);

		// Recv Stream Queue�� ���� ���� ������ Ȯ���ؼ� ������ ������ ������ ���� ���� �����.
		dataSize = recvQueue->GetRemainSize() < dataSize ? recvQueue->GetRemainSize() : dataSize;

		// Stream Queue�� ����
		recvQueue->Write(&recvBuffer.buffer[recvBuffer.done], dataSize);
    }

    //  ���� ũ�⸦ ���ؼ� Buffer�� ������ٸ� �ʱ�ȭ�Ѵ�.
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
        return false; // ���� ó��

    sendQueue->Write(data, size);
    Send();

	return true;
}

bool TCPSocket::RecvStream(char* data, UINT size)
{
	std::lock_guard<recursive_mutex> lock(recvMutex);

	if (recvQueue->GetDataSize() < size)
		return false; // ���� ó��

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