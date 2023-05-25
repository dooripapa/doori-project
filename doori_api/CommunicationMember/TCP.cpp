//
// Created by jaeseong on 23. 1. 31.
//

#include "TCP.h"
#include "Common/Log.h"

namespace doori::CommunicationMember{

    int TCP::Create(IIPCBuilder &builder) {

        if( !builder.EstablishTopologies() )
        {
            LOG(ERROR, "Establish Topologies,call exit()");
            std::exit(-1);
        }

        mResource = builder.GetListenRsc();

        return 0;
    }

    unique_ptr<doori::CommunicationMember::IIPCTopology> TCP::GetIPC() noexcept {
        return std::move(mResource);
    }

}

