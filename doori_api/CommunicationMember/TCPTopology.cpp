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
        int nMsgLen = data.size();
        int nRet = TcpApi::Send(mDataPassage, data.serialize(), nMsgLen);
        if (nRet != 0)
        {
            LOG(ERROR, "TcpApi::Send() error");
            return -1;
        }
        return 0;
    }

    int TCPTopology::Recv(DataStream::IStream &data) {
        int nRet = TcpApi::Recv(mDataPassage, data.serialize(), HEADER_LEN);
        if (nRet != 0)
        {
            LOG(ERROR, "TcpApi::Recv() error");
            return -1;
        }
        return 0;
    }

} // CommunicationMember