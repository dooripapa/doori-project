# **doori-project(version 0.1)**
> 아들의 태명(doori)으로 시작된 프로젝트

> c++20 기반으로 만들어져 있습니다. Concept, Require, Template, Template parameter ... 등 적용되어 있습니다.
> 디자인패턴도 적용하였으나, 디자인패턴보다 일반적프로그래밍(generic)을 선호합니다.
> 컴파일은 cmake 버전 3.15으로 적용되어 있습니다.
> 해당 프로젝트의 구조를 파악하기 위해서는 doori_uml 폴더에 참고해 주세요. 해당 텍스트는 plantuml[https://plantuml.com/ko]  의 의해서 제작되어 있습니다.
> 각각의 클래스의 API사용법은 단위테스트폴더 doori_test에서 볼 수 있습니다.
------------------------------

## *Architecture*
![Architecture](https://github.com/dooripapa/doori-project/blob/master/doori_uml/images/doori.png)
1. doori-api : libraries 집합체
2. doori-service : doori-api을 이용하여 만들어진 services
   1. Tnsd : Topology를 관리하기 위한, middleware
   1. Tnsdb : Topic Naming Service DB
3. doori-solution
   1. Watcher
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
   - [Bigdecimal](https://github.com/dooripapa/bigdecimal) : 부동소수점 Epsilon의 문제를 극복하기 위한 객체, 무한대 사칙연산가능

------------------------------

## *doori-service*

### [Tnsd](https://github.com/dooripapa/doori-project/tree/master/Tnsd) : Topic Naming Service Daemon
