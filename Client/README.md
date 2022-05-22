# __Client__

#### Socket Buffer에 데이터가 들어오면 Event를 발생시켜 Server와 통신하는 클래스

------------

### __History__

-----------

**2022. 05. 22**

- GameProcess
  + Remote Player 추가, Packet 전송 옵션 관련 테스트 기능 추가

- Client
  + Server와 송/수신하는 주체, PlayerManager의 작업 Queue에 전달

- PlayerManager
  + Client로부터 작업을 전달받고 Player에 전달 구현

- Player
  + Packet을 분석해서 처리하는 기능 추가

**2022. 05. 16**

- main
  + GameProcess와 Client 클래스 쓰레드 생성

- Client
  + Singleton화
  + Send/Recv 함수에서 간단하게 Packet Size 정도만 추가하게 구현

- GameProcess
  + Singleton화
  + Game의 Base로 사용할 계획
  
- Player
  + Packet을 송/수신할 임시 객체, 나중에 User와 Opponent 객체로 사용

- stdafx
  + PCH 생성

**2022. 05. 15**

- main
  + 간단한 테스트를 위한 작성

- Client
  + Client 생성시 WSAStartup 호출, clientSocket 생성
  + IP와 PORT를 명령 인수로 전달 받아서 서버에 연결 시도
  + Client 소멸시 WSACleanup 호출, clientSocket 해제