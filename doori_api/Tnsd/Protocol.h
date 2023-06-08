//
// Created by jaeseong on 23. 4. 28.
//
#ifndef DOORI_PROJECT_PROTOCOL_H
#define DOORI_PROJECT_PROTOCOL_H

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include "Common/Error.h"
#include "Tnsd/Topic.h"
#include "Tnsd/NodeInfo.h"
#include "Data/Json.h"
#include "Communication/IIPCTopology.h"
#include "Stream/StreamTemplate.h"
#include "Stream/IHeader.h"
#include "Stream/IBody.h"
#include "Stream/IFooter.h"
#include "NodeInfo.h"

using namespace std;

namespace doori::Tnsd{

    enum PROTOCOL
    {
         INTERNAL_ERROR
        ,NOTIFY
        ,ANWSER
        ,CHANGE
        ,ALIVE
        ,CLOSE
        ,PUBLISH
        ,REPORT
    };

    class Protocol : public Common::Error, public Stream::StreamTemplate
    {
    public:
        Protocol(Stream::IHeader& header, Stream::IBody& body, Stream::IFooter& footer);
        /**
         * 처리중 에러가 발생되었습니다. 해당 에러를 보냅니다.
         * @param node subscriber 또는 publisher node
         * @param error 에러내용
         */
        void InternalError(Communication::IIPCTopology& pubsub, string error);

        /**
         * Notify protocol
         * @param tnsdNode Tnsd의 node
         * @param myself  subscriber 또는 publisher의 Tnsd에게 자신의 정보를 알립니다.
         */
        void Notify(Communication::IIPCTopology& tnsd, NodeInfo myself);

        /**
         * Anwser protocol
         * @param node  subscriber 또는 publisher Topology
         * @param hash16  정보의 유일성을 체크할 수 있는 해쉬값
         */
        void Anwser(Communication::IIPCTopology& node, string hash16);

        /**
         * Tnsd -> Subscriber
         * @param subNode subscriber node
         * @param publisherList 특정 Topic에 publish하는 publisher의 집합
         */
        void Anwser(Communication::IIPCTopology& sub, vector<NodeInfo> publisherList);

        /**
         * Change protocol
         * @param iipcTopology subscriber Topology
         * @param publisher 특정 Topic에 publish하는 a publisher의 정보
         */
        void Change(Communication::IIPCTopology& sub, NodeInfo publisher);

        /**
         * Close protocol
         * @param iipcTopology  Tnsd의 Topology
         * @param myself 자신의 정보
         */
        void Close(Communication::IIPCTopology& tnsd, NodeInfo myself);

        /**
         * Publish protocol
         * @param iipcTopology subsriber Topology
         * @param bytes publish하기 위한 데이터
         */
        void Publish(Communication::IIPCTopology& sub, vector<char> bytes);

        /**
         * Alive protocol
         * @param iipcTopology subscriber 또는 publisher Topology
         * @param hash16  정보의 유일성을 체크할 수 있는 해쉬값
         */
        void Alive(Communication::IIPCTopology& pubsub, string hash16 );

        /**
         * Report protocol
         * @param iipcTopology Admin Topology
         * @param report 정보성 데이터
         */
        void Report(Communication::IIPCTopology& admin, string report);


        vector<char> GetStream() override;

    private:
        void Send(Communication::IIPCTopology &iipcTopology, const string &buffer);
        std::string SwitchProtocolName(PROTOCOL protocol) const;
        PROTOCOL SwitchProtocolEnum(string protocol) const;
    };
}

#endif //DOORI_PROJECT_PROTOCOL_H
