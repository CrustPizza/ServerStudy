# __Network__

#### 네트워크 통신을 위해 필요한 클래스들을 이 프로젝트에 모아서 라이브러리화한다.
#### 라이브러리를 Server와 Client 각각에서 사용할 것

------------

### __Problem__

-----------

+ 어느정도 호환성을 고려하고 싶었는데 Socket 핸들의 경우는 어떻게 해야하는가..
  - 일단 SOCKET 사용, 오늘 교수님 말씀 들어보니 이거는 고민할 필요가 없어보인다.

+ Socket을 move 연산 적용시키는 부분에서 어디까지 이동시킬 것인가

------------

### __History__

-----------

**2022. 05. 22**

- Protocol
  + 현재 Project에서 사용할 통신 Protocol 목록
  + 일단 Print, Echo, Broadcast만 등록

**2022. 05. 16**

- TCPSocket
  + Send/Recv Buffer 및 StreamQueue 생성
  + Send/Recv 함수 구현
  + Thread Safe하도록 Mutex 적용

**2022. 05. 15**

- TCPSocket
  + Listen과 Accept 구현

**2022. 05. 11**

- SocketInterface
  + 내부에 있던 SocketAddrIn 구조체를 Endpoint 클래스로 이동

- TCPSocket
  + Open / Bind / Connet / Close / Shutdown 구현

- Endpoint
  + IP 정보와 Port 번호를 다루기 위한 클래스 생성
  + IP 주소 문자열과 Port 번호를 받는 생성자 사용

**2022. 05. 10**

- SocketInterface
  + Socket 인터페이스 개선하여 옵션은 배제한 상태의 인터페이스로 구성

- TCPSocket
  + TCPSocket의 대략적인 인터페이스 설계

**2022. 05. 09**

- SocketInterface
  + Socket의 기본 인터페이스

- TCPSocket
  + SocketInterface를 상속받아서 사용할 TCPSocket 클래스