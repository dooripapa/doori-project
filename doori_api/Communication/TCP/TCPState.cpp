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
}

