//
// Created by jaeseong on 24. 3. 20.
//

#include "TCP.h"

TCP::Node & TCP::getTopologyNode() {
    return node;
}

/**
 * 입력받은 ip가 유효한지 체크 합니다.
 * @param ip
 * @return true / false
 */
[[maybe_unused]] bool TCP::Node::IsValidIP(const std::string &ip) {
    constexpr auto checkIpRegex = R"(^((\d{1,3})\.){3}(\d{1,3})$)";
    std::regex regex(checkIpRegex);
    return std::regex_match(ip, regex);
}

/**
 * 입력받은 port가 유효한지 체크 합니다.
 * @param port
 * @return true / false
 */
[[maybe_unused]] bool TCP::Node::IsValidPort(const std::string &port) {
    constexpr auto checkPortRegex = R"(^([1-9][0-9]{3,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$)";
    std::regex regex(checkPortRegex);
    return std::regex_match(port, regex);
}
