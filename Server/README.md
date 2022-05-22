# __Server__

#### Socket Buffer에 데이터가 들어오면 Event를 발생시켜 Client와 통신하는 클래스
#### 간단한 서버이므로 로그인, 로비, 인게임 서버를 따로 사용하지 않고 통합한다.

------------

### __History__

-----------

**2022. 05. 22**

- Server
  + Server에서 Client 관리하는 ClientManager를 따로 분리
  + Server에서 클라이언트의 연결 요청만 처리하고, ClientManager로 인계

- ClientManager
  + Server에 접속중인 Client들의 관리, 송/수신 요청 처리
  + 각 Client에 Unique한 Network ID 부여
  + 송/수신, 연결 해제 요청이 들어왔을 때 PlayerManager의 작업 Queue에 등록

- PlayerManager
  + 연결된 Player들의 목록을 저장하고, 송/수신이 될 때 각 Player에 작업 전달

- Player
  + Player의 Network ID, 상태 등을 저장하고 관리할 클래스
  + 송/수신 패킷 관리, 연결 해제 됐을 때 필요한 작업 처리

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