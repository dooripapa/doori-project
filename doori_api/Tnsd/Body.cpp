//
// Created by jaeseong on 23. 6. 8.
//

#include "Body.h"

namespace doori::Tnsd {
    vector<char> Body::Get() {
        return vector<char>({mBody.cbegin(), mBody.cend()});
    }

    Body::Body(string data) : mBody{data}{

    }
} // Tnsd