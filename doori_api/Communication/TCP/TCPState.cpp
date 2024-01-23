//
// Created by jaeseong on 24. 1. 23.
//
#include "TCPState.h"

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

