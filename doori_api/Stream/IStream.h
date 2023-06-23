//
// Created by jaeseong on 23. 6. 11.
//

#ifndef DOORI_PROJECT_ISTREAM_H
#define DOORI_PROJECT_ISTREAM_H

#include <vector>

using namespace std;

namespace doori::api::Stream{
    class IStream{
    public:
        virtual long GetLength() const = 0;
        virtual vector<char> ToStream() = 0;
        virtual int FromStream(string buffer) = 0;
    };

}

#endif //DOORI_PROJECT_ISTREAM_H
