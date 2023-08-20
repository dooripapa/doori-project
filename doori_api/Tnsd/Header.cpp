//
// Created by jaeseong on 23. 6. 11.
//

#include "Header.h"

namespace doori::api::Tnsd {

    constexpr long Header::GetLength() const {
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
                oss << setw(16) << std::left << "INTERNAL_ERROR";
                break;
            case PROTOCOL::NOTIFY:
                oss << setw(16) << std::left << "NOTIFY";
                break;
            case PROTOCOL::ANWSER:
                oss << setw(16) << std::left << "ANWSER";
                break;
            case PROTOCOL::CHANGE:
                oss << setw(16) << std::left << "CHANGE";
                break;
            case PROTOCOL::ALIVE:
                oss << setw(16) << std::left << "ALIVE";
                break;
            case PROTOCOL::CLOSE:
                oss << setw(16) << std::left << "CLOSE";
                break;
            case PROTOCOL::PUBLISH:
                oss << setw(16) << std::left << "PUBLISH";
                break;
            case PROTOCOL::REPORT:
                oss << setw(16) << std::left << "REPORT";
                break;
            default:
                oss << setw(16) << std::left << "";
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
        else if(protocol.compare(0, strlen("ALIVE"), "ALIVE") == 0) {
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

    int Header::FromStream(string buffer) {

        buffer.erase(std::remove_if(buffer.begin(), buffer.end(), [](unsigned char x){return std::isspace(x);}), buffer.end());

        mTnsdProtocol = switchProtocolEnum(buffer);

        if(PROTOCOL::INTERNAL_ERROR == mTnsdProtocol)
            return -1;

        return 0;
    }

    PROTOCOL Header::GetProtocol() {
        return mTnsdProtocol;
    }

} // Tnsd