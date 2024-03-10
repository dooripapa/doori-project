//
// Created by jaeseong on 24. 1. 10.
//

#ifndef DOORI_PROJECT_FROMSTRATEGIES_H
#define DOORI_PROJECT_FROMSTRATEGIES_H

#include "TcpApi.h"

namespace doori::api::Communication{

    /**
     * @note IPC(shared memory, tcp, udp ...) 에서, source 지역에서 초기화 및 처리할 로직은 이 클래스에 다 정의해서 사용한다.
     */
    class FromStrategies {
    public:
        int operator()(TCP::TCPNode &node);
/*
        int operator()(SHM::ShmNode &node);
        int operator()(UDP::UDPNode &node);
*/

    private:
        static bool IsValidIP(const string &ip);
        static bool IsValidPort(const string &port);
    };

}



#endif //DOORI_PROJECT_FROMSTRATEGIES_H
