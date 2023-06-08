//
// Created by jaeseong on 23. 6. 8.
//

#include "Header.h"

namespace doori {
    namespace Tnsd {
        long Header::GetLength() const{
            return sizeof(struct _solidHeader) ;
        }

        Stream::CODER Header::GetCoder() {
            return mCoder;
        }

        Stream::ENDIAN Header::GetEndian() {
            return mEndian;
        }

        Stream::DATA_FORMAT Header::GetDataFormat() {
            return mDataFormat;
        }

        vector<char> Header::Get() {

            memset(&mSolidHeader, 0x20, sizeof(mSolidHeader));

            switch (mCoder) {
                case Stream::CODER::ASCII:
                    snprintf(mSolidHeader.coder,5, "ASCII");
                    break;
                case Stream::CODER::UTF8:
                    snprintf(mSolidHeader.coder,4, "UTF8");
                    break;
                case Stream::CODER::UTF16:
                    snprintf(mSolidHeader.coder,5, "UTF16");
                    break;
                default:
                    snprintf(mSolidHeader.coder,5, "ASCII");
            }

            switch (mEndian) {
                case Stream::ENDIAN::BIG:
                    snprintf(mSolidHeader.endian, 3, "BIG");
                    break;
                case Stream::ENDIAN::LITTLE:
                    snprintf(mSolidHeader.endian, 6, "LITTLE");
                    break;
                default:
                    snprintf(mSolidHeader.endian, 6, "LITTLE");
            }

            switch (mDataFormat) {
                default:
                    snprintf(mSolidHeader.dataformat, 4, "JSON");
            }

            return ToBytes(mSolidHeader);
        }

        Header::Header(Stream::CODER coder, Stream::ENDIAN endian, Stream::DATA_FORMAT format) : mCoder{coder}, mEndian{endian}, mDataFormat{format}{

        }

    } // doori
} // Tnsd