# **doori-project(version 0.1)**
> 아들의 태명(doori)으로 시작된 프로젝트

> c++20 기반으로 만들어져 있습니다. Concept, Require, Template, Template parameter ... 등 적용되어 있습니다.
> 디자인패턴도 적용하였으나, 디자인패턴보다 일반적프로그래밍(generic)을 선호합니다.
> 컴파일은 cmake 버전 3.15으로 적용되어 있습니다.
> 해당 프로젝트의 구조를 파악하기 위해서는 doori_uml 폴더에 참고해 주세요. 해당 텍스트는 plantuml[https://plantuml.com/ko]  의 의해서 제작되어 있습니다.
> 각각의 클래스의 API사용법은 단위테스트폴더 doori_test에서 볼 수 있습니다.
------------------------------

## *Software Architecture*
1. doori-api : libraries
2. doori-service : doori-api's libraries 으로 만들어진 daemon 서비스
   1. Tnsd : Topology를 관리하기 위한, middleware
   1. Tnsdb : Topic Naming Service DB
3. doori-solution
------------------------------

## *doori-api*
1. Communication
   - EpollApi : Epoll 추상객체
   - Socket : TCP 추상객체
   - TCP : Socket 생성 추상객체
   - TcpApi : linux socket를 설정을 위한 추상객체
   - TCPBuilder : Socket 객체를 생성을 위한 빌더객체
2. Data
   - Fid,FidSegment : 현재사용불가
   - Stream
   - Dictionary : 설정파일 추상객체
   - Json, Json_Value
3. Data Structure
   - Tree : 데이터집합 추상객체
   - Branch : 데이터집합 작은 단위 추상객체
   - ILeafKey(Interface) : Branch 의 데이터('Leaf'이라는 추상화) 접근을 위한 Finder Key 추상객체
4. Tnsd(dedicated)
   - Protocol : Tnsd 영역의 Topologis Node간 통신프로토콜 정의한 추상객체
   - Topic : ILeafKey 인터페이스를 상속받은 실체객체
   - Header : 인터페이스 Stream::IHeader 상속받은 Tnsd 영역의 프로토콜 추상객체
   - Body : 인터페이스 Stream::IBody 상속받은 Tnsd 영역의 프로토콜 데이터부분 추상객체
   - NodeInfo : Tnsd가 Topologies 관리 하기 위한 데이터객체(Leaf)
5. Process
   - Getopt
   - Runner
   - Application
6. Common
   - Log
   - Error : 객체 상태를 관리 객체
   - Util
7. Stream
   - IHeader(Interface)
   - IBody(Interface)
   - IStream(Interface)
   - StreamTemplate<IHeader,IBody>
8. Etc
   - Regex : 정규식 Wrapper객체
   - Bigdecimal : 부동소수점 Epsilon 문제를 없앤 객체, 무한대 사칙연산

------------------------------

## *doori-service*
### Tnsd : 미들웨어
> **T**opic **N**aming **S**ervice **D**aemon. => Tnsd

#### Topolgies
* Tnsd : Publisher와 Subscriber를 연결시켜주는 데몬프로세스(서비스)
* Publisher : 해당 Topic에 관심있는 Subscriber에게 데이터를 송신하는 프로세스
* Subscriber : 해당 Topic의 데이터에 관심있는 프로세스

#### Protocol
* Alive : Tnsd에게 살아있음을 알림.

![Alive Protocol](https://github.com/dooripapa/doori-project/blob/master/doori_uml/Alive.png)

* Notify : Tnsd에가 Topic에 관심있다는 것을 알림

![Notify Protocol](https://github.com/dooripapa/doori-project/blob/master/doori_uml/Subscriber_flow.png)

* Anwser : Topologies에 응답메시지를 보냄
* Change : Topic관심 있는 Topologies가 변경. 통보, 섹션연결 요청

![Change Protocol](https://github.com/dooripapa/doori-project/blob/master/doori_uml/Publisher_flow.png)

* Close : 이제는 Topic에 관심을 두지 않는다는 의미. 섹션종료, 리소스해제

![Close Protocol](https://github.com/dooripapa/doori-project/blob/master/doori_uml/Close.png)

* Report : Admin계정을 위한 정보프로토콜

