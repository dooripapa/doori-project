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
        this->mStatus = static_cast<SOCK_STATUS>(this->mStatus|status);
    };

    bool Socket::IsBitwise(SOCK_STATUS status) const {
        if( (this->mStatus & status) == status) {
            LOG(DEBUG, "Socket status Bitwise:[", status, "]");
            return true;
        }
        return false;
    }

    int Socket::GetFd() const{
        return this->mFd;
    }

    void Socket::SetBitwise(int fd, SOCK_STATUS status) {
        this->mFd = fd;
        this->mStatus = static_cast<SOCK_STATUS>(this->mStatus | status);
    }

    long Socket::Send(const string &data) {

        LOG(DEBUG, "Send() stream:", data);

        return Send(data.c_str(), data.length());
    }

    long Socket::Recv(string &container, uint32_t tilDataSize) {

        char acTemp[1024] = {0,};

        auto p = make_unique<char[]>(tilDataSize);
        if(p == nullptr) {
            assert("malloc error");
        }

        auto ret = recv(mFd, p.get(), tilDataSize, MSG_WAITALL);
        if(ret == -1) {
            auto p = strerror_r(errno, acTemp, sizeof(acTemp) - 1);
            LOG(ERROR, "recv errno:", errno, " cause:", p);
            return -1;
        }

        container = p.get();

        return ret;
    }

    long Socket::Send(const char *data, uint16_t dataSize) {

        char acTemp[1024] = {0,};

        auto ret = send(mFd, data, dataSize, 0);
        if(ret == -1) {
            auto p = strerror_r(errno, acTemp, sizeof(acTemp) - 1);
            LOG(ERROR, "send errno:", errno, " cause:", p);
            return -1;
        }
        return ret;
    }

    int Socket::Close() {
        return close(mFd);
    }

    int Socket::GetBitwise() const {
        return mStatus;
    }

    bool Socket::Init() {

        auto fd= socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
            LOG(ERROR, "socket error");
            return false;
        }

        mFd = fd;
        mStatus = SOCK_STATUS::INIT;

        return true;
    }

    void Socket::SetOptBitwise(SOCK_OPT option) {
        this->mOption= static_cast<SOCK_OPT>(this->mOption | option);
    }

    bool Socket::IsOptBitwise(SOCK_OPT option) const {

        if( (this->mOption & option) == option) {
            LOG(DEBUG, "Socket Option Bitwise:[", option, "]");
            return true;
        }
        return false;
    }

} // CommunicationMember