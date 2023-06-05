//
// Created by jslee on 23. 4. 29.
//

#include "Tnsd/Protocol.h"

using namespace std;

namespace doori::Tnsd{

    ProtocolSide::ProtocolSide(FLOW_SIDE side, string ip, string port, Topic topic) : Common::Error(), mMySide{side}, mMyIp{ip}, mMyPort{port}, mMyTopic{topic} {

    };

    ProtocolSideStream ProtocolSide::operator()() {

        ProtocolSideStream stream{};

        switch (mMySide) {
            case FLOW_SIDE::PUB:
                strncpy(stream.side, "PUB", 3);
                break;
            case FLOW_SIDE::SUB:
                strncpy(stream.side, "SUB", 3);
                break;
        }

        char *endPtr;
        stream.ip = inet_addr(mMyIp.c_str());
        stream.port = std::strtoul( mMyPort.c_str(), &endPtr, 10 );
        if (endPtr == mMyPort.c_str() || *endPtr != '\0') {
            LoggingByClientError("Error: Invalid input string");
            return stream;
        }

        if (stream.port > UINT16_MAX) {
            LoggingByClientError("Error: Value exceeds the maximum range of in_port_t");
            return stream;
        }

        auto length = mMyTopic.getTopicName().length();
        length = length > 64 ?64 : length;

        strncpy(stream.topic, mMyTopic.getTopicName().c_str(), length);

        this->AsSuccess();

        return stream;
    }

    void Protocol::InternalError(CommunicationMember::IIPCTopology &iipcTopology, string error) {

        auto ret = iipcTopology.Send(error);

        if(ret <= 0){
            LoggingByClientError("Topology's Send() error");
        }

    }

    // Error객체는 상태값은 정상으로 셋팅함.
    Protocol::Protocol() : Common::Error(0,true) {

    }

    void Protocol::Notify(CommunicationMember::IIPCTopology &iipcTopology, ProtocolSide myself) {

        auto data = myself();
        if(!myself.Status()) {
            LoggingByClientError("ProtocolSide -> ProtocolSideStream error");
            return;
        }

        const char* buffer = reinterpret_cast<const char*>(&data);
        std::vector<char> byteVector(buffer, buffer + sizeof(data));

        auto ret = iipcTopology.Send( {byteVector.cbegin(), byteVector.cend()});
        if(ret <= 0){
            LoggingByClientError("Topology's Send() error");
            return;
        }

    }
} // Tnsd
