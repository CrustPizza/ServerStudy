/****************************
*	Stream Queue			*
*							*
*	Created : 2022/05/03	*
*	Updated : 2022/05/05	*
*****************************/

#pragma once
#include <assert.h>

// Type Define
using UINT = unsigned int;
using BYTE = unsigned char;

template <UINT BufferSize = 1024>
class StreamQueue
{
	// 버퍼를 동적으로 할 것인지 정적으로 할 것인지 고민
	// 데이터는 바이너리 파일 만들듯이 bit 상태로 저장
	// 일단 스트림이니까 배열은 맞겠지
	BYTE buffer[BufferSize];

	UINT headIndex;
	UINT tailIndex;

public:
	StreamQueue();
	~StreamQueue();

	// 일단 받아서 자료형의 크기를 계산하고
	// 자료형의 크기만큼 버퍼에 저장
	template <typename T>
	bool Write(T& data);

	// Size만큼 값을 읽어서 T로 변환 후 반환
	template <typename T>
	T Read(UINT size);

	// 문자열로 전부 다 읽어온다.
	std::string ReadAll();

	// Write와 동일한 동작을 한다.
	template <typename T>
	StreamQueue& operator <<(T& data);

	// Read와 동일한 동작을 한다.
	// 다만 오른쪽 인수로 들어오는 자료형의 크기에 맞게 반환한다.
	template <typename T>
	StreamQueue& operator >>(T& data);

	// 스트림의 상태를 확인하기 위한 함수들
	UINT GetDataSize();
	bool Empty();

private:
	// Head의 위치를 0으로 바꾸는 함수인데
	// 이름이 Push도 이상하고 Sort도 이상하고..
	void Rebuild();
};

template<UINT BufferSize>
inline StreamQueue<BufferSize>::StreamQueue()
	: headIndex(0)
	, tailIndex(0)
	, buffer{}
{

}

template<UINT BufferSize>
inline StreamQueue<BufferSize>::~StreamQueue()
{

}

template<UINT BufferSize>
template<typename T>
inline bool StreamQueue<BufferSize>::Write(T& data)
{
	UINT typeSize = sizeof(T);

	// 버퍼에 저장이 불가능한 경우
	if (typeSize > BufferSize - GetDataSize())
	{
		// 자료형을 통째로 넣는 것이기 때문에 분할해서 넣는 것은 힘들 것 같다.
		// throw를 할지, assert를 할지, false를 할지 고민
		return false;
	}

	// 저장은 가능하지만 버퍼를 정리할 필요가 있는 경우
	if (typeSize > BufferSize - tailIndex)
	{
		// 환형 큐를 써도 되지만 직관적인 구조를 위해
		// Head의 값이 Tail보다 크지 않은 상태를 유지한다.
		Rebuild();
	}

	// reinterpret_cast를 통해서 비트 상태 그대로 저장 한다.
	BYTE* writer = reinterpret_cast<BYTE*>(&data);

	for (int i = 0; i < typeSize; i++)
	{
		buffer[tailIndex++] = *(writer + i);
	}
}

template<UINT BufferSize>
template<typename T>
inline T StreamQueue<BufferSize>::Read(UINT size)
{
	// 입력된 사이즈 만큼 읽어서 반환을 한다.
	// size와 T의 크기가 다를 수도 있는데 그런 경우에 어떻게 처리를 해야할 것인가
	// 서버에서 사용할 것을 생각한다면 당연히 문자열 데이터를 고려해야한다.
	// 문자열 데이터는 당연히 1바이트씩 뽑는 것은 비효율적일 것이고..
	// T의 자료형이 char*, string과 같은 문자열 자료형일 때 다른 방식으로 동작하게 오버로딩을 해야할까?

	// 원하는 값이 저장량보다 크면 안된다.
	if (size > GetDataSize())
	{
		// 값을 반환해야하기 때문에 bool은 못쓰고
		// throw나 assert
		assert(!"Cannot read more than data stored in buffer.\n");
	}

	UINT typeSize = sizeof(T);

	// 임시 변수를 생성하고 그 주소 값을 가져와서 BYTE 포인터로 변환한다.
	// Write할 때와 마찬가지로 1바이트씩 읽어서 넣는다.
	T temp;
	BYTE* reader = reinterpret_cast<BYTE*>(&temp);

	for (int i = 0; i < typeSize; i++)
	{
		*(reader + i) = buffer[headIndex++];
	}

	// 읽었을 때 Head와 Tail이 같은 위치라면
	// 모든 데이터를 가져온 것이므로 0으로 초기화 시켜준다.
	if (tailIndex == headIndex)
	{
		headIndex = 0;
		tailIndex = 0;
	}

	return temp;
}

template<UINT BufferSize>
template<typename T>
inline StreamQueue<BufferSize>& StreamQueue<BufferSize>::operator<<(T& data)
{
	// Write로 데이터를 그대로 전달한다.
	if (Write(data))
	{
		// 결과값이 True인 경우 문제 없이 저장된 상태
		// 나중에 수정을 통해서 bool값이 아닌 다른 결과가 나올 수도
		// 쉬프트 연산을 이어가야 하니 이 객체를 반환한다.
		return *this;
	}
	else
	{
		// 실패했을 때 무엇을 할 것인가
		// TO DO
	}
}

template<UINT BufferSize>
template<typename T>
inline StreamQueue<BufferSize>& StreamQueue<BufferSize>::operator>>(T& data)
{
	// Read로 데이터를 전달한다.
	// 성공적으로 함수가 동작했다면 반환값을 data에 넣는다.
	data = Read<T>(sizeof(T));

	return *this;
}

template<UINT BufferSize>
inline UINT StreamQueue<BufferSize>::GetDataSize()
{
	// 현재 저장중인 데이터의 크기
	return tailIndex - headIndex;
}

template<UINT BufferSize>
inline bool StreamQueue<BufferSize>::Empty()
{
	// 현재 구조상 데이터가 없다면
	// tailIndex == 0해도 비어있는지 확인이 가능하지만
	// 혹시 모르니 사이즈로 체크
	return GetDataSize() == 0;
}

template<UINT BufferSize>
inline void StreamQueue<BufferSize>::Rebuild()
{
	int index = 0;

	// 값을 앞으로 이동
	while (headIndex == tailIndex)
	{
		buffer[index++] = buffer[headIndex++];
	}

	// 데이터를 가리키는 위치 변경
	headIndex = 0;
	tailIndex = index;
}