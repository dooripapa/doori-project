//
// Created by jaeseong on 23. 6. 11.
//

#ifndef DOORI_PROJECT_HEADER_H
#define DOORI_PROJECT_HEADER_H

#include <string>
#include "Stream/IHeader.h"
#include "Tnsd/NodeInfo.h"

using namespace std;
using namespace doori::api;

namespace doori::api::Tnsd {

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

    class Header : public Stream::IHeader {

    public:
        [[nodiscard]] long GetLength() const override;

        [[nodiscard]] vector<char> ToStream() override;

        void SetProtocol(PROTOCOL protocol);

        PROTOCOL GetProtocol();

        [[nodiscard]] int FromStream(const string& buffer) override;

    private:

        PROTOCOL mTnsdProtocol;

        [[nodiscard]] static string switchProtocolName(PROTOCOL protocol) ;

        [[nodiscard]] static PROTOCOL switchProtocolEnum(const string& protocol) ;

    };

} // Tnsd

#endif //DOORI_PROJECT_HEADER_H
