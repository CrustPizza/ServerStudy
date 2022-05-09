/****************************
*	Test for stream queue   *
*	main.cpp				*
*							*
*	Created : 2022/05/03	*
*	Updated : 2022/05/07	*
*****************************/

#include <iostream>
#include "../StreamQueue/StreamQueue.h"

#pragma comment(lib, "../Debug/StreamQueue.lib")

int main()
{
	// ���� Ȯ���� ���� �׽�Ʈ �ڵ�
	{
		StreamQueue<> testClass;

		int a = 10;
		float b = 10.0f;
		char c = 'A';

		testClass.Write(a);
		testClass.Write(b);
		testClass.Write(c);

		// Queue �����̱� ������ ���� ������� ���� ���ϴ� ���� ���� �� �ִ�.
		int d = testClass.Read<int>(sizeof(int));
		float e = testClass.Read<float>(sizeof(float));
		char f = testClass.Read<char>(sizeof(char));

		std::cout << "Int : " << d << "\tFloat : " << e << "\tChar : " << f << std::endl;

		// Shift ���� üũ
		a += 30;
		b += 100.0f;
		c = 'T';

		testClass << a << b << c;
		testClass >> d >> e >> f;

		std::cout << "Int : " << d << "\tFloat : " << e << "\tChar : " << f << std::endl;

		// ���� ������ �׽�Ʈ
		//int arr[300] = {};

		//for (int i = 0; i < 300; i++)
		//{
		//	arr[i] = i + 1;
		//	testClass << arr[i];
		//}

		//testClass >> a;

		// String �׽�Ʈ
		std::string test = "adfadfasdfadsfasdfasdfads";
		std::string result;

		testClass << test;
		testClass >> result;
		//result = testClass.ReadString();

		std::cout << result << std::endl;
	}

	return 0;
}