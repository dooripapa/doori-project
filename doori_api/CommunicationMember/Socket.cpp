//
// Created by jaeseong on 23. 5. 19.
//

#include "Socket.h"

namespace doori {
    namespace CommunicationMember {

        void Socket::SetBitwise(int fd, SOCK_STATUS status) {
            this->fd = fd;
            this->status = this->status | status;
        };

        bool Socket::IsFailure() const {
            if( (this->status & SOCK_STATUS::FAILURE) == SOCK_STATUS::FAILURE) {
                LOG(ERROR, "Failure Bitwise.");
                return true;
            }
            return false;
        };

        bool Socket::IsBitwise(SOCK_STATUS status) const {
            if( (this->status & status) == status) {
                LOG(DEBUG, "Set Bitwise:[", status, "]");
                return true;
            }
            return false;
        }
    } // doori
} // CommunicationMember