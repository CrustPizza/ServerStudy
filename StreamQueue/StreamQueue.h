/****************************
*							*
*	Stream Queue.h			*
*							*
*	Created : 2022/05/03	*
*	Updated : 2022/05/16	*
*****************************/

#pragma once
#include <assert.h>
#include <string>
#include <typeinfo>

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

	// ���ڿ��� ���� overloading �ؾߵȴ�. 
	bool Write(std::string& data);
	bool Write(const char* data, UINT size);

	// Size��ŭ ���� �о T�� ��ȯ �� ��ȯ
	template <typename T>
	T Read(UINT size);

	// ���ڿ��� ���ϴ� ��ŭ�� �о����
	const char* Read(UINT size);
	void Read(char* data, UINT size);

	// ���ڿ��� ���� �� �о�´�.
	const char* ReadAll();

	// Write�� ������ ������ �Ѵ�.
	template <typename T>
	StreamQueue& operator <<(T& data);

	// Read�� ������ ������ �Ѵ�.
	// �ٸ� ������ �μ��� ������ �ڷ����� ũ�⿡ �°� ��ȯ�Ѵ�.
	template <typename T>
	StreamQueue& operator >>(T& data);

	// ���� �ڷ����� ���õ� ������ �׳� Overloading���� �ذ�
	StreamQueue& operator >>(std::string& data);

	// ��Ʈ���� ���¸� Ȯ���ϱ� ���� �Լ���
	UINT GetDataSize();
	UINT GetRemainSize();
	bool Full();
	bool Empty();
	void Clear();

private:
	// Head�� ��ġ�� 0���� �ٲٴ� �Լ��ε�
	// �̸��� Push�� �̻��ϰ� Sort�� �̻��ϰ�..
	void Rebuild();
	bool WriteBufferCheck(UINT size);
	bool ReadBufferCheck(UINT size);
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
	UINT typeSize = sizeof(data);

	// ���ۿ� ������ �Ұ����� ���
	if (typeSize > BufferSize - GetDataSize())
	{
		// �ڷ����� ��°�� �ִ� ���̱� ������ �����ؼ� �ִ� ���� ���� �� ����.
		// throw�� ����, assert�� ����, false�� ���� ���
		return false;
	}

	// ������ ���������� ���۸� ������ �ʿ䰡 �ִ� ���
	if (typeSize > BufferSize - tailIndex)
	{
		// ȯ�� ť�� �ᵵ ������ �������� ������ ����
		// Head�� ���� Tail���� ũ�� ���� ���¸� �����Ѵ�.
		Rebuild();
	}

	// reinterpret_cast�� ���ؼ� ��Ʈ ���� �״�� ���� �Ѵ�.
	BYTE* writer = reinterpret_cast<BYTE*>(&data);

	for (int i = 0; i < typeSize; i++)
	{
		buffer[tailIndex++] = writer[i];
	}
}

template<UINT BufferSize>
inline bool StreamQueue<BufferSize>::Write(std::string& data)
{
	// string�� ��쿡�� length
	UINT typeSize = data.length();

	if (WriteBufferCheck(typeSize) != true)
	{
		// ���� ������ �����ϸ� false�� ��ȯ�ȴ�.
		return false;
	}

	// ������ String���� 1����Ʈ�� �о�� �� ������ �׳� �ִ´�.
	for (int i = 0; i < typeSize; i++)
	{
		buffer[tailIndex++] = data[i];
	}
}

template<UINT BufferSize>
inline bool StreamQueue<BufferSize>::Write(const char* data, UINT length)
{
	if (WriteBufferCheck(length) != true)
	{
		// ���� ������ �����ϸ� false�� ��ȯ�ȴ�.
		return false;
	}

	// ������ String���� 1����Ʈ�� �о�� �� ������ �׳� �ִ´�.
	for (int i = 0; i < length; i++)
	{
		buffer[tailIndex++] = data[i];
	}
}

template<UINT BufferSize>
template<typename T>
inline T StreamQueue<BufferSize>::Read(UINT size)
{
	if (ReadBufferCheck(size) != true)
		; // Error

	UINT typeSize = sizeof(T);

	// �ӽ� ������ �����ϰ� �� �ּ� ���� �����ͼ� BYTE �����ͷ� ��ȯ�Ѵ�.
	// Write�� ���� ���������� 1����Ʈ�� �о �ִ´�.
	T temp;
	BYTE* reader = reinterpret_cast<BYTE*>(&temp);

	for (int i = 0; i < typeSize; i++)
	{
		reader[i] = buffer[headIndex++];
	}

	// �о��� �� Head�� Tail�� ���� ��ġ���
	// ��� �����͸� ������ ���̹Ƿ� 0���� �ʱ�ȭ �����ش�.
	if (tailIndex == headIndex)
	{
		Clear();
	}

	return temp;
}

template<UINT BufferSize>
inline const char* StreamQueue<BufferSize>::Read(UINT size)
{
	if (ReadBufferCheck(size) != true)
		; // Error

	// ���۸� �ʿ��� ��ŭ �д� �͵� �ʿ��� �� ����
	std::string str = "";

	for (int i = 0; i < size; i++)
	{
		str += buffer[headIndex++];
	}

	// �о��� �� Head�� Tail�� ���� ��ġ���
	// ��� �����͸� ������ ���̹Ƿ� 0���� �ʱ�ȭ �����ش�.
	if (tailIndex == headIndex)
	{
		Clear();
	}

	return str.c_str();
}

template<UINT BufferSize>
inline void StreamQueue<BufferSize>::Read(char* data, UINT size)
{
	if (ReadBufferCheck(size) != true)
		; // Error

	// 
	for (int i = 0; i < size; i++)
	{
		data[i] = buffer[headIndex++];
	}

	// �о��� �� Head�� Tail�� ���� ��ġ���
	// ��� �����͸� ������ ���̹Ƿ� 0���� �ʱ�ȭ �����ش�.
	if (tailIndex == headIndex)
	{
		Clear();
	}
}

template<UINT BufferSize>
inline const char* StreamQueue<BufferSize>::ReadAll()
{
	return Read(GetDataSize());
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
		assert(!"There is no more space to store in the buffer.\n");
	}
}

template<UINT BufferSize>
template<typename T>
inline StreamQueue<BufferSize>& StreamQueue<BufferSize>::operator>>(T& data)
{
	// Read�� �����͸� �����Ѵ�.
	// String�� ��쿡 �ѹ��� ���� �־�����
	if (typeid(std::string) == typeid(T))
	{
		//data = ReadString();
		// �� �̰� �������� �޾�ô� �����ε� ���� �ذ����� ���ߴ�.
		// Template �Լ��� ����� �� ������ �ڷ����� ����ϴ� ���
		// ������ �ش� �ڵ尡 �������� �ʴ� ��Ȳ�̶� �ϴ���
		// ��ȯ�� ���������� ������ ������ ���� ���̽��� �ִµ�
		// ��� ó�����־�� �ұ�..
		//decltype(data) str = ReadString();
		//data = str;
	}
	else
	{
		// ���������� �Լ��� �����ߴٸ� ��ȯ���� data�� �ִ´�.
		data = Read<T>(sizeof(T));
	}

	return *this;
}

template<UINT BufferSize>
inline StreamQueue<BufferSize>& StreamQueue<BufferSize>::operator>>(std::string& data)
{
	data = ReadAll();

	return *this;
}

template<UINT BufferSize>
inline UINT StreamQueue<BufferSize>::GetDataSize()
{
	// ���� �������� �������� ũ��
	return tailIndex - headIndex;
}

template<UINT BufferSize>
inline UINT StreamQueue<BufferSize>::GetRemainSize()
{
	return BufferSize - GetDataSize();
}

template<UINT BufferSize>
inline bool StreamQueue<BufferSize>::Full()
{
	return BufferSize == GetDataSize();
}

template<UINT BufferSize>
inline bool StreamQueue<BufferSize>::Empty()
{
	// ���� ������ �����Ͱ� ���ٸ�
	// tailIndex == 0�ص� ����ִ��� Ȯ���� ����������
	// Ȥ�� �𸣴� ������� üũ
	return GetDataSize() == 0;
}

template<UINT BufferSize>
inline void StreamQueue<BufferSize>::Clear()
{
	// ���� 0���� �ʱ�ȭ ���� �ʿ�� ���°� ����
	headIndex = 0;
	tailIndex = 0;
}

template<UINT BufferSize>
inline void StreamQueue<BufferSize>::Rebuild()
{
	int index = 0;

	// ���� ������ �̵�
	while (headIndex != tailIndex)
	{
		buffer[index++] = buffer[headIndex++];
	}

	// �����͸� ����Ű�� ��ġ ����
	headIndex = 0;
	tailIndex = index;
}

template<UINT BufferSize>
inline bool StreamQueue<BufferSize>::WriteBufferCheck(UINT size)
{
	// ���ۿ� ������ �Ұ����� ���
	if (size > BufferSize - GetDataSize())
	{
		// �ڷ����� ��°�� �ִ� ���̱� ������ �����ؼ� �ִ� ���� ���� �� ����.
		// throw�� ����, assert�� ����, false�� ���� ���
		return false;
	}

	// ������ ���������� ���۸� ������ �ʿ䰡 �ִ� ���
	if (size > BufferSize - tailIndex)
	{
		// ȯ�� ť�� �ᵵ ������ �������� ������ ����
		// Head�� ���� Tail���� ũ�� ���� ���¸� �����Ѵ�.
		Rebuild();
	}

	return true;
}

template<UINT BufferSize>
inline bool StreamQueue<BufferSize>::ReadBufferCheck(UINT size)
{
	// �Էµ� ������ ��ŭ �о ��ȯ�� �Ѵ�.
	// size�� T�� ũ�Ⱑ �ٸ� ���� �ִµ� �׷� ��쿡 ��� ó���� �ؾ��� ���ΰ�
	// �������� ����� ���� �����Ѵٸ� �翬�� ���ڿ� �����͸� ����ؾ��Ѵ�.
	// ���ڿ� �����ʹ� �翬�� 1����Ʈ�� �̴� ���� ��ȿ������ ���̰�..
	// T�� �ڷ����� char*, string�� ���� ���ڿ� �ڷ����� �� �ٸ� ������� �����ϰ� �����ε��� �ؾ��ұ�?

	// ���ϴ� ���� ���差���� ũ�� �ȵȴ�.
	if (size > GetDataSize())
	{
		// ���� ��ȯ�ؾ��ϱ� ������ bool�� ������
		// throw�� assert
		assert(!"Cannot read more than data stored in buffer.\n");
	}

	// ���� ���� �� assert�� �����ؼ� �ǹ̰� ������ ������ ������ ���ؼ�..
	return true;
}