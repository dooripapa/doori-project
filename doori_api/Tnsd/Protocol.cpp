//
// Created by jslee on 23. 4. 29.
//

#include "Tnsd/Protocol.h"

using namespace std;

namespace doori::Tnsd{

    void Protocol::InternalError(Communication::IIPCTopology &node, string error) {

        auto ret = node.Send(error);

        if(ret <= 0){
            LoggingByClientError("Topology's Send() error");
        }

    }

    // Error객체는 상태값은 정상으로 셋팅함.
    Protocol::Protocol() : Common::Error(0,true) {

    }

    void Protocol::Notify(Communication::IIPCTopology &tnsdNode, NodeInfo myself) {

        auto data = myself();
        if(!myself.Status()) {
            LoggingByClientError("ProtocolSide -> ProtocolSideStream error");
            return;
        }

        const char* buffer = reinterpret_cast<const char*>(&data);
        std::vector<char> byteVector(buffer, buffer + sizeof(data));

        Send(tnsdNode, {byteVector.cbegin(), byteVector.cend()});

    }

    void Protocol::Send(Communication::IIPCTopology &iipcTopology, const string &buffer) {
        auto ret = iipcTopology.Send( buffer );
        if(ret <= 0){
            this->LoggingByClientError("Topology's Send() error");
            return;
        }
    }

    void Protocol::Anwser(Communication::IIPCTopology &node, string hash16) {

        Send(node, hash16);
    }

    void Protocol::Anwser(Communication::IIPCTopology &subNode, vector<ProtocolSide> publisherList) {

        auto pData = make_unique<char[]>(sizeof(ProtocolSide) * publisherList.size());


    }

    vector<char> Protocol::GetStream() {
        return vector<char>();
    }

    Protocol::Protocol(Stream::IHeader &header, Stream::IBody &body, Stream::IFooter &footer)
    : StreamTemplate(header, body, footer) , Common::Error(0, true){

    }

    std::string Protocol::SwitchProtocolName(PROTOCOL protocol) const {

        string ret{};

        switch(protocol) {
            case PROTOCOL::INTERNAL_ERROR:
                ret = std::string{"INTERNAL_ERROR"};
                break;
            case PROTOCOL::NOTIFY:
                ret = std::string{"NOTIFY"};
                break;
            case PROTOCOL::ANWSER:
                ret = std::string{"ANWSER"};
                break;
            case PROTOCOL::CHANGE:
                ret = std::string{"CHANGE"};
                break;
            case PROTOCOL::ALIVE:
                ret = std::string{"CLOSE"};
                break;
            case PROTOCOL::CLOSE:
                ret = std::string{"PUBLISH"};
                break;
            case PROTOCOL::PUBLISH:
                ret = std::string{"ALIVE"};
                break;
            case PROTOCOL::REPORT:
                ret = std::string{"REPORT"};
                break;
            default:
                ret = std::string{""};
        }
        return ret;
    }

    PROTOCOL Protocol::SwitchProtocolEnum(string protocol) const {
        if(protocol == "NOTIFY") {
            return PROTOCOL::NOTIFY;
        }
        else if(protocol == "ANWSER") {
            return PROTOCOL::ANWSER;
        }
        else if(protocol == "CHANGE") {
            return PROTOCOL::CHANGE;
        }
        else if(protocol == "CLOSE") {
            return PROTOCOL::CLOSE;
        }
        else if(protocol == "PUBLISH") {
            return PROTOCOL::PUBLISH;
        }
        else if(protocol == "ALIVE") {
            return PROTOCOL::ALIVE;
        }
        else if(protocol == "REPORT") {
            return PROTOCOL::REPORT;
        }
        else{
            return PROTOCOL::INTERNAL_ERROR;
        }
    }

} // Tnsd
