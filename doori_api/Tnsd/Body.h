//
// Created by jaeseong on 23. 6. 8.
//

#ifndef DOORI_PROJECT_BODY_H
#define DOORI_PROJECT_BODY_H

#include <string>
#include "Stream/IBody.h"
#include "Data/Json.h"
#include "Tnsd/NodeInfo.h"

using namespace doori;

namespace doori::api::Tnsd {

    class Body : public Stream::IBody {
    public:

        long GetLength() const override;

        vector<char> ToStream() override;

        /**
         * 처리중 에러가 발생되었습니다. 해당 에러를 보냅니다.
         * @param node subscriber 또는 publisher node
         * @param error 에러내용
         */
        void InternalError(string error);

        /**
         * Notify protocol
         * @param tnsdNode Tnsd의 node
         * @param myNodeInfo  subscriber 또는 publisher의 Tnsd에게 자신의 정보를 알립니다.
         */
        void Notify(const NodeInfo& myNodeInfo);

        /**
         * Anwser protocol
         * @param node  subscriber 또는 publisher Topology
         * @param hash16  정보의 유일성을 체크할 수 있는 해쉬값
         */
        void Anwser(string hash16);

        /**
         * Tnsd -> Subscriber
         * @param subNode subscriber node
         * @param publisherList 특정 Topic에 publish하는 publisher의 집합
         */
        void Anwser(vector<NodeInfo> publisherList);

        /**
         * Change protocol
         * @param iipcTopology subscriber Topology
         * @param publisher 특정 Topic에 publish하는 a publisher의 정보
         */
        void Change(NodeInfo publisher);

        /**
         * Close protocol
         * @param iipcTopology  Tnsd의 Topology
         * @param myNodeInfo 자신의 정보
         */
        void Close(NodeInfo myNodeInfo);

        /**
         * Publish protocol
         * @param iipcTopology subsriber Topology
         * @param json publish하기 위한 데이터
         */
        void Publish(Data::Json json);

        /**
         * Alive protocol
         * @param iipcTopology subscriber 또는 publisher Topology
         * @param hash16  정보의 유일성을 체크할 수 있는 해쉬값
         */
        void Alive(string hash16);

        /**
         * Report protocol
         * @param iipcTopology Admin Topology
         * @param report 정보성 데이터
         */
        void Report(Data::Json json);

        int FromStream(string buffer) override;

    private:
        Data::Json mJson;
    };
} // Tnsd

#endif //DOORI_PROJECT_BODY_H
