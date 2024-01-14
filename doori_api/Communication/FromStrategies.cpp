//
// Created by jaeseong on 24. 1. 10.
//

#include "FromStrategies.h"

namespace doori::api::Communication {

    int FromStrategies::operator()(TCP::TCPNode &node) {

        Socket sock;
        auto ret = sock.Init();

        if (!ret) {
            LOG(ERROR, "Socket Init, error");
            return -1;
        }

        TcpApi tcpApi{sock};
        if(!tcpApi.Status()){


        }

        tcpApi.Bind(node.getSource().ip, node.getSource().port);


        return 0;
    }
}

