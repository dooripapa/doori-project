//
// Created by jaeseong on 23. 2. 7.
//

#include "TCPTopology.h"
#include "Common/Log.h"
#include "TcpApi.h"

namespace doori::CommunicationMember {

    TCPTopology::~TCPTopology() {

    }

    TCPTopology::TCPTopology(int dataPassage) : mDataPassage(dataPassage){
    }

    int TCPTopology::Send(const DataStream::IStream &data) {
        return 0;
    }

    int TCPTopology::Recv(DataStream::IStream &data) {
        return 0;
    }
} // CommunicationMember