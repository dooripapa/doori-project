//
// Created by jaeseong on 24. 1. 23.
//
#include <netinet/in.h>
#include <regex>
#include <arpa/inet.h>
#include <bits/fcntl.h>
#include <fcntl.h>
#include "TCPState.h"
#include "Common/Log.h"

namespace doori::api::Communication::TCP {

    void TCPState::establish(TCPNode *node) {

    }

    void TCPState::close(TCPNode *node) {

    }

    void TCPState::wait(TCPNode *node) {

    }

    void TCPState::changeState(TCPNode *node, TCPState *state) {
        node->changeState(state);
    }

    int TCPState::binding(int sock, const string& ip, const string& port) {

        auto cPort = checkAddress(ip, port);
        if (cPort == -1)
            return cPort;

        struct sockaddr_in sockaddrIn{};
        sockaddrIn.sin_family = AF_INET;
        sockaddrIn.sin_port = ::htons(cPort);
        sockaddrIn.sin_addr.s_addr = ip.empty() ? INADDR_ANY : ::inet_addr(ip.c_str());;

        if (bind(sock, (struct sockaddr *) &sockaddrIn, sizeof(::sockaddr_in)) < 0) {
            return -1;
        }

        return sock;
    }

    bool TCPState::IsValidIP(const string &ip) {
        constexpr auto checkIpRegex ="^((\\d{1,3})\\.){3}(\\d{1,3})$";
        std::regex regex(checkIpRegex);
        return std::regex_match(ip, regex);
    }

    bool TCPState::IsValidPort(const string &port) {
        constexpr auto checkPortRegex ="^([1-9][0-9]{3,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$";
        std::regex regex(checkPortRegex);
        return std::regex_match(port, regex);
    }

    int TCPState::listen(int sock, int backlogNum) {
        if (::listen(sock, backlogNum) != 0) {
            LOG(ERROR, "listen error");
            return -1;
        }
        return sock;
    }

    bool TCPState::reuseSockOpt(int sock, const string &ip, const string &port) {

        auto cPort = checkAddress(ip, port);
        if (cPort == -1)
            return cPort;

        struct sockaddr_in sockaddrIn{};
        sockaddrIn.sin_family = AF_INET;
        sockaddrIn.sin_port = htons(cPort);
        sockaddrIn.sin_addr.s_addr = inet_addr(ip.c_str());

        if (::setsockopt(sock, SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR, (void *) &sockaddrIn, sizeof(::sockaddr_in)) < 0) {
            LOG(ERROR, "error setsockopt()");
            return -1;
        }
        return sock;
    }

    int TCPState::checkAddress(const string &ip, const string &port) {

        if(!IsValidIP(ip)) {
            LOG(ERROR, "ip is invalid:");
            return -1;
        }

        if(!IsValidPort(port)) {
            LOG(ERROR, "port is invalid:");
            return -1;
        }

        int cPort;
        std::size_t pos{};
        try{
            cPort = std::stoi(port, &pos);
        } catch (const std::exception &ex ){
            LOG(ERROR,"stoi exception:", ex.what());
            return -1;
        }
        return cPort;
    }

    bool TCPState::setNonblockingOpt(int sock, std::uint8_t timeout) {

        /*
          소켓 설정을 논블로킹으로 셋팅함
          그래야, recv()함수에서 Timeout를 설정할 수 있음
        */
        int flags = ::fcntl(sock, F_GETFL, 0);  // 소켓의 플래그 값을 가져옴
        if( flags == -1 ) {
            LOG(ERROR, "fcntl(F_GETFL) error");
            return false;
        }

        auto iRet = ::fcntl(sock, F_SETFL, flags | O_NONBLOCK);  // 소켓의 플래그 값을 변경하여 넌블로킹 모드로 설정
        if( iRet == -1 ) {
            LOG(ERROR, "fcntl(F_SETFL, flags | O_NONBLOCK) error");
            return false;
        }

        struct timeval tv{};
        tv.tv_sec  = timeout;
        tv.tv_usec = 0;

        if (::setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (void *) &tv, sizeof(struct timeval)) < 0) {
            LOG(ERROR, "setsockopt error");
            return false;
        }
        return true;
    }
}

