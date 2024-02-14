//
// Created by jslee on 24. 1. 7.
//

#include "TCPClose.h"
#include "TCPNode.h"
#include "Communication/TcpApi.h"

namespace doori {
    namespace api {
        namespace Communication {
            namespace TCP {

                //명시적으로 사전 초기화.
                TCPClose TCPClose::myInstance;

                void TCPClose::close(TCPNode *node) {
                    LOG(INFO, "Socket is already closed");
                }

                void TCPClose::establish(TCPNode *node) {

                    int ret = 0;
                    auto sock = node->getSock();
                    ::close(sock);

                    auto source = node->getSource();
                    auto dest = node->getRemote();

                    sock = TcpApi::connect(source.ip, source.port, dest.ip, dest.port);

                    if (sock==-1) {
                        LOG(ERROR, "TcpApi::connect source[%t:%t] destination[%t:%t]", source.ip, source.port, dest.ip,
                            dest.port);
                        return;
                    }
                    return;
                }

                void TCPClose::listen(TCPNode *node) {
                    LOG(INFO, "TCP close(listen)");
                }

                TCPState *TCPClose::Instance() {
                    return &myInstance;
                }
            } // TCP
        } // Communication
    } // api
} // doori