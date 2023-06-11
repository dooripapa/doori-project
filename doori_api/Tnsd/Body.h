//
// Created by jaeseong on 23. 6. 8.
//

#ifndef DOORI_PROJECT_BODY_H
#define DOORI_PROJECT_BODY_H

#include <string>
#include "Stream/IBody.h"
#include "Data/Json.h"

using namespace doori;

namespace doori::Tnsd {

    class Body : public Stream::IBody {
    public:
        Body(Data::Json json) : mJson(json) { };

        long GetLength() const override;

        vector<char> ToStream() override;
    private:
        Data::Json mJson;
    };
} // Tnsd

#endif //DOORI_PROJECT_BODY_H
