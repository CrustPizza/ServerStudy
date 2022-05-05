/****************************
*	Test for stream queue	*
*							*
*	Created : 2022/05/03	*
*	Updated : 2022/05/05	*
*****************************/

#include <iostream>
#include "../StreamQueue/StreamQueue.h"

#pragma comment(lib, "../Debug/StreamQueue.lib")

int main()
{
	// 동작 확인을 위한 테스트 코드
	{
		StreamQueue<> testClass;

		int a = 10;
		float b = 10.0f;
		char c = 'A';

		testClass.Write(a);
		testClass.Write(b);
		testClass.Write(c);

		// Queue 구조이기 때문에 넣은 순서대로 빼야 원하는 값을 얻을 수 있다.
		int d = testClass.Read<int>(sizeof(int));
		float e = testClass.Read<float>(sizeof(float));
		char f = testClass.Read<char>(sizeof(char));

		std::cout << "Int : " << d << "\tFloat : " << e << "\tChar : " << f << std::endl;

		// Shift 연산 체크
		a += 30;
		b += 100.0f;
		c = 'T';

		testClass << a << b << c;
		testClass >> d >> e >> f;

		std::cout << "Int : " << d << "\tFloat : " << e << "\tChar : " << f << std::endl;
	}

	return 0;
}