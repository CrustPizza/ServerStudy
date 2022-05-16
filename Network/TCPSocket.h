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

#pragma comment(lib, "../Output/StreamQueue.lib")

class TCPSocket : public SocketInterface
{
	template<UINT bufferSize = 1024>
	struct TCPBuffer
	{
		WORD count;
		WORD done;
		WORD size;
		char* buffer;

		TCPBuffer() : count(0), done(0), size(bufferSize), buffer(nullptr)
		{
			buffer = new char[bufferSize];
		}
		~TCPBuffer()
		{
			if (buffer != nullptr)
				delete[] buffer;
		}
	};

private:
	TCPBuffer<1024> sendBuffer;
	TCPBuffer<1024> recvBuffer;

	StreamQueue<4096>* sendQueue;
	StreamQueue<4096>* recvQueue;

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
	int		Recv();
	int		Send();

	// Listen
	bool	Listen();
	SOCKET	Accept(Endpoint& target);
};