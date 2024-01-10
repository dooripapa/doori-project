//
// Created by KOSCOM on 2024-01-10.
//

#ifndef DOORI_PROJECT_HANDLER_H
#define DOORI_PROJECT_HANDLER_H

#include "TCP/TCPNode.h"

using namespace doori::api::Communication::TCP;

namespace doori::api::Communication {
    class Handler {
        int recv(const TCPNode &node, std::string &buffe);

        int send(const TCPNode &node, std::string buffer, int bufferSize);
    };
} // doori

#endif //DOORI_PROJECT_HANDLER_H
