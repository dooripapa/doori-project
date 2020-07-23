# doori-project
> 사랑하는 아들의 태명을 본 따 시작된 프로젝트

## 아키텍처
1. doori-api : 하위레벨의 libraries 집합체
2. doori-service : doori-api 도구로 만들어진 daemon 프로세스(서비스)
3. doori-solution : 사용자에게 편의성을 제공하는 솔루션.

### doori-pai
1. 통신 libraries, 캡슐화 객체들
> Addr, Endpoint, Connection, Epoll, EpollEvents
   
2. 데이터캡슐화 위한 객체들
> Data, DataSegment, Stream, Dictionary, Json
   
3. 데이터구조캡슐화 위한 객체들
> Tree, Branch
   
4. 미들웨어 서비스 캡슐화 위한 객체들
> Protocol, Topic, ICommunication, ,Publisher, Subscriber
 
5. 프로세스 캡슐화 객체들
> Getopt, Runner, Application, Error
 
6. 기타 객체 
> Regex

### doori-service
1. Middleware : tnsd (프로토타입완료)
> Topic Naming Service Daemon.

2. MemoryDB : tnsdb
> Token Number Service DB. -----> ing


### doori-solution
1. doori-watcher
> Linux monitoring application -----> ing

