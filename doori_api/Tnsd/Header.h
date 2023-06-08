//
// Created by jaeseong on 23. 6. 8.
//

#ifndef DOORI_PROJECT_HEADER_H
#define DOORI_PROJECT_HEADER_H


#include <cstring>
#include <string>
#include <array>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "Stream/IHeader.h"
#include "Common/Error.h"
#include "Common/Util.h"
#include "Topic.h"


namespace doori {

    namespace Tnsd {

        enum struct FLOW_SIDE {
            SUB
            ,PUB
        };

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

        class Header : public Common::Error, public Stream::IHeader{

        public:
            Header(Stream::CODER coder=Stream::CODER::ASCII, Stream::ENDIAN endian=Stream::ENDIAN::LITTLE, Stream::DATA_FORMAT format=Stream::DATA_FORMAT::JSON);

            long GetLength() const override;

            Stream::CODER GetCoder() override;

            Stream::ENDIAN GetEndian() override;

            Stream::DATA_FORMAT GetDataFormat() override;

            vector<char> Get() override;

        private:
            struct _solidHeader{
                char coder[8];
                char endian[8];
                char dataformat[8];
            }mSolidHeader;

            Stream::CODER mCoder;
            Stream::ENDIAN mEndian;
            Stream::DATA_FORMAT mDataFormat;
        };

    } // doori
} // Tnsd

#endif //DOORI_PROJECT_HEADER_H
