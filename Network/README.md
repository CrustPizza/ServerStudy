# __Network__

#### 네트워크 통신을 위해 필요한 클래스들을 이 프로젝트에 모아서 라이브러리화한다.
#### 라이브러리를 Server와 Client 각각에서 사용할 것

------------

### __Problem__

-----------

- 어느정도 호환성을 고려하고 싶었는데 Socket 핸들의 경우는 어떻게 해야하는가..

------------

### __History__

-----------

**2022. 05. 09**

- SocketInterface.h
  + Socket의 기본 인터페이스

- TCPSocket.h
  + SocketInterface를 상속받아서 사용할 TCPSocket 클래스