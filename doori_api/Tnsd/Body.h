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

    template<typename T>
    class Body : public Stream::IBody {
    public:

        long GetLength() const override;

        vector<char> ToStream() override;

        /**
         * 처리중 에러가 발생되었습니다. 해당 에러를 보냅니다.
         * @param node Subscriber 또는 Publisher node
         * @param error 에러내용
         */
        void InternalError(string error);

        /**
         * Notify protocol
         * @param tnsdNode Tnsd의 node
         * @param myNodeInfo  Subscriber 또는 publisher의 Tnsd에게 자신의 정보를 알립니다.
         */
        void Notify(string topic, string side, string ip, string port);

        /**
         * Anwser protocol
         * @param node  Subscriber 또는 Publisher Topology
         * @param hash16  정보의 유일성을 체크할 수 있는 해쉬값
         */
        void Anwser(string hash16);
        void Anwser(string hash16, string info);

        /**
         * Change protocol
         * @param iipcTopology Subscriber Topology
         * @param publisher 특정 Topic에 publish하는 a publisher의 정보
         */
        void Change(string topic, string side, string ip, string port);

        /**
         * Close protocol
         * @param iipcTopology  Tnsd의 Topology
         * @param myNodeInfo 자신의 정보
         */
        void Close(string topic, string side, string ip, string port);

        /**
         * Publish protocol
         * @param iipcTopology subsriber Topology
         * @param json publish하기 위한 데이터
         */
        void Publish(T data);

        /**
         * Alive protocol
         * @param iipcTopology Subscriber 또는 Publisher Topology
         * @param hash16  정보의 유일성을 체크할 수 있는 해쉬값
         */
        void Alive(string hash16);

        /**
         * Report protocol
         * @param iipcTopology Admin Topology
         * @param report 정보성 데이터
         */
        void Report(T json);

        int FromStream(const string& buffer) override;

        [[nodiscard]] const T& GetBody() const;

    private:
        T mData;
    };

} // Tnsd

#include "Body.hpp"

#endif //DOORI_PROJECT_BODY_H
