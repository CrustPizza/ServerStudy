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
	// ���۸� �������� �� ������ �������� �� ������ ���
	// �����ʹ� ���̳ʸ� ���� ������� bit ���·� ����
	// �ϴ� ��Ʈ���̴ϱ� �迭�� �°���
	BYTE buffer[BufferSize];

	UINT headIndex;
	UINT tailIndex;

public:
	StreamQueue();
	~StreamQueue();

	// �ϴ� �޾Ƽ� �ڷ����� ũ�⸦ ����ϰ�
	// �ڷ����� ũ�⸸ŭ ���ۿ� ����
	template <typename T>
	bool Write(T& data);

	// Size��ŭ ���� �о T�� ��ȯ �� ��ȯ
	template <typename T>
	T Read(UINT size);

	// Write�� ������ ������ �Ѵ�.
	template <typename T>
	StreamQueue& operator <<(T& data);

	// Read�� ������ ������ �Ѵ�.
	// �ٸ� ������ �μ��� ������ �ڷ����� ũ�⿡ �°� ��ȯ�Ѵ�.
	template <typename T>
	StreamQueue& operator >>(T& data);
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

	// ���ۿ� ������ �Ұ����� ���
	if (typeSize > BufferSize - tailIndex)
	{
		// �ڷ����� ��°�� �ִ� ���̱� ������ �����ؼ� �ִ� ���� ���� �� ����.
		// throw�� ����, assert�� ����, false�� ���� ���
		return false;
	}

	// reinterpret_cast�� ���ؼ� ��Ʈ ���� �״�� ���� �Ѵ�.
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
	// �Էµ� ������ ��ŭ �о ��ȯ�� �Ѵ�.
	// size�� T�� ũ�Ⱑ �ٸ� ���� �ִµ� �׷� ��쿡 ��� ó���� �ؾ��� ���ΰ�
	// �������� ����� ���� �����Ѵٸ� �翬�� ���ڿ� �����͸� ����ؾ��Ѵ�.
	// ���ڿ� �����ʹ� �翬�� 1����Ʈ�� �̴� ���� ��ȿ������ ���̰�..
	// T�� �ڷ����� char*, string�� ���� ���ڿ� �ڷ����� �� �ٸ� ������� �����ϰ� �����ε��� �ؾ��ұ�?

	// ���ϴ� ���� ���差���� ũ�� �ȵȴ�.
	if (size > tailIndex - headIndex)
	{
		// ���� ��ȯ�ؾ��ϱ� ������ bool�� ������
		// throw�� assert
		assert(!"Cannot read more than data stored in buffer.\n");
	}

	UINT typeSize = sizeof(T);

	// �ӽ� ������ �����ϰ� �� �ּ� ���� �����ͼ� BYTE �����ͷ� ��ȯ�Ѵ�.
	// Write�� ���� ���������� 1����Ʈ�� �о �ִ´�.
	T temp;
	BYTE* reader = reinterpret_cast<BYTE*>(&temp);

	for (int i = 0; i < typeSize; i++)
	{
		*(reader + i) = buffer[headIndex++];
	}

	return temp;
}

template<UINT BufferSize>
template<typename T>
inline StreamQueue<BufferSize>& StreamQueue<BufferSize>::operator<<(T& data)
{
	// Write�� �����͸� �״�� �����Ѵ�.
	if (Write(data))
	{
		// ������� True�� ��� ���� ���� ����� ����
		// ���߿� ������ ���ؼ� bool���� �ƴ� �ٸ� ����� ���� ����
		// ����Ʈ ������ �̾�� �ϴ� �� ��ü�� ��ȯ�Ѵ�.
		return *this;
	}
	else
	{
		// �������� �� ������ �� ���ΰ�
		// TO DO
	}
}

template<UINT BufferSize>
template<typename T>
inline StreamQueue<BufferSize>& StreamQueue<BufferSize>::operator>>(T& data)
{
	// Read�� �����͸� �����Ѵ�.
	// ���������� �Լ��� �����ߴٸ� ��ȯ���� data�� �ִ´�.
	data = Read<T>(sizeof(T));

	return *this;
}