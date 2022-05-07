# __StreamQueue__

#### 간단한 서버를 만들기에 앞서 패킷 데이터를 송수신하는 용도로 사용할 Stream Queue 구조를 만든다.

------------

### __Problem__

-----------

1. Template 함수에서 Template 자료형에 변환이 지원되지 않아서 생기는 문제
  + 지정된 자료형을 매개변수로 받는 함수를 사용할 때
  + ~~지정된 자료형을 Template 자료형으로 반환할 때~~
	- 함수를 Overloading하는 것으로 해결
  + Template 자료형으로 선언된 변수에 지정된 자료형을 저장할 때

2. Pointer를 고려할 것인가? 고려한다면 어떤 식으로 다룰 것인가?
  + 해당 변수가 Pointer인 것을 우선 식별해야 함
  + 식별한 후 배열인 것을 어떻게 조사할 것인가 (sizeof와 _msize?)
  + 다중 Pointer인 경우에는 재귀?

------------

### __History__

-----------

**2022. 05. 07**

- StreamQueue.h
  + Read/Write에서 원하는 만큼만 읽고 쓸 수 있도록 수정
  + Template 함수의 반환값으로 string을 지정해서 사용할 수 없는 문제 수정
  + Full, WriteBufferCheck, ReadBufferCheck 함수 구현

**2022. 05. 06**

- StreamQueue.h
  + Write 함수 String은 따로 동작하도록 Overloading
  + 버퍼를 한번에 문자열로 읽는 ReadString 함수 구현
  + GetDataSize(), Empty(), Clear() 스트림의 상태를 반환하는 함수들 구현
  + Rebuild(), 간단하게 버퍼를 재구축?하는 함수

**2022. 05. 05**

 - StreamQueue.h
   + 자료형에 맞춰서 Binary형태로 Read/Write하는 구조 설계
   + <</>> 연산자 Overriding
   + 예외처리 및 구조의 설계에 방향이 필요함
