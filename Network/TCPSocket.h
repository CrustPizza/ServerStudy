/****************************
*							*
*	TCP Socket.h			*
*							*
*	Created : 2022/05/09	*
*	Updated : 2022/05/16	*
*****************************/

#pragma once
#include "SocketInterface.h"
#include "../StreamQueue/StreamQueue.h"
#include <mutex>

#pragma comment(lib, "../Output/StreamQueue.lib")

using std::recursive_mutex;

class TCPSocket : public SocketInterface
{
	// Stream Queue와 Socket Buffer 사이의 징검다리 역할
	template<UINT BufferSize = 1024>
	struct TCPBuffer
	{
		WORD count;
		WORD done;
		WORD size;
		char* buffer;

		TCPBuffer() : count(0), done(0), size(BufferSize), buffer(nullptr)
		{
			buffer = new char[BufferSize];
		}
		~TCPBuffer()
		{
			if (buffer != nullptr)
				delete[] buffer;
		}
	};

private:
	// Send
	TCPBuffer<1024>		sendBuffer;
	StreamQueue<4096>*	sendQueue;
	recursive_mutex		sendMutex;

	// Receive
	TCPBuffer<1024>		recvBuffer;
	StreamQueue<4096>*	recvQueue;
	recursive_mutex		recvMutex;

public:
	TCPSocket();
	~TCPSocket() override;

	// Standard
	bool	Open() override;
	bool	Bind(Endpoint& target) override;
	bool	Connect(Endpoint& target) override;

	void	Close() override;
	void	Shutdown(int option = SD_BOTH) override;

	// Input / Output
	int		Send();
	int		Recv();
	bool	SendStream(const char* data, UINT size);
	bool	RecvStream(char* data, UINT size);

	// Listen
	bool	Listen();
	SOCKET	Accept(Endpoint& target);
};