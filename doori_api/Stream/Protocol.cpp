//
// Created by jslee on 23. 4. 29.
//

#include "Protocol.h"

namespace doori::StreamInterface {

    Protocol *Protocol::GetInstance() {

        if (gInstance == nullptr)
            gInstance = new Protocol(MSG::OK);

        return gInstance;
    }

    Protocol::Protocol(MSG msgType) {
        this->mMsgType = msgType;
    }

} // Handler
