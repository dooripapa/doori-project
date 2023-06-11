//
// Created by jaeseong on 23. 6. 11.
//

#ifndef DOORI_PROJECT_HEADER_H
#define DOORI_PROJECT_HEADER_H

#include "Stream/IHeader.h"

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

    };

} // Tnsd

#endif //DOORI_PROJECT_HEADER_H
