//
// Created by jaeseong on 23. 1. 31.
//

#include "TCPBuilder.h"
#include "TcpApi.h"
#include "Common/Log.h"

namespace doori::CommunicationMember {

    TCPBuilder::~TCPBuilder() {

    }

    TCPBuilder::TCPBuilder(TOPOLOGY_TYPE type, string ip, string port) : mListenFd(-1){
        mType = type;
        switch (type) {
            case CLIENT:
                mDestIp = ip;
                mDestPort = port;
                mSourceIp.clear();
                mSourceIp.clear();
                break;
            case SERVER:
                mDestIp.clear();
                mDestPort.clear();
                mSourceIp = ip;
                mSourcePort = port;
                break;
        }
    }

    TCPBuilder::TCPBuilder(string destination_ip, string destination_port, string source_ip, string source_port)
            : mType(CLIENT), mListenFd(-1), mDestIp(destination_ip), mDestPort(destination_port), mSourceIp(source_ip),
              mSourcePort(source_port) {
    }

    unique_ptr<IIPCTopology> TCPBuilder::GetListenRsc() {
        return unique_ptr<IIPCTopology>();
    }

    bool TCPBuilder::EstablishTopologies() {
        return false;
    }

} // doori