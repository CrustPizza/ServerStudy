# __Server__

#### Socket Buffer에 데이터가 들어오면 Event를 발생시켜 Client와 통신하는 클래스
#### 간단한 서버이므로 로그인, 로비, 인게임 서버를 따로 사용하지 않고 통합한다.

------------

### __History__

-----------

**2022. 05. 16**

- Server
  + 송/수신 확인을 위해 Echo 서버 구현 및 테스트

**2022. 05. 15**

- main
  + 간단한 테스트를 위한 작성

- Server
  + Server 생성시 WSAStartup 호출, listenSocket 생성 및 바인딩까지 처리
  + Listen할 때 Accept도 한번에 처리하고, Accept의 결과를 Client List에 보관
  + Server 소멸시 WSACleanup 호출, listenSocket 및 연결된 ClientSocket 해제