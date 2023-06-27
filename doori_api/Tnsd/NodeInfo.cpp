//
// Created by jaeseong on 23. 6. 8.
//

#include <cstring>
#include "NodeInfo.h"

namespace doori::api::Tnsd {

    template<typename T>
    NodeInfo<T>::NodeInfo(Topic topic, SIDE side, string ip, string port) : mTopic{topic}, mSide{side}, mIp{ip}, mPort{port}, mFd(0){

    }

    template<typename T>
    string NodeInfo<T>::GetTopic() const {
        return mTopic.GetKey();
    }

    template<typename T>
    string NodeInfo<T>::GetSide() const {
        return switchSideString();
    }

    template<typename T>
    string NodeInfo<T>::switchSideString() const {
        switch (mSide) {
            case SIDE::PUB:
                return string{"PUB"};
            case SIDE::SUB:
                return string{"SUB"};
        }
        return {"ERR"};
    }

    template<typename T>
    string NodeInfo<T>::GetIp() const {
        return mIp;
    }

    template<typename T>
    string NodeInfo<T>::GetPort() const {
        return mPort;
    }

    template<typename T>
    bool NodeInfo<T>::operator==(const NodeInfo &rhs) {
        if(this == &rhs)
            return true;

        return this->mTopic == rhs.mTopic && this->mSide == rhs.mSide && this->mIp == rhs.mIp &&
               this->mPort == rhs.mPort;
    }

    template<typename T>
    T NodeInfo<T>::GetIPC() const {
        return mIPC;
    }

    template<typename T>
    void NodeInfo<T>::SetIPC(T ipc) {
        this->mIPC = ipc;
    }
} // Tnsd