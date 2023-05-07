//
// Created by jslee on 23. 4. 29.
//

#include "IProtocolState.h"

namespace doori {
    namespace TnsdDistrict {
        namespace Protocol {
            void IProtocolState::notify() {
                LOG(DEBUG, "Notify" );
            }

            void IProtocolState::listUp() {
                LOG(DEBUG, "ListUp" );
            }

            void IProtocolState::changed() {
                LOG(DEBUG, "Changed" );
            }

            void IProtocolState::report() {
                LOG(DEBUG, "Report" );
            }

            void IProtocolState::alive() {
                LOG(DEBUG, "Alive" );
            }

            void IProtocolState::pub() {
                LOG(DEBUG, "Pub" );
            }

            void IProtocolState::sub() {
                LOG(DEBUG, "Sub" );
            }

        } // doori
    } // TnsdDistrict
} // Handler