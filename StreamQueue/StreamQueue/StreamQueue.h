/****************************
*	Stream Queue			*
*							*
*	Created : 2022/05/03	*
*	Updated : 2022/05/03	*
*****************************/

#pragma once

typedef unsigned int UINT;

template <UINT BufferSize = 1024>
class StreamQueue
{
	char buffer[BufferSize];

public:
	StreamQueue() = default;
	StreamQueue(const char* str, int length);
	~StreamQueue();
};

template<UINT BufferSize>
inline StreamQueue<BufferSize>::StreamQueue(const char* str, int length)
{

}

template<UINT BufferSize>
inline StreamQueue<BufferSize>::~StreamQueue()
{

}