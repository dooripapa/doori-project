//
// Created by jaeseong on 23. 2. 3.
//

#ifndef DOORI_PROJECT_ISTREAM_H
#define DOORI_PROJECT_ISTREAM_H

#include <string>

using namespace std;

namespace doori::DataStream{
    class IStream {
    public:
        virtual auto getHeaderSize() const -> std::uint8_t = 0 ;
        virtual auto getTotalSize() const -> std::uint8_t = 0 ;
        virtual auto serialize() const-> std::string = 0;
        virtual auto unserialzie(IStream&) -> bool = 0;
    };
};

#endif //DOORI_PROJECT_ISTREAM_H
