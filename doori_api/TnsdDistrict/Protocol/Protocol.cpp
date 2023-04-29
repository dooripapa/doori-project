//
// Created by jslee on 23. 4. 29.
//

#include "TnsdDistrict/Protocol/Protocol.h"

namespace doori {
    namespace TnsdDistrict {
        namespace Protocol {

            Protocol *Protocol::GetInstance() {

                if (gInstance == nullptr)
                    gInstance = new Protocol(MSG_TYPE::OK);

                return gInstance;
            }

            TnsdDistrict::Protocol::IProtocolState_s getState() {
                return this->mMsgType;
            }

            Protocol::Protocol(TnsdDistrict::Protocol::MSG_TYPE msgType) {
                this->mMsgType = msgType;
            }

        } // doori
    } // TnsdDistrict
} // Handler
