//
// Created by jaeseong on 23. 6. 11.
//

#include "Header.h"

namespace doori::Tnsd {

    long Header::GetLength() const {
        return 16;
    }

    vector<char> Header::ToStream() {
        auto value = switchProtocolName(mTnsdProtocol);
        return vector<char>(begin(value), end(value));
    }

    std::string Header::switchProtocolName(PROTOCOL protocol) const {

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

    PROTOCOL Header::switchProtocolEnum(string protocol) const {
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

    void Header::SetProtocol(PROTOCOL protocol) {

        mTnsdProtocol = protocol;

    }
} // Tnsd