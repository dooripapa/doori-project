//
// Created by jaeseong on 23. 5. 19.
//

#include <cassert>
#include "Socket.h"

namespace doori::CommunicationMember {

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

    long Socket::Send(const string &data) {

        return Send(data.c_str(), data.length());
    }

    long Socket::Recv(string &container, uint32_t tilDataSize) {

        auto p = make_unique<char[]>(tilDataSize);
        if(p == nullptr) {
            assert("malloc error");
        }

        auto ret = recv(mFd, p.get(), tilDataSize, MSG_WAITALL);
        if(ret == -1) {
            LOG(ERROR, "recv error");
            return -1;
        }

        container = p.get();

        return ret;
    }

    long Socket::Send(const char *data, uint16_t dataSize) {

        auto ret = send(mFd, data, dataSize, 0);
        if(ret == -1) {
            LOG(ERROR, "send error");
            return -1;
        }
        return ret;
    }

    int Socket::Close() {
        return close(mFd);
    }

} // CommunicationMember