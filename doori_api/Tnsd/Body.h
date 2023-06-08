//
// Created by jaeseong on 23. 6. 8.
//

#ifndef DOORI_PROJECT_BODY_H
#define DOORI_PROJECT_BODY_H

#include <string>
#include "Stream/IBody.h"

namespace doori::Tnsd {

    class Body : public Stream::IBody {

    public:
        Body(string data);
        vector<char> Get() override;

    private:
        string mBody;
    };

} // Tnsd

#endif //DOORI_PROJECT_BODY_H
