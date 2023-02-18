//
// Created by jaeseong on 23. 2. 7.
//

#include <sys/socket.h>
#include <netinet/in.h>

#include <utility>
#include "TCPConnection.h"
#include "Common/Log.h"

namespace doori::CommunicationMember {
    int TCPConnection::WaitFor() {

        char errorStr[1024] = {0};

        //source 바인딩 IP, PORT
        DataStream::Json networkTopologiesInfo = mFromInfo.getTopoloiesInfo();
        if (networkTopologiesInfo["ip"].toString().empty() || networkTopologiesInfo["port"].toString().empty()) {
            LOG(ERROR, "Topologies Bad");
            std::abort();
        }
        string ip = networkTopologiesInfo["ip"].toString();
        string port = networkTopologiesInfo["port"].toString();

        auto bindingSock = Bind(ip, port);
        if (bindingSock == -1) {
            LOG(ERROR, "TCP socket fd, fail to open");
            std::abort();
        }

        struct sockaddr_in sockadd_source{};
        Set_sockaddr_in(sockadd_source, ip, port);

        if (listen(bindingSock, 1) < 0) {
            LOG(ERROR, "TCP socket fd, fail to listen:", strerror_r(errno, errorStr, sizeof(errorStr)));
            close(bindingSock);
            return -1;
        }

        socklen_t len = sizeof(struct sockaddr_in);

        if (accept(bindingSock, (struct sockaddr *) &sockadd_source, &len) < 0) {
            LOG(ERROR, "TCP socket fd, fail to accept:", strerror_r(errno, errorStr, sizeof(errorStr)));
            close(bindingSock);
            return -1;
        }
        return bindingSock;
    }

    TCPConnection::TCPConnection(CommunicationMember::Endpoint From, CommunicationMember::Endpoint To)
    : mFromInfo{std::move(From)}, mToInfo{std::move(To)} {

    }

    int TCPConnection::ConnectTo() {
        char errorStr[1024] = {0};

        //source 바인딩 IP, PORT
        DataStream::Json networkTopologiesInfo = mFromInfo.getTopoloiesInfo();
        if (networkTopologiesInfo["ip"].toString().empty() || networkTopologiesInfo["port"].toString().empty()) {
            LOG(ERROR, "Topologies Bad");
            std::abort();
        }
        string ip = networkTopologiesInfo["ip"].toString();
        string port = networkTopologiesInfo["port"].toString();

        auto bindingSock = Bind(ip, port);
        if (bindingSock == -1) {
            LOG(ERROR, "TCP socket fd, fail to open");
            std::abort();
        }

        //목적지 IP, PORT
        struct sockaddr_in sockadd_dest{};

        networkTopologiesInfo = mToInfo.getTopoloiesInfo();
        if (networkTopologiesInfo["ip"].toString().empty() || networkTopologiesInfo["port"].toString().empty()) {
            LOG(ERROR, "Topologies Bad");
            std::abort();
        }

        ip = networkTopologiesInfo["ip"].toString();
        port = networkTopologiesInfo["port"].toString();

        Set_sockaddr_in(sockadd_dest, ip, port);

        auto iRet = connect(bindingSock, (struct sockaddr *) &sockadd_dest, sizeof(struct sockaddr_in));
        if (iRet < 0) {
            LOG(ERROR, "TCP socket fd, fail to connect:", strerror_r(errno, errorStr, sizeof(errorStr)));
            close(bindingSock);
            return -1;
        }
        return bindingSock;
    }

    int TCPConnection::Send(int rscFd, const DataStream::IStream &data) {
        return 0;
    }

    int TCPConnection::Recv(int rscFd, DataStream::IStream &data) {
        return 0;
    }

    auto TCPConnection::Set_sockaddr_in(struct sockaddr_in &addr, const string &ip, const string &port) -> void {
        uint16_t u16Temp = atoi(port.c_str());

        addr.sin_family = AF_INET;
        addr.sin_port = htons(u16Temp);
        addr.sin_addr.s_addr = ip.c_str() ? inet_addr(ip.c_str()) : INADDR_ANY;
    }

    int TCPConnection::Bind(const string &ip, const string &port) {
        struct sockaddr_in addr{};

        Set_sockaddr_in(addr, ip, port);

        char errorStr[1024] = {0};
        auto socketFd = socket(AF_INET, SOCK_STREAM, 0);
        if (socketFd < 0) {
            LOG(ERROR, "TCP socket fd, fail to open");
            return -1;
        }

        if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, (void *) &addr, sizeof(struct sockaddr_in)) < 0) {
            LOG(ERROR, "TCP socket fd, fail to setsockopt");
            return -1;
        }

        if (bind(socketFd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) < 0) {
            LOG(ERROR, "TCP socket fd, fail to bind:", strerror_r(errno, errorStr, sizeof(errorStr)));
            return -1;
        }

        return socketFd;
    }

} // CommunicationMember