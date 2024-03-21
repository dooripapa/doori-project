//
// Created by jaeseong on 24. 3. 20.
//

#ifndef IPC_TCP_H
#define IPC_TCP_H


#include <regex>
#include "string"

class TCP {

private:
    struct Node{
        std::string ip;
        std::string port;

        /**
         * 입력받은 ip가 유효한지 체크 합니다.
         * @param ip
         * @return true / false
         */
        [[maybe_unused]] static bool IsValidIP(const std::string &ip);

        /**
         * 입력받은 port가 유효한지 체크 합니다.
         * @param port
         * @return true / false
         */
        [[maybe_unused]] static bool IsValidPort(const std::string &port);
    };

public:
    Node & getTopologyNode();

private:
    Node node;
};


#endif //IPC_TCP_H
