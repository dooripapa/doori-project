//
// Created by jaeseong on 23. 4. 28.
//
#ifndef DOORI_PROJECT_PROTOCOL_H
#define DOORI_PROJECT_PROTOCOL_H

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "Common/Error.h"
#include "Tnsd/Topic.h"
#include "Data/Json.h"
#include "Communication/IIPCTopology.h"

using namespace std;

namespace doori::Tnsd{

    enum struct FLOW_SIDE {
        SUB
        ,PUB
    };

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

    constexpr std::size_t PROTOCOL_SIZE = static_cast<std::size_t>(PROTOCOL::REPORT)+1;

    class ProtocolMsg {
    public:
        ProtocolMsg()
        {
            mProtocol[PROTOCOL::INTERNAL_ERROR] = std::string("INTERNAL_ERROR");
            mProtocol[PROTOCOL::NOTIFY        ] = std::string("NOTIFY");
            mProtocol[PROTOCOL::ANWSER        ] = std::string("ANWSER");
            mProtocol[PROTOCOL::CHANGE        ] = std::string("CHANGE");
            mProtocol[PROTOCOL::CLOSE         ] = std::string("CLOSE");
            mProtocol[PROTOCOL::PUBLISH       ] = std::string("PUBLISH");
            mProtocol[PROTOCOL::ALIVE         ] = std::string("ALIVE");
            mProtocol[PROTOCOL::REPORT        ] = std::string("REPORT");
        };

        string GetProtocolName(enum struct PROTOCOL protocol) { return mProtocol[protocol]; };

    private:
        array<string, PROTOCOL_SIZE> mProtocol;
    };

    struct ProtocolSideStream{
        char side[7+1];
        in_addr_t ip;
        in_port_t port;
        char topic[63+1];
    };

    /**
     * Publisher, Subscriber 어느 side인지 정의합니다.
     * 기본정보를 입력받습니다.
     */
    class ProtocolSide : public Common::Error{
    public:
        ProtocolSide(FLOW_SIDE side, string ip, string port, Topic topic);

        /**
         * Side의 정보를 Solid struct형식의 c타입의 byte를 출력합니다.
         * @return
         */
        ProtocolSideStream operator()() ;
    private:
        FLOW_SIDE mMySide;
        string mMyIp;
        string mMyPort;
        Topic mMyTopic;
    };

    class Protocol : Common::Error
    {
    public:
        Protocol();
        /**
         * 처리중 에러가 발생되었습니다. 해당 에러를 보냅니다.
         * @param iipcTopology subscriber 또는 publisher Topology
         * @param error 에러내용
         */
        void InternalError(Communication::IIPCTopology& iipcTopology, string error);

        /**
         * Notify protocol
         * @param iipcTopology Tnsd의 Topology
         * @param myself  subscriber 또는 publisher의 Tnsd에게 자신을 알립니다.
         */
        void Notify(Communication::IIPCTopology& iipcTopology, ProtocolSide myself);

        /**
         * Anwser protocol
         * @param iipcTopology  subscriber 또는 publisher Topology
         * @param hash16  정보의 유일성을 체크할 수 있는 해쉬값
         */
        void Anwser(Communication::IIPCTopology& iipcTopology, string hash16);

        /**
         * Anwser protocol
         * @param iipcTopology subscriber Topology
         * @param publisherList 특정 Topic에 publish하는 publisher의 집합
         */
        void Anwser(Communication::IIPCTopology& iipcTopology, vector<ProtocolSide> publisherList);

        /**
         * Change protocol
         * @param iipcTopology subscriber Topology
         * @param publisher 특정 Topic에 publish하는 a publisher의 정보
         */
        void Change(Communication::IIPCTopology& iipcTopology, ProtocolSide publisher);

        /**
         * Close protocol
         * @param iipcTopology  Tnsd의 Topology
         * @param myself 자신의 정보
         */
        void Close(Communication::IIPCTopology& iipcTopology, ProtocolSide myself);

        /**
         * Publish protocol
         * @param iipcTopology subsriber Topology
         * @param bytes publish하기 위한 데이터
         */
        void Publish(Communication::IIPCTopology& iipcTopology, vector<char> bytes);

        /**
         * Alive protocol
         * @param iipcTopology subscriber 또는 publisher Topology
         * @param hash16  정보의 유일성을 체크할 수 있는 해쉬값
         */
        void Alive(Communication::IIPCTopology& iipcTopology, string hash16 );

        /**
         * Report protocol
         * @param iipcTopology Admin Topology
         * @param report 정보성 데이터
         */
        void Report(Communication::IIPCTopology& iipcTopology, string report);
    };
}

#endif //DOORI_PROJECT_PROTOCOL_H
