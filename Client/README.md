# __Client__

#### Socket Buffer에 데이터가 들어오면 Event를 발생시켜 Server와 통신하는 클래스

------------

### __History__

-----------

**2022. 05. 15**

- main
  + 간단한 테스트를 위한 작성

- Client
  + Client 생성시 WSAStartup 호출, clientSocket 생성
  + IP와 PORT를 명령 인수로 전달 받아서 서버에 연결 시도
  + Client 소멸시 WSACleanup 호출, clientSocket 해제