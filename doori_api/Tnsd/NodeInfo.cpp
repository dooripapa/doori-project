//
// Created by jaeseong on 23. 6. 8.
//

#include <cstring>
#include "NodeInfo.h"

namespace doori {
    namespace Tnsd {
        NodeInfo::NodeInfo(Topic topic, SIDE side, string ip, string port) : mTopic{topic}, mSide{side}, mIp{ip}, mPort{port}{

        }

        vector<char> NodeInfo::operator()() {

            struct _solid solidStruct;

            memset(&solidStruct, 0x20, sizeof(struct _solid));

            switch (mSide) {
                case SIDE::PUB:
                    strncpy(solidStruct.side, "PUB", 3);
                    break;
                case SIDE::SUB:
                    strncpy(solidStruct.side, "SUB", 3);
                    break;
            }

            char *endPtr;
            solidStruct.ip = inet_addr(mIp.c_str());
            solidStruct.port = std::strtoul( mPort.c_str(), &endPtr, 10 );
            if (endPtr == mIp.c_str() || *endPtr != '\0') {
                LoggingByClientError("Error: Invalid input string");
                return vector<char>{};
            }

            if (solidStruct.port > UINT16_MAX) {
                LoggingByClientError("Error: Value exceeds the maximum range of in_port_t");
                return vector<char>{};
            }

            auto length = mTopic.getTopicName().length();
            length = length > 64 ?64 : length;

            strncpy(solidStruct.topic, mTopic.getTopicName().c_str(), length);

            this->AsSuccess();

            return ToBytes(solidStruct);
        }
    } // doori
} // Tnsd