//
// Created by jaeseong on 23. 6. 8.
//

#ifndef DOORI_PROJECT_NODEINFO_H
#define DOORI_PROJECT_NODEINFO_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <vector>
#include "Common/Error.h"
#include "Common/Util.h"
#include "Topic.h"

using namespace std;

namespace doori {
    namespace Tnsd {

        enum struct SIDE {
            SUB
            ,PUB
        };

        /**
         * Tnsd node정보를 정의하는 객체입니다.
         */
        class NodeInfo : public Common::Error{
        public:
            NodeInfo(Topic topic, SIDE side, string ip, string port);
            vector<char> operator()();
        private:
            struct _solid{
                char topic[64];
                char side[8];
                in_addr_t ip;
                in_port_t port;
            };

            SIDE mSide;
            string mIp;
            string mPort;
            Topic mTopic;
        };

    } // doori
} // Tnsd

#endif //DOORI_PROJECT_NODEINFO_H
