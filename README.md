# **doori-project**
> 아들의 태명(doori)으로 시작된 프로젝트
------------------------------

## *Software Architecture*
1. doori-api : libraries
2. doori-service : doori-api's libraries 으로 만들어진 daemon 서비스
3. doori-solution : 제작중...
------------------------------

## *doori-api*
1. Communication
   - EpollApi
   - Socket 
   - TCP
   - TcpApi
   - TCPBuilder
2. Data
   - Data
   - DataSegment
   - Stream
   - Dictionary
   - Json
   - Json_value
   - Stream
3. Data Structure
   - Tree
   - Branch
4. Tnsd(dedicated)
   - Protocol
   - Topic
5. Process
   - Getopt
   - Runner
   - Application
6. Common
   - Log
   - Error
7. Stream
   - IHeader(Interface)
   - IBody(Interface)
   - IFooter(Interface)
8. Etc
   - Regex
   - Bigdecimal

------------------------------

## *doori-service*
### Tnsd : 미들웨어
> **T**opic **N**aming **S**ervice **D**aemon. => Tnsd


#### Topolgies
* Tnsd : Publisher와 Subscriber를 연결시켜주는 서비스프로세스
* Publisher : 해당 Topic에 관심있는 Subscriber에게 데이터를 송신하는 프로세스
* Subscriber : 해당 Topic의 데이터에 관심있는 프로세스

#### Protocol
* Alive : Tnsd에게 살아있음을 알림.
* Notify : Tnsd에가 Topic에 관심있다는 것을 알림
* Anwser : Topologies에 응답메시지를 보냄
* Change : Topic관심 있는 Topologies가 변경. 통보, 섹션연결 요청
* Close : 이제는 Topic에 관심을 두지 않는다는 의미. 섹션종료, 리소스해제