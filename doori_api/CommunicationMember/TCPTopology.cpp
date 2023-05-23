//
// Created by jaeseong on 23. 2. 7.
//

#include "TCPTopology.h"
#include "Common/Log.h"
#include "TcpApi.h"

namespace doori::CommunicationMember {

    TCPTopology::~TCPTopology() {

    }

    /* 의존성 주입 */
    TCPTopology::TCPTopology(Socket socket) : mTcpApi(socket){

    }

    int TCPTopology::Send(const string &data) {

        int nMsgLen = data.size();

        mTcpApi.Send(data.c_str(), nMsgLen);
        if (!mTcpApi.Status()) {
            LOG(ERROR, "TcpApi.Send() error");
            return -1;
        }

        return 0;
    }

    int TCPTopology::Recv(string &data, std::uint8_t tilSize) {

        std::unique_ptr<char []> container = std::make_unique<char []>(tilSize);

        mTcpApi.Recv(container.get(), tilSize);
        if ( !mTcpApi.Status() ) {
            LOG(ERROR, "TcpApi.Recv() error");
            return -1;
        }
        data = container.get();

        return 0;
    }

} // CommunicationMember