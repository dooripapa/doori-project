# doori-project
> 사랑하는 아들의 태명을 본 따 시작된 프로젝트

## 아키텍처
1. doori-api : 하위레벨의 libraries 집합체
2. doori-service : doori-api 도구로 만들어진 daemon 프로세스(서비스)
3. doori-solution : doori-service 기반으로, 사용자에게 편의성을 제공하는 솔루션.
> 일차적인 목표는, 리녹스 모니터링 솔루션을 목표로 함.

### doori-api
1. 통신 libraries, 캡슐화 객체들
> Addr, Endpoint, Connection, Epoll, EpollEvents
   
2. 데이터캡슐화 위한 객체들
> Data, DataSegment, Stream, Dictionary, Json, Json_value
   
3. 데이터구조캡슐화 위한 객체들
> Tree, Branch
   
4. 미들웨어 서비스 캡슐화 위한 객체들
> Protocol, Topic, ICommunication, ,Publisher, Subscriber
 
5. 프로세스 캡슐화 객체들
> Getopt, Runner, Application
 
6. 기타 객체 
> Regex, Error

### doori-service
1. Middleware : tnsd
> Topic Naming Service Daemon.
> doori_service 풀더의, pub, sub, tnsd 참고할것.

2. MemoryDB : tnsdb
> Token Number Service DB. -----> 설계중.


### doori-solution
1. doori-watcher
> Linux monitoring application -----> 설계중.
