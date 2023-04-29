//
// Created by jaeseong on 23. 4. 28.
//
#ifndef DOORI_PROJECT_PROTOCOL_H
#define DOORI_PROJECT_PROTOCOL_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include "ProtocolMsg.h"
#include "TnsdDistrict/Handler/IProtocolState.h"

namespace doori {
    namespace TnsdDistrict {
        namespace Protocol {

            class Protocol
            {
            public:
                /**
                 * 싱글톤이라, 복사생성자, 복사연산자 금지
                 */
                Protocol(Protocol&) = delete;
                Protocol(const Protocol&) = delete;
                void operator=(const Protocol&) = delete;

                /**
                 * 싱글톤 endpoint 호출함수
                 * @return
                 */
                static Protocol* GetInstance();

                /**
                 *
                 * @return
                 */
                IProtocolState* getState();
                void transitionTo(IProtocolState *state);
            private:
                Protocol(IProtocolState* msgType);
                TnsdDistrict::Protocol::MSG_TYPE mMsgType;
            };

            Protocol* gInstance = nullptr;

        }
    }
};

#endif //DOORI_PROJECT_PROTOCOL_H
