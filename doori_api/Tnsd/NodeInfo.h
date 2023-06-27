//
// Created by jaeseong on 23. 6. 8.
//

#ifndef DOORI_PROJECT_NODEINFO_H
#define DOORI_PROJECT_NODEINFO_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdio>
#include <string>
#include <vector>
#include "Common/Error.h"
#include "Common/Util.h"
#include "Topic.h"

using namespace std;
using namespace doori::api;

namespace doori::api::Tnsd {

    enum struct SIDE {
        SUB
        ,PUB
    };

    /**
     * Tnsd node정보를 정의하는 객체입니다.
     */
    template<typename T>
    class NodeInfo : public Common::Error{
    public:
        NodeInfo() = default;
        NodeInfo(const NodeInfo& rhs) = default;
        NodeInfo(Topic topic, SIDE side, string ip, string port);
        NodeInfo& operator=(const NodeInfo& rhs) = default;
        NodeInfo& operator=(NodeInfo&& rhs) = default;

        bool operator==(const NodeInfo &rhs);

        string GetTopic() const;

        string GetSide() const;

        string GetIp() const;

        string GetPort() const;

        T GetIPC() const;

        void SetIPC(T t);

    private:
        string switchSideString() const;

        Topic mTopic;
        SIDE mSide;
        string mIp;
        string mPort;
        T mIPC;
    };

} // Tnsd

#endif //DOORI_PROJECT_NODEINFO_H
