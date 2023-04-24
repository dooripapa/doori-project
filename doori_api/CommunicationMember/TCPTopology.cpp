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

    int TCPTopology::Send(const string &data) {

        int nMsgLen = data.size();

        int nRet = TcpApi::Send(mDataPassage, data.c_str(), nMsgLen);
        if (nRet != 0)
        {
            LOG(ERROR, "TcpApi::Send() error");
            return -1;
        }

        return 0;
    }

    int TCPTopology::Recv(string &data, std::uint8_t tilSize) {

        std::unique_ptr<char []> container = std::make_unique<char []>(tilSize);

        int nRet = TcpApi::Recv(mDataPassage, container.get(), tilSize);
        if (nRet != 0)
        {
            LOG(ERROR, "TcpApi::Recv() error");
            return -1;
        }

        data = container.get();

        return 0;
    }

} // CommunicationMember