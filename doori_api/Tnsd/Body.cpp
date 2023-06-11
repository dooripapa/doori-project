//
// Created by jaeseong on 23. 6. 8.
//

#include "Body.h"

namespace doori::Tnsd {
    long Body::GetLength() const {
        return mJson.serialize().length();
    }

    vector<char> Body::ToStream() {
        return vector<char>{begin(mJson.serialize()), end(mJson.serialize())};
    }
} // Tnsd