//
// Created by jaeseong on 23. 6. 11.
//

#ifndef DOORI_PROJECT_HEADER_H
#define DOORI_PROJECT_HEADER_H

#include <string>
#include "Stream/IHeader.h"
#include "Tnsd/NodeInfo.h"

using namespace std;
using namespace doori;

namespace doori::Tnsd {

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
        long GetLength() const override;

        vector<char> ToStream() override;

        void SetProtocol(PROTOCOL protocol);

    private:

        PROTOCOL mTnsdProtocol;

        string switchProtocolName(PROTOCOL protocol) const;

        PROTOCOL switchProtocolEnum(string protocol) const;

    };

} // Tnsd

#endif //DOORI_PROJECT_HEADER_H
