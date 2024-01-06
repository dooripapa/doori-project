//
// Created by jslee on 24. 1. 6.
//

#include <string>
#include "TCPtopologyNode.h"

namespace doori::api::Communication::TCP {
    long TCPtopologyNode::Send(const std::string &data) const {
        return 0;
    }

    long TCPtopologyNode::Recv(std::string &data, std::uint32_t utilSize) const {
        return 0;
    }

    int TCPtopologyNode::Close() {
        return 0;
    }
} // doori