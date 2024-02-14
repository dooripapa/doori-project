//
// Created by jslee on 24. 1. 7.
//

#include "TCPEstablish.h"
namespace doori::api::Communication::TCP {


    void TCPEstablish::establish(TCPNode *node) {
        LOG(INFO, "Already status Established ");
    }

    void TCPEstablish::close(TCPNode *node) {
        node->close();
        LOG(INFO, "Node is closed");
    }

    void TCPEstablish::listen(TCPNode *node) {
        node->close();
        PLOG(INFO, "Socket FD[%t]", node->getSock() );

        node->listen()
    }
} // doori