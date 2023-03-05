//
// Created by jaeseong on 23. 1. 31.
//

#include "TCP.h"
#include "Common/Log.h"

namespace doori::CommunicationMember{

    TCP::~TCP() {
    }

    TCP::TCP() : mConnection(-1) {

    }

    int TCP::Create(IIPCBuilder *builder) {
        auto from = builder->BindFrom();
        if( from == -1 )
        {
            LOG(ERROR, "Binding error, From endpoint");
            return -1;
        }

        auto to = builder->BindTo();
        if( to == -1 )
        {
            LOG(ERROR, "Binding error, To endpoint");
            return -1;
        }

        try {
            mConnection = builder->EstablishTopologies();
        }catch (exception e) {
            LOG(ERROR, "Establish Topologies,call exit()", ":", e.what());
            std::exit(-1);
        }

        return mConnection;
    }

    ITopology *TCP::GetIPC() noexcept {
        return new TCPTopology{mConnection};
    }

}

