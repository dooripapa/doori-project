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

namespace doori::Tnsd {

    enum struct SIDE {
        SUB
        ,PUB
    };

    /**
     * Tnsd node정보를 정의하는 객체입니다.
     */
    class NodeInfo : public Common::Error{
    public:
        NodeInfo() = default;
        NodeInfo(const NodeInfo& rhs) = default;
        NodeInfo(Topic topic, SIDE side, string ip, string port);
        NodeInfo& operator=(const NodeInfo& rhs) = default;
        NodeInfo& operator=(NodeInfo&& rhs) = default;

        /**
         * Bytes 스트림으로 출력한다.
         * @return
         */
        vector<char> Serialize();

        /**
         * Bytes 스트림을 입력받아서, NodeInfo로 변환한다.
         * @param stream
         * @return
         */
        static NodeInfo Unserialize(vector<char> stream);

        string GetTopic() const;

        string GetSide() const;

        string GetIp() const;

        string GetPort() const;

    private:
        struct _solid{
            char topic[64];
            char side[8];
            in_addr_t ip;
            in_port_t port;
        };

        string switchSideString() const;

        Topic mTopic;
        SIDE mSide;
        string mIp;
        string mPort;
    };

} // Tnsd

#endif //DOORI_PROJECT_NODEINFO_H
