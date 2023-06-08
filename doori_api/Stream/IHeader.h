//
// Created by jaeseong on 23. 6. 5.
//

#ifndef DOORI_PROJECT_IHEADER_H
#define DOORI_PROJECT_IHEADER_H

#include <cstdint>
#include <cstdio>
#include <vector>

using namespace std;

namespace doori::Stream{

    enum struct CODER{
        ASCII
        ,UTF8
        ,UTF16
    };

    enum struct ENDIAN{
        BIG
        ,LITTLE
    };

    enum struct DATA_FORMAT{
        SOLID
        ,JSON
        ,XML
        ,CSV
    };

    class IHeader{

    public:
        /**
         * Header의 길이를 리턴합니다.
         * @return ssize_t
         */
        virtual long GetLength() const = 0;

        /**
         * Encode, Decode방식을 리턴합니다.
         * @return CODER
         */
        virtual CODER GetCoder() = 0;

        /**
         * Byte Order 방식을 리턴합니다.
         * @return ENDIAN
         */
        virtual ENDIAN GetEndian() = 0;

        /**
         * Data 포멧정보를 리턴합니다.
         * @return DATA_FORMAT
         */
        virtual DATA_FORMAT GetDataFormat() = 0;

        /**
         * Header의 구성된 row data를 리턴합니다.
         * @return vector<char>
         */
        virtual vector<char> Get() = 0;
    };
};

#endif //DOORI_PROJECT_IHEADER_H
