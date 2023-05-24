//
// Created by jaeseong on 23. 3. 1.
//

#include "TcpApi.h"

namespace doori::CommunicationMember {

    TcpApi::TcpApi() : Common::Error(), mSocket(-1, SOCK_STATUS::CLOSED){

    }

    TcpApi::TcpApi(Socket socket) : Common::Error(), mSocket(socket) {

    }

    void TcpApi::CreateSocket() {

        auto fd= socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
            InjectBySystemcall();
            LOG(ERROR, "TCP socket fd, fail to open", Cause());
        }

        mSocket = Socket{fd, SOCK_STATUS::INIT};
    }

    void TcpApi::SetReuseOpt(const std::string& ip, const std::string& port) {

        uint16_t u16Temp = 0;
        u16Temp = atoi(port.c_str());

        struct sockaddr_in sockaddrIn{};
        sockaddrIn.sin_family = AF_INET;
        sockaddrIn.sin_port = htons(u16Temp);
        sockaddrIn.sin_addr.s_addr = ip.empty() ? INADDR_ANY : inet_addr(ip.c_str());

        if (setsockopt(mSocket.GetFd(), SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR, (void *) &sockaddrIn, sizeof(::sockaddr_in)) < 0) {
            InjectBySystemcall();
            LOG(ERROR, "TCP socket socket, fail to setsockopt:", Cause() );
        }

    }

    void TcpApi::Bind(const std::string& ip, const std::string& port) {

        char errorStr[1024] = {0};

        uint16_t u16Temp = 0;
        u16Temp = atoi(port.c_str());

        struct sockaddr_in sockaddrIn{};
        sockaddrIn.sin_family = AF_INET;
        sockaddrIn.sin_port = htons(u16Temp);
        sockaddrIn.sin_addr.s_addr = ip.empty() ? INADDR_ANY : inet_addr(ip.c_str());

        if (bind(mSocket.GetFd(), (struct sockaddr *) &sockaddrIn, sizeof(::sockaddr_in)) < 0) {
            InjectBySystemcall();
            LOG(ERROR, "TCP socket socket, fail to bind:", Cause());
        }

        mSocket.SetBitwise(SOCK_STATUS::BINDING);

    }

    void TcpApi::Listen(int backlogNum) {

        if( !mSocket.IsBitwise(SOCK_STATUS::BINDING) ) {
            InjectByClient("Socket is not Binding");
            return;
        }

        char errorStr[1024] = {0};
        if (listen(mSocket.GetFd(), backlogNum) != 0) {
            InjectBySystemcall();
            LOG(ERROR, "listen error:", Cause() );
        }

        mSocket.SetBitwise(SOCK_STATUS::LISTEN);

    }

    Socket TcpApi::Accept() {

        if( !mSocket.IsBitwise(SOCK_STATUS::BINDING) ) {
            InjectByClient("Socket is not Binding");
            return {-1, SOCK_STATUS::CLOSED};
        }

        if( !mSocket.IsBitwise(SOCK_STATUS::LISTEN) ) {
            InjectByClient("Socket is not Listen");
            return {-1, SOCK_STATUS::CLOSED};
        }

        int acptFd = 0;

        struct sockaddr_in sockaddrIn{};

        socklen_t len = sizeof(::sockaddr_in);

        // sockaddrIn 변수는 접속한 클라이언트 주소 저장용도로 사용가능하다.
        if ((acptFd = accept(mSocket.GetFd(), (struct sockaddr *) &sockaddrIn, &len)) == -1) {
            InjectBySystemcall();
            LOG(ERROR, "accept error:", Cause());
            return {-1, SOCK_STATUS::CLOSED};
        }

        Socket acceptSocket = Socket(acptFd, SOCK_STATUS::INIT);
        acceptSocket.SetBitwise(SOCK_STATUS::ACCEPT);
        acceptSocket.SetBitwise(SOCK_STATUS::ESTABLISED);

        return acceptSocket;
    }

    Socket TcpApi::Connect(const string &ip, const string &port) {

        if (ip.empty() || port.empty()) {
            InjectByClient("ip, port value is empty.");
            close(mSocket.GetFd());
            mSocket.SetBitwise(SOCK_STATUS::CLOSED);
            return mSocket;
        }

        uint16_t u16Temp = 0;
        u16Temp = atoi(port.c_str());

        struct sockaddr_in sockaddrIn{};
        sockaddrIn.sin_family = AF_INET;
        sockaddrIn.sin_port = htons(u16Temp);
        sockaddrIn.sin_addr.s_addr = inet_addr(ip.c_str());

        auto iRet = connect(mSocket.GetFd(), (struct sockaddr *)&sockaddrIn, sizeof(struct sockaddr_in));
        if (iRet < 0) {
            InjectBySystemcall();
            LOG(ERROR, "connect error:", Cause());
            close(mSocket.GetFd());
            mSocket.SetBitwise(SOCK_STATUS::CLOSED);
            return mSocket;
        }

        mSocket.SetBitwise(SOCK_STATUS::ESTABLISED);

        return mSocket;
    }

    void TcpApi::SetTimeoutOpt(std::uint8_t timeout) {

        /*
          소켓 설정을 논블로킹으로 셋팅함
          그래야, recv()함수에서 Timeout를 설정할 수 있음
        */
        int flags = fcntl(mSocket.GetFd(), F_GETFL, 0);  // 소켓의 플래그 값을 가져옴
        if( flags == -1 ) {
            InjectBySystemcall();
            LOG(ERROR, "fcntl error:", Cause());
        }

        auto iRet = fcntl(mSocket.GetFd(), F_SETFL, flags | O_NONBLOCK);  // 소켓의 플래그 값을 변경하여 넌블로킹 모드로 설정
        if( iRet == -1 ) {
            InjectBySystemcall();
            LOG(ERROR, "fcntl error:", Cause());
        }

        struct timeval tv{};
        tv.tv_sec  = timeout;
        tv.tv_usec = 0;

        if (setsockopt(mSocket.GetFd(), SOL_SOCKET, SO_RCVTIMEO, (void *) &tv, sizeof(struct timeval)) < 0) {
            InjectBySystemcall();
            LOG(ERROR, "setsockopt error:", Cause());
        }

    }

    Socket TcpApi::Connect(const string& ip, const string& port, std::uint8_t timeout) {
        char errorStr[1024] = {0};

        CreateSocket();
        if( !this->Status() ) {
            return Socket{-1, SOCK_STATUS::CLOSED};
        }

        SetTimeoutOpt(timeout);
        if( !this->Status() ) {
            return Socket{-1, SOCK_STATUS::CLOSED};
        }

        return Connect(ip, port);
    }

    Socket TcpApi::GetSocket() {
        return mSocket;
    }

} // CommunicationMember