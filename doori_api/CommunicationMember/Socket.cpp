//
// Created by jaeseong on 23. 5. 19.
//

#include "Socket.h"

namespace doori {
    namespace CommunicationMember {

        Socket::Socket() : mFd(-1), mStatus(SOCK_STATUS::CLOSED) {

        }

        Socket::Socket(int fd, SOCK_STATUS status) : mFd(fd), mStatus(status) {

        };

        void Socket::SetBitwise(SOCK_STATUS status) {
            this->mStatus = this->mStatus | status;
        };

        bool Socket::IsBitwise(SOCK_STATUS status) const {
            if( (this->mStatus & status) == status) {
                LOG(DEBUG, "Set Bitwise:[", status, "]");
                return true;
            }
            return false;
        }

        int Socket::GetFd() const{
            return this->mFd;
        }

        void Socket::SetBitwise(int fd, SOCK_STATUS status) {
            this->mFd = fd;
            this->mStatus = this->mStatus | status;
        }

    } // doori
} // CommunicationMember