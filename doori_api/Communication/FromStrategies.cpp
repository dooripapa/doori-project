//
// Created by jaeseong on 24. 1. 10.
//

#include <regex>
#include "FromStrategies.h"

namespace doori::api::Communication {

    int FromStrategies::operator()(TCP::TCPNode &node) {

        auto address = node.getSource();

        auto fd= socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
            LOG(ERROR, "socket error");
            return false;
        }

        if (!IsValidIP(address.ip)) {
            LOG(ERROR, "invalid ip");
            return -1;
        }

        if (!IsValidPort(address.port)) {
            LOG(ERROR, "invalid port");
            return -1;
        }

        int cPort;
        std::size_t pos{};
        try {
            cPort = std::stoi(address.port, &pos);
        } catch (const std::exception &ex) {
            LOG(ERROR, "stoi exception:", ex.what());
            return -1;
        }

        struct sockaddr_in sockaddrIn{};
        sockaddrIn.sin_family = AF_INET;
        sockaddrIn.sin_port = htons(cPort);
        sockaddrIn.sin_addr.s_addr = inet_addr(address.ip.c_str());

        if (setsockopt(node.getSock(), SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR, (void *) &sockaddrIn,
                       sizeof(::sockaddr_in)) < 0) {
            LOG(ERROR, "failed to setsockopt SO_REUSEPORT SO_REUSEADDR");
            return -1;
        }

        if (bind(node.getSock(), (struct sockaddr *) &sockaddrIn, sizeof(::sockaddr_in)) < 0) {
            LOG(ERROR, "TCP socket socket, fail to bind");
            return -1;
        }

        if (listen(node.getSock(), 10) != 0) {
            LOG(ERROR, "listen error");
            return -1;
        }

        return 0;
    }

    /**
     * 입력받은 ip가 유효한지 체크 합니다.
     * @param ip
     * @return true / false
     */
    bool FromStrategies::IsValidIP(const string &ip) {
        constexpr auto checkIpRegex = R"(^((\d{1,3})\.){3}(\d{1,3})$)";
        std::regex regex(checkIpRegex);
        return std::regex_match(ip, regex);
    }

    /**
     * 입력받은 port가 유효한지 체크 합니다.
     * @param port
     * @return true / false
     */
    bool FromStrategies::IsValidPort(const string &port) {
        constexpr auto checkPortRegex = R"(^([1-9][0-9]{3,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$)";
        std::regex regex(checkPortRegex);
        return std::regex_match(port, regex);
    }
}

