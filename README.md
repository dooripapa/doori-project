■ doori-project
 - doori-api : service 구현을 위한 api 집합체
 - doori-service : doori-solution의 특정 서비스를 위임받아 처리하는 daemon프로세스
 - doori-solution : 사용자에게 편의성 제공하는 솔루션


■ doori-api
 - 통신캡슐화 위한 객체들
   > Addr
   > Endpoint
   > Connection
   > Epoll
   > EpollEvents
   
 - 데이터캡슐화 위한 객체들
   > Data
   > DataSegment
   > Stream
   > Dictionary
   
 - 데이터구조캡슐화 위한 객체들
   > Tree
   > Branch
   
 - 미들웨어 서비스 캡슐화 위한 객체들
   > Protocol
   > Topic
   > ICommunication
   > Publisher
   > Subscriber
 
 - 프로세스 캡슐화 객체들
   > Getopt
   > Runner
   > Application
 
 - 기타 객체 
   > Regex


■ doori-service
 - tnsd ( Topic naming service daemon ) : doori 미들웨어

 - tnsdb ( Token number service db ) : doori 메모리DB
   > @todo

■ doori-solution
 - @todo
