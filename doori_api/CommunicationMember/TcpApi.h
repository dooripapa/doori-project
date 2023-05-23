//
// Created by jaeseong on 23. 3. 1.
//

#ifndef DOORI_PROJECT_TCPAPI_H
#define DOORI_PROJECT_TCPAPI_H

#include "Common/Log.h"
#include "Socket.h"
#include "Common/Error.h"
#include <string>
#include <sys/socket.h>
#include <bits/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <functional>

namespace doori::CommunicationMember {

        class Socket;

        class TcpApi final : public Common::Error {

        public:

            TcpApi();
            explicit TcpApi(Socket socket);
            TcpApi(const TcpApi&) = default;
            TcpApi(TcpApi&&) = default;
            TcpApi& operator=(const TcpApi&) = default;
            TcpApi& operator=(TcpApi&&) = default;

            /**
             * DOMAIN(AF_INET), mMsgName(SOCK_STREAM), PROTOCOL(0) 기본설정으로 소켓 file descriptor리턴
             * @return 소켓 FD
             */
            void CreateSocket();

            /**
             * SOL_SOCKET을 SO_REUSEPORT 와 SO_REUSEADDR 설정함
             * @note 이 함수가 호출되기전 CreateSocket()호출로 반환된 소켓FD 필요함
             * @param socket Socket Wrapper 클래스
             * @param ip가 ""이면, INADDR_ANY, 지정되면 해당IP로 바인딩 및 옵션이 셋팅됨
             * @param port 포트번호
             * @todo 수신부 IP, PORT 접속정보를 확인할 수 있는 옵션도 셋팅할것
             * @return Socket Wrapper 클래스
             */
            void SetReuseOpt(const std::string& ip, const std::string& port);

            /**
             * recv()함수에 Timeout 를 설정하기위해서 사전에 호출되어야 하는 함수
             * 소켓을 재설정함. SOL_SCOCKET, SO_RCVTIMEO를 설정함.
             * @param socket Socket Wrapper 클래스
             * @param timeout Timeout값(second)
             * @return Socket Wrapper 클래스
             */
            void SetTimeoutOpt(std::uint8_t timeout);

            /**
             * IP, PORT정보를 이용하여 바인딩처리함
             * @note 이 함수가 호출되기전 CreateSocket()호출로 반환된 소켓FD 필요함
             * @param socket Socket Wrapper 클래스
             * @param sockaddrIn
             * @return Socket Wrapper 클래스
             */
            void Bind(const std::string& ip, const std::string& port);

            /**
             * 입력 받은 소켓을 listen상태로 만든다.
             * @note 이 함수가 호출되기전 CreateSocket()호출로 반환된 소켓FD 필요함
             *       또한 Bind()함수로 바인딩 상태이어야 한다.
             * @param socket Socket Wrapper 클래스
             * @param backlogNum 접속 요청이 들어올 경우, 최대 대기 배열 수
             * @return Socket Wrapper 클래스
             */
            void Listen(int backlogNum);

            /**
             * 입력 받은 소켓을 accept상태로 만든다. 수신요청이 오면, 또 다른 FD로 리턴되고, 해당 FD로 통신이 가능하다.
             * @note 이 함수가 호출되기전 CreateSocket()호출로 반환된 소켓FD 필요함
             *       또한 Bind()함수로 바인딩 상태이어야 한다.
             * @param socket Socket Wrapper 클래스
             * @param sockaddrIn
             * @return Socket Wrapper 클래스
             */
            Socket Accept();

            /**
             * 원격지 접속을 요청합니다.
             * @param socket Socket Wrapper 클래스
             * @param ip 원격지 IP
             * @param port 원격지 PORT
             * @return Socket Wrapper 클래스
             */
            void Connect(const string& ip, const string& port);

            /**
             * 데이터를 송신합니다.
             * @param socket Socket Wrapper 클래스
             * @note established socket꼭 특정 IP, PORT에 바인딩 될 필요는 없음.
             * @param data 데이터
             * @param dataLen 데이터길이
             * @return Socket Wrapper 클래스
             */
            void Send(const char *data, uint8_t dataLen);

            /**
             * 데이터를 수신합니다.
             * @param socket Socket Wrapper 클래스
             * @note established socket꼭 특정 IP, PORT에 바인딩 될 필요는 없음.
             * @param dataContainer 데이터를 저장할 변수
             * @param dataLen  데이터 수신 후 수신이 완료 되기 위한 데이터 길이
             * @return Socket Wrapper 클래스
             */
            void Recv(char *dataContainer, std::uint8_t dataLen);

            /**
             * 주어진 ip, port정보를 이용하여, 접속요청 바로 시도 합니다.
             * 내부적으로 SetTimeoutOpt 자동으로 셋팅합니다.
             * @param ip destination ip
             * @param port  destination port
             * @param timeout SetTimeoutOpt 값
             * @return Socket Wrapper 클래스
             */
            void Connect(const string& ip, const string& port, std::uint8_t timeout);

            /**
             * 이미 Established 소켓으로 부터 데이터를 주고 받습니다.
             * @param socket Socket Wrapper 클래스
             * @param data  송신데이터
             * @param dataLen  송신데이터 길이
             * @param recvData 수신데이터
             * @param tilDataSize 수신데이터 완전한 데이터 사이즈
             * @return Socket Wrapper 클래스
             */
            void Reply(const char *data, uint8_t dataLen, char *recvData, uint8_t tilDataSize);

            /**
             * Socket wrapper 객체를 리턴합니다.
             * @return Socker wrapper class
             */
            Socket GetSocket() ;
        private:
            Socket mSocket;
        };

    } // CommunicationMember

#endif //DOORI_PROJECT_TCPAPI_H
