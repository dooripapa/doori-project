//
// Created by jslee on 23. 4. 29.
//

#ifndef DOORI_PROJECT_PROTOCOLSTATE_H
#define DOORI_PROJECT_PROTOCOLSTATE_H

#include "IProtocolState.h"

namespace doori {
    namespace TnsdDistrict {
        namespace Protocol {

            class  Notify : public IProtocolState{
            };

            class  ListUp : public IProtocolState{
            };

            class  Changed : public IProtocolState{
            };

            class  Report : public IProtocolState{
            };

            class  Alive : public IProtocolState{
            };

            class  Pub : public IProtocolState{
            };

            class  Sub : public IProtocolState{
            };

        } // doori
    } // TnsdDistrict
} // Handler

#endif //DOORI_PROJECT_PROTOCOLSTATE_H
