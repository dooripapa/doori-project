//
// Created by jaeseong on 23. 3. 1.
//

#include <regex>
#include "TcpApi.h"

namespace doori::api::Communication{

    TcpApi::TcpApi(Socket &socket) : Common::Error(), mSocket(socket) {

    }

    void TcpApi::SetReuseOpt(const std::string& ip, const std::string& port) {

        if(!IsValidIP(ip)) {
            LoggingByClientError("ip is invalid:");
            return;
        }

        if(!IsValidPort(port)) {
            LoggingByClientError("port invalid");
            return;
        }

        int cPort;
        std::size_t pos{};
        try{
            cPort = std::stoi(port, &pos);
        } catch (const std::exception &ex ){
            LOG(ERROR,"stoi exception:", ex.what());
            LoggingByClientError("stoi exception");
            return;
        }

        struct sockaddr_in sockaddrIn{};
        sockaddrIn.sin_family = AF_INET;
        sockaddrIn.sin_port = htons(cPort);
        sockaddrIn.sin_addr.s_addr = inet_addr(ip.c_str());

        if (setsockopt(mSocket.GetFd(), SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR, (void *) &sockaddrIn, sizeof(::sockaddr_in)) < 0) {
            LoggingBySystemcallError("setsockopt");
            return;
        }

        mSocket.SetOptBitwise(SOCK_OPT::REUSE);

    }

    void TcpApi::SetReuseOpt(const string &port) {

        if(!IsValidPort(port)) {
            LoggingByClientError("port invalid");
            return;
        }

        int cPort;
        std::size_t pos{};
        try{
            cPort = std::stoi(port, &pos);
        } catch (const std::exception &ex ){
            LOG(ERROR,"stoi exception:", ex.what());
            LoggingByClientError("stoi exception");
            return;
        }

        struct sockaddr_in sockaddrIn{};
        sockaddrIn.sin_family = AF_INET;
        sockaddrIn.sin_port = htons(cPort);
        sockaddrIn.sin_addr.s_addr = INADDR_ANY;

        if (setsockopt(mSocket.GetFd(), SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR, (void *) &sockaddrIn, sizeof(::sockaddr_in)) < 0) {
            LoggingBySystemcallError("setsockopt");
            return;
        }

        mSocket.SetOptBitwise(SOCK_OPT::REUSE);


    }

    void TcpApi::Bind(const std::string& ip, const std::string& port) {

        if(!IsValidIP(ip)) {
            LoggingByClientError("ip is invalid:");
            return;
        }

        if(!IsValidPort(port)) {
            LoggingByClientError("port invalid");
            return;
        }

        int cPort;
        std::size_t pos{};
        try{
            cPort = std::stoi(port, &pos);
        } catch (const std::exception &ex ){
            LOG(ERROR,"stoi exception:", ex.what());
            LoggingByClientError("stoi exception");
            return;
        }

        struct sockaddr_in sockaddrIn{};
        sockaddrIn.sin_family = AF_INET;
        sockaddrIn.sin_port = htons(cPort);
        sockaddrIn.sin_addr.s_addr = ip.empty() ? INADDR_ANY : inet_addr(ip.c_str());

        if (bind(mSocket.GetFd(), (struct sockaddr *) &sockaddrIn, sizeof(::sockaddr_in)) < 0) {
            LoggingBySystemcallError("TCP socket socket, fail to bind");
            return;
        }

        mSocket.SetBitwise(SOCK_STATUS::BINDING);

    }

    void TcpApi::Bind(const string &port) {

        if(!IsValidPort(port)) {
            LoggingByClientError("port invalid");
            return;
        }

        int cPort;
        std::size_t pos{};
        try{
            cPort = std::stoi(port, &pos);
        } catch (const std::exception &ex ){
            LOG(ERROR,"stoi exception:", ex.what());
            LoggingByClientError("stoi exception");
            return;
        }

        struct sockaddr_in sockaddrIn{};
        sockaddrIn.sin_family = AF_INET;
        sockaddrIn.sin_port = htons(cPort);
        sockaddrIn.sin_addr.s_addr = INADDR_ANY;

        if (bind(mSocket.GetFd(), (struct sockaddr *) &sockaddrIn, sizeof(::sockaddr_in)) < 0) {
            LoggingBySystemcallError("TCP socket socket, fail to bind");
            return;
        }

        mSocket.SetBitwise(SOCK_STATUS::BINDING);
    }


    void TcpApi::Listen(int backlogNum) {

        if( !mSocket.IsBitwise(SOCK_STATUS::BINDING) ) {
            InjectedByClientError("Socket is not Binding");
            return;
        }

        if (listen(mSocket.GetFd(), backlogNum) != 0) {
            InjectedBySystemcallError();
            LOG(ERROR, "listen error:", Cause() );
        }

        mSocket.SetBitwise(SOCK_STATUS::LISTEN);

    }

    int TcpApi::Accept() {

        if( !mSocket.IsBitwise(SOCK_STATUS::BINDING) ) {
            InjectedByClientError("Socket is not Binding");
            return -1;
        }

        if( !mSocket.IsBitwise(SOCK_STATUS::LISTEN) ) {
            InjectedByClientError("Socket is not Listen");
            return -1;
        }

        int acptFd = 0;

        struct sockaddr_in sockaddrIn{};

        socklen_t len = sizeof(struct sockaddr_in);

        if(mSocket.IsOptBitwise(SOCK_OPT::RCVTIMEO)) {

            struct timeval tv{};
            auto socklen =sizeof(struct timeval);
            if (getsockopt(mSocket.GetFd(), SOL_SOCKET, SO_RCVTIMEO, (void *) &tv,
                           reinterpret_cast<socklen_t *>(&socklen)) < 0) {
                LoggingBySystemcallError("getsockopt error");
                return -1;
            }

            while(true)
            {
                acptFd = accept(mSocket.GetFd(), nullptr, nullptr);
                if ( acptFd == -1) {
                    if (errno == EWOULDBLOCK || errno == EAGAIN) {
                        // No incoming connections at the moment
                        // Handle other tasks or sleep/wait
                        LOG(INFO, "Non-blocking Mode, timeout --> sleep(", tv.tv_sec,")");
                        sleep(tv.tv_sec);
                        continue;
                    } else {
                        LoggingBySystemcallError("Non-blocking accept error");
                        return -1;
                    }
                } else {
                    LOG(INFO, "Accept Socket[", acptFd, "]" );
                    break;
                }
            }

        }
        else {
            /**
             * sockaddrIn 변수는 접속한 클라이언트 주소 저장용도로 사용가능하다.
             * 현재는 의미 없음
             * @todo 수신ip 정보를 get하는 기능추가
             */
            if ((acptFd = accept(mSocket.GetFd(), (struct sockaddr *) &sockaddrIn, &len)) == -1) {
                LoggingBySystemcallError("Blocking accept error");
                return -1;
            }
        }

        return acptFd;
    }

    void TcpApi::Connect(const string &ip, const string &port) {

        std::size_t pos{};

        int cPort;
        try{
            cPort = std::stoi(port, &pos);
        } catch (const std::exception &ex ){
            LOG(ERROR,"stoi exception:", ex.what());
            LoggingByClientError("stoi exception");
            return;
        }

        if( 65535 < cPort ) {
            LoggingByClientError("port range over > 65535");
            return;
        }

        if(!mSocket.IsBitwise(SOCK_STATUS::INIT))
        {
            LoggingByClientError("socket not init.");
            return;
        }

        if (ip.empty() || port.empty()) {
            LoggingByClientError("ip, port value is empty.");
            close(mSocket.GetFd());
            mSocket.SetBitwise(SOCK_STATUS::CLOSED);
            return;
        }

        struct sockaddr_in sockaddrIn{};
        sockaddrIn.sin_family = AF_INET;
        sockaddrIn.sin_port = htons(cPort);
        sockaddrIn.sin_addr.s_addr = inet_addr(ip.c_str());

        auto ret = connect(mSocket.GetFd(), (struct sockaddr *)&sockaddrIn, sizeof(struct sockaddr_in));
        if (ret < 0) {

            int errorValue = 0;
            socklen_t len = sizeof(errorValue);
            if( getsockopt(mSocket.GetFd(), SOL_SOCKET, SO_ERROR, &errorValue, &len ) < 0 ) {
                LoggingBySystemcallError("connect() error");
                return;
            }

            if (errorValue != 0) {
                LOG(ERROR,"Error Value:", errorValue);
                LoggingByClientError("connect() error");
                return;
            }
        }

        LOG(INFO, "connected Socket[", mSocket.GetFd(), "]" );
    }

    void TcpApi::SetTimeoutOpt(std::uint8_t timeout) {

        if(!mSocket.IsBitwise(SOCK_STATUS::INIT))
        {
            InjectedByClientError("socket not init.");
            return;
        }

        /*
          소켓 설정을 논블로킹으로 셋팅함
          그래야, recv()함수에서 Timeout를 설정할 수 있음
        */
        int flags = fcntl(mSocket.GetFd(), F_GETFL, 0);  // 소켓의 플래그 값을 가져옴
        if( flags == -1 ) {
            LoggingBySystemcallError("fcntl error");
            return;
        }

        auto iRet = fcntl(mSocket.GetFd(), F_SETFL, flags | O_NONBLOCK);  // 소켓의 플래그 값을 변경하여 넌블로킹 모드로 설정
        if( iRet == -1 ) {
            LoggingBySystemcallError("fcntl error");
            return;
        }

        struct timeval tv{};
        tv.tv_sec  = timeout;
        tv.tv_usec = 0;

        if (setsockopt(mSocket.GetFd(), SOL_SOCKET, SO_RCVTIMEO, (void *) &tv, sizeof(struct timeval)) < 0) {
            LoggingBySystemcallError("setsockopt error");
            return;
        }

        mSocket.SetOptBitwise(SOCK_OPT::RCVTIMEO);
    }

    void TcpApi::Connect(const string& ip, const string& port, std::uint8_t timeout) {

        Connect(ip, port);
    }

    void TcpApi::InitEndpoint() {

        if(!mSocket.Init()) {
            LoggingByClientError("InitEndpoint error");
            mSocket = Socket{-1, SOCK_STATUS::CLOSED};
            return;
        }

        this->AsSuccess();

    }

    Socket & TcpApi::GetSocket() {
        return mSocket;
    }

    /**
     * ip 문자열을 검증합니다. 유효하지 않으면 false
     * @return
     */
    bool TcpApi::IsValidIP(const string &ip) {
        constexpr auto checkIpRegex ="^((\\d{1,3})\\.){3}(\\d{1,3})$";
        std::regex regex(checkIpRegex);
        return std::regex_match(ip, regex);
    }

    /**
     * port 문자열을 검증합니다. 유효하지 않으면 false
     * 1024 ~ 65535 사이 문자열이어야 합니다.
     * @return
     */
    bool TcpApi::IsValidPort(const string &port) {
        constexpr auto checkPortRegex ="^([1-9][0-9]{3,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$";
        std::regex regex(checkPortRegex);
        return std::regex_match(port, regex);
    }


} // Communication