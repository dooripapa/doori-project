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
             * DOMAIN(AF_INET), TYPE(SOCK_STREAM), PROTOCOL(0) 기본설정으로 소켓 file descriptor리턴
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
        };

    } // doori
} // CommunicationMember

#endif //DOORI_PROJECT_TCPAPI_H
