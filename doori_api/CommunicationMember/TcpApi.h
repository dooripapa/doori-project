//
// Created by jaeseong on 23. 3. 1.
//

#ifndef DOORI_PROJECT_TCPAPI_H
#define DOORI_PROJECT_TCPAPI_H

#include "Common/Log.h"
#include <string>
#include <sys/socket.h>
#include <bits/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace doori {
    namespace CommunicationMember {

        /**
         * TcpApi이 함수는 static 기질을 가지면 안된다.
         */
        class TcpApi {
        public:
            /**
             * DOMAIN(AF_INET), mMsgName(SOCK_STREAM), PROTOCOL(0) 기본설정으로 소켓 file descriptor리턴
             * @return 소켓 FD
             */
            static int Socket();

            /**
             * SOL_SOCKET을 SO_REUSEPORT 와 SO_REUSEADDR 설정함
             * @note 이 함수가 호출되기전 Socket()호출로 반환된 소켓FD 필요함
             * @param fd 소켓 FD
             * @param ip가 ""이면, INADDR_ANY, 지정되면 해당IP로 바인딩 및 옵션이 셋팅됨
             * @param port 포트번호
             * @todo 수신부 IP, PORT 접속정보를 확인할 수 있는 옵션도 셋팅할것
             * @return
             */
            static int SetReuseOpt(int fd,std::string ip, std::string port);

            /**
             * recv()함수에 Timeout 를 설정하기위해서 사전에 호출되어야 하는 함수
             * 소켓을 재설정함. SOL_SCOCKET, SO_RCVTIMEO를 설정함.
             * @param fd  소켓 FD
             * @param timeout  Timeout값(second)
             * @return 재설정되고 나서, 리턴되는 소켓 FD, -1이면 error
             */
            static int SetTimeoutOpt(int fd, std::uint8_t timeout);

            /**
             * IP, PORT정보를 이용하여 바인딩처리함
             * @note 이 함수가 호출되기전 Socket()호출로 반환된 소켓FD 필요함
             * @param fd 소켓 FD
             * @param sockaddrIn
             * @return
             */
            static int Bind(int fd, std::string ip, std::string port);

            /**
             * 입력 받은 소켓을 listen상태로 만든다.
             * @note 이 함수가 호출되기전 Socket()호출로 반환된 소켓FD 필요함
             *       또한 Bind()함수로 바인딩 상태이어야 한다.
             * @param fd 소켓 FD
             * @param backlogNum 접속 요청이 들어올 경우, 최대 대기 배열 수
             * @return
             */
            static int Listen(int fd, int backlogNum);

            /**
             * 입력 받은 소켓을 accept상태로 만든다. 수신요청이 오면, 또 다른 FD로 리턴되고, 해당 FD로 통신이 가능하다.
             * @note 이 함수가 호출되기전 Socket()호출로 반환된 소켓FD 필요함
             *       또한 Bind()함수로 바인딩 상태이어야 한다.
             * @param fd 소켓 FD
             * @param sockaddrIn
             * @return
             */
            static int Accept(int fd);

            /**
             * 원격지 접속을 요청합니다.
             * @param fd 바인딩 소켓, 혹은 바인딩 되지 않는 소켓
             * @param ip 원격지 IP
             * @param port 원격지 PORT
             * @return
             */
            static int Connect(int fd, string ip, string port);

            /**
             * 데이터를 송신합다니.
             * @param fd 바인딩 소켓, 혹은 바인딩 되지 않는 소켓
             * @param data 데이터
             * @param dataLen 데이터길이
             * @return
             */
            static int Send(int fd, const char *data, uint8_t dataLen);

            /**
             * 데이터를 수신합니다.
             * @param fd 바인딩 소켓, 혹은 바인딩 되지 않는 소켓
             * @param dataContainer 데이터를 저장할 변수
             * @param dataLen  데이터 수신 후 수신이 완료 되기 위한 데이터 길이
             * @return  -1 error, 0이면 session 종료, 0 보다 크면, 실제로 수신된 데이터 사이즈
             */
            static int Recv(int fd, char *dataContainer, std::uint8_t dataLen);

            /**
             * 주어진 ip, port정보를 이용하여, 접속요청 바로 시도 합니다.
             * 내부적으로 SetTimeoutOpt 자동으로 셋팅합니다.
             * @param ip destination ip
             * @param port  destination port
             * @param timeout SetTimeoutOpt 값
             * @return -1 이면 fail, 0보다 이상이면 socket fd값
             */
            static int RequestConnection(string ip, string port, std::uint8_t timeout);

            /**
             * 이미 Established 소켓으로 부터 데이터를 주고 받습니다.
             * @param socketFd  Establised socket
             * @param data  송신데이터
             * @param dataLen  송신데이터 길이
             * @param recvData 수신데이터
             * @param tilDataSize 수신데이터 완전한 데이터 사이즈
             * @return -1 이면 fail, 0> 이면 수신데이터 총 사이즈
             */
            static int Reply(int socketFd, const char *data, uint8_t dataLen, char *recvData, uint8_t tilDataSize);
        };

    } // doori
} // CommunicationMember

#endif //DOORI_PROJECT_TCPAPI_H
