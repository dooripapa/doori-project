# **doori-project**
> 사랑하는 아들의 태명(doori)을 본 따 시작된 프로젝트
------------------------------

## *Software Architecture*
1. doori-api : 하위레벨의 libraries 집합체
2. doori-service : doori-api 도구로 재생산된 daemon 타입 서비스
3. doori-solution : doori-service 기반으로, 특정 목적에 따라 구현된 서비스
------------------------------

## *doori-api*
1. Communication Member Objects
   - Addr
   - Endpoint
   - Connection
   - Epoll
   - EpollEvents
   
2. Data Stream Objects
   - Data
   - DataSegment
   - Stream
   - Dictionary
   - Json
   - Json_value
   
3. Data Structure Objects
   - Tree
   - Branch
   
4. Middleware Support Objects
   - Protocol
   - Topic
   - ICommunication
   - Publisher
   - Subscriber
 
5. Process Support Objects
   - Getopt
   - Runner
   - Application
 
6. Common Objects
   - Log
   - Error
 
6. Etc
   - Regex
   - Bigdecimal

------------------------------

## *doori-service*
1. Middleware : TNSD 
> **T**opic **N**aming **S**ervice **D**aemon. => TNSD

2. MemoryDB : TNSDB
> **T**oken **N**umber **S**ervice **DB**. ==> TNSDB

------------------------------
## doori-solution
1. doori-watcher
> Linux monitoring application -----> 	https://github.com/GULapp/doori-watcher 