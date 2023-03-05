//
// Created by jaeseong on 23. 1. 31.
//

#ifndef DOORI_PROJECT_TCPBUILDER_H
#define DOORI_PROJECT_TCPBUILDER_H

#include "CommunicationMember/IIPCBuilder.h"
#include "Endpoint.h"
#include <string>

using namespace std;

namespace doori::CommunicationMember {

    class TCPBuilder : public IIPCBuilder {
    public:

        TCPBuilder() = delete;

        virtual ~TCPBuilder();

        /**
         * 초기화를 합니다.
         * @param type  SERVER | CLIENT 속성
         * @param ip TOPOLOGY_TYPE SERVER이면, 바인딩 ip, CLIENT이면 원격지 ip
         * @param port TOPOLOGY_TYPE SERVER이면, 바인딩 port , CLIENT이면 원격지 port
         */
        TCPBuilder(TOPOLOGY_TYPE type, string ip, string port);

        /**
         * 초기화를 합니다. TOPOLOGY가 CLIENT 설정됩니다.
         * @param destination_ip : 접속 원격지 IP
         * @param destination_port : 접속 원격지 PORT
         * @param source_ip : 원격지 접속시, 지정된 IP로 바인딩 객체와 통신을 강제화 합니다.
         * @param source_port : 원격지 접속시, 지정된 PORT로 바인딩 객체와 통신을 강제화 합니다.
         */
        TCPBuilder(string destination_ip, string destination_port, string source_ip, string source_port);

        int GetListenRsc() override;

        int BindFrom() override;

        int BindTo() override;

        int EstablishTopologies() override;

    private:
        string mDestIp;
        string mDestPort;
        string mSourceIp;
        string mSourcePort;
        int mListenFd;
        int mSourceFd;
        TOPOLOGY_TYPE mType;
        const int kBackLog=10;
    };

} // doori

#endif //DOORI_PROJECT_TCPBUILDER_H
