//
// Created by jaeseong on 23. 6. 8.
//

#include <cstring>
#include "NodeInfo.h"

namespace doori::api::Tnsd {

    template<typename T>
    NodeInfo<T>::NodeInfo(Topic topic, SIDE side, string ip, string port) : mIp{ip}, mPort{port}, mIPC(0){

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
    T NodeInfo<T>::GetIPCTopoloyNode() const {
        return mIPC;
    }

    template<typename T>
    void NodeInfo<T>::SetIPCTopologyNode(T ipc) {
        this->mIPC = ipc;
    }
} // Tnsd