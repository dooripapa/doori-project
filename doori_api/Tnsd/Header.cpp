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
        return vector<char>{begin(value), end(value)};
    }

    std::string Header::switchProtocolName(PROTOCOL protocol) const {
        ostringstream oss;

        switch(protocol) {
            case PROTOCOL::INTERNAL_ERROR:
                oss << setw(16) << "INTERNAL_ERROR";
                break;
            case PROTOCOL::NOTIFY:
                oss << setw(16) << "NOTIFY";
                break;
            case PROTOCOL::ANWSER:
                oss << setw(16) << "ANWSER";
                break;
            case PROTOCOL::CHANGE:
                oss << setw(16) << "CHANGE";
                break;
            case PROTOCOL::ALIVE:
                oss << setw(16) << "ALIVE";
                break;
            case PROTOCOL::CLOSE:
                oss << setw(16) << "CLOSE";
                break;
            case PROTOCOL::PUBLISH:
                oss << setw(16) << "PUBLISH";
                break;
            case PROTOCOL::REPORT:
                oss << setw(16) << "REPORT";
                break;
            default:
                oss << setw(16) << "";
                break;
        }
        return oss.str();
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