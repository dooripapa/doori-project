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

namespace doori::api::Communication{

    class Socket;

    class TcpApi final : public Common::Error {

    public:

        TcpApi() = delete;
        explicit TcpApi(Socket &socket);
        TcpApi(const TcpApi& rhs) = delete;
        TcpApi(TcpApi&& rhs) noexcept = delete;
        TcpApi& operator=(const TcpApi& rhs) = delete;
        TcpApi& operator=(TcpApi&& rhs) = delete;

        /**
         * DOMAIN(AF_INET), mStreamProtocol(SOCK_STREAM), PROTOCOL(0) 기본설정으로 소켓 Endpoint 초기화함
         */
        void InitEndpoint();

        /**
         * SOL_SOCKET을 SO_REUSEPORT 와 SO_REUSEADDR 설정함
         * @note 이 함수가 호출되기전 InitEndpoint()호출로 반환된 소켓FD 필요함
         * @param ip가 ""이면, INADDR_ANY, 지정되면 해당IP로 바인딩 및 옵션이 셋팅됨
         * @param port 포트번호
         */
        void SetReuseOpt(const std::string& ip, const std::string& port);

        /**
         * SOL_SOCKET을 SO_REUSEPORT 와 SO_REUSEADDR 설정함
         * @note 이 함수가 호출되기전 InitEndpoint()호출로 반환된 소켓FD 필요함
         *       ip가 INADDR_ANY, 지정된 Port로 바인딩 및 옵션이 셋팅됨
         * @param port 포트번호
         */
        void SetReuseOpt(const std::string& port);

        /**
         * recv()함수에 Timeout 를 설정하기위해서 사전에 호출되어야 하는 함수
         * 소켓을 재설정함. SOL_SCOCKET, SO_RCVTIMEO를 설정함.
         * NON-BLOCKING
         * @param timeout Timeout값(second)
         */
        void SetTimeoutOpt(std::uint8_t timeout);

        /**
         * IP, PORT정보를 이용하여 바인딩처리함
         * @note 이 함수가 호출되기전 InitEndpoint()호출로 반환된 소켓FD 필요함
         * @param ip가 ""이면, INADDR_ANY, 지정되면 해당IP로 바인딩 및 옵션이 셋팅됨
         * @param port 포트번호
         */
        void Bind(const std::string& ip, const std::string& port);

        /**
         * IP, PORT정보를 이용하여 바인딩처리함 * @note 이 함수가 호출되기전 InitEndpoint()호출로 반환된 소켓FD 필요함
         *       ip가 INADDR_ANY, 지정된 Port로 바인딩
         * @param port 포트번호
         */
        void Bind(const std::string& port);

        /**
         * 입력 받은 소켓을 listen상태로 만든다.
         * @note 이 함수가 호출되기전 InitEndpoint()호출로 반환된 소켓FD 필요함
         *       또한 Bind()함수로 바인딩 상태이어야 한다.
         * @param backlogNum 접속 요청이 들어올 경우, 최대 대기 배열 수
         */
        void Listen(int backlogNum);

        /**
         * 입력 받은 소켓을 accept상태로 만든다. 수신요청이 오면, 또 다른 FD로 리턴되고, 해당 FD로 통신이 가능하다.
         * @note 이 함수가 호출되기전 InitEndpoint()호출로 반환된 소켓FD 필요함
         *       또한 Bind()함수로 바인딩 상태이어야 한다.
         */
        int Accept();

        /**
         * 원격지 접속을 요청합니다.
         * @param ip 원격지 IP
         * @param port 원격지 PORT
         */
        void Connect(const string &ip, const string &port);

        /**
         * 주어진 ip, port정보를 이용하여, 접속요청 바로 시도 합니다.
         * 내부적으로 SetTimeoutOpt 자동으로 셋팅합니다.
         * @param ip destination ip
         * @param port  destination port
         * @param timeout SetTimeoutOpt 값
         */
        void Connect(const string& ip, const string& port, std::uint8_t timeout);

        /**
         * TcpApi클래스의 api 의해 재설정된 Socket wrapper 객체를 리턴합니다.
         * @return Socker wrapper class
         */
        Socket & GetSocket() ;
    private:
        static bool IsValidIP(const string& ip);
        static bool IsValidPort(const string& port);
        Socket& mSocket;
    };

} // Communication

#endif //DOORI_PROJECT_TCPAPI_H
