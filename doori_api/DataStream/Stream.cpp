// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Stream.h"


using namespace std;
/*
STX context start (0x02)
ETX context end (0x03)

STX LLLLLEEEEEEEE $|...Data...|$ ETX
   123456789|123456789|123456789|
*/

namespace doori::DataStream{

    Stream::Stream()
    {
    }

    Stream::Stream(const Stream& rhs)
    {
        copyFrom( rhs );
    }

    Stream::Stream(Stream&& rhs)
    {
        copyFrom( rhs );

        rhs.mType = doori_stream_enc_type::ASCII;
        rhs.mLen = 0;
        rhs.mDooridata = "";
    }

    Stream::Stream(const Data& dooridata)
    {
        mLen = dooridata.toString().length();
        mDooridata = dooridata.toString();
    }
    auto Stream::init(const Data& dooridata)->void
    {
        mLen = dooridata.toString().length();
        mDooridata = dooridata.toString();
    }

    Stream::Stream(const Data& dooridata, const doori_stream_enc_type& type)
    {
        mLen = dooridata.toString().length();
        mType = type;
        mDooridata = dooridata.toString();
    }

    Stream::~Stream()
    {
    }


    auto Stream::setEncType(std::string encStr) -> int
    {
        if( encStr.length() != kEncordingBuffLen )
        {
            LOG(ERROR, "setInetAddr Encording str len is bad");
            return -1;
        }
        bitset<kEncordingBuffLen> encBits( encStr );

        switch( static_cast<doori_stream_enc_type>(encBits.to_ulong()) )
        {
            case doori_stream_enc_type::ASCII:
                mType = doori_stream_enc_type::ASCII;
                break;
            case doori_stream_enc_type::UTF_8:
                mType = doori_stream_enc_type::UTF_8;
                break;
            default:
                return -1;
        }
        return 0;
    }

    auto Stream::setString(string& encordType, string& dooriDataString) -> int
    {
        mLen = dooriDataString.length();
        if( setEncType( encordType ) < 0 )
        {
            LOG(ERROR, "encord info wrong");
            return -1;
        }
        mDooridata = dooriDataString;
        return 0;
    }

    auto Stream::toByteStream() const -> std::string
    {
        string Temp;
        string lenPart(5, '0');
        string lenStr = to_string( mLen+ 1/*ETX*/ + kEncordingBuffLen/*encording str*/ );
        auto len = lenStr.length();
        lenPart.replace( 5-len, len, lenStr );

        Temp += 0x02; //STX
        Temp += lenPart;

        bitset<kEncordingBuffLen> encBits( static_cast<unsigned int>(mType) );
        string encPart = encBits.to_string<char,std::string::traits_type, std::string::allocator_type>();

        Temp += encPart;
        Temp += mDooridata;
        Temp += 0x03; //ETX

        return Temp;
    }

    auto
    Stream::fromByteStream(string byteStream) -> int
    {
        // Header : STX, tail : ETX, check
        if (byteStream[0] != 0x02 && byteStream[byteStream.size()-1] != 0x03)
        {
            LOG(ERROR, "ByteStream, bad pos(STX or ETX)", byteStream[0], byteStream[byteStream.size()-1]);
            return -1;
        }
        // Length Info
        unsigned int lengthInfo = std::stoi( byteStream.substr(1, 5) );
        LOG(DEBUG, "Header's Len column[", lengthInfo,"]");
        if (lengthInfo>kMaxSendBuffLen)
        {
            LOG(ERROR, "Length column in Header is too big[",lengthInfo,"]" );
            return -1;
        }
        // Encording Info
        auto encordingInfo = byteStream.substr(6, kEncordingBuffLen);
        mType = getEncType(std::stoi(encordingInfo));
        if (mType == doori_stream_enc_type::UNKNOWN)
        {
            LOG(ERROR, "Stream unknown encording type");
            return -1;
        }
        mLen = lengthInfo;
        mDooridata = byteStream.substr(1+5+8, byteStream.size()-(1+5+8+1)/*STX + LEN + ENC + ETX*/ );
        LOG(DEBUG, "Data[", mDooridata, "]");

        return 0;
    }

    auto
    Stream::getEncType(const unsigned int code) -> doori_stream_enc_type
    {
        doori_stream_enc_type type;
        switch ( static_cast<doori_stream_enc_type>(code) )
        {
            case doori_stream_enc_type::ASCII:
                LOG(DEBUG, "ASCII stream");
                type = doori_stream_enc_type::ASCII;
                break;
            case doori_stream_enc_type::UTF_8:
                /* code */
                LOG(DEBUG, "UTF_8 stream");
                type = doori_stream_enc_type::UTF_8;
                break;
            default:
                LOG(FATAL, "unknown Encording stream");
                type = doori_stream_enc_type::UNKNOWN;
                break;
        }
        return type;
    }

    auto Stream::copyFrom(const Stream& rhs) -> void
    {
        mType = rhs.mType;
        mLen  = rhs.mLen ;
        mDooridata = rhs.mDooridata;
    }

    auto Stream::operator=(Stream&& rhs)-> Stream&
    {
        if( this == &rhs )
            return *this;

        mLen = 0;
        mType = doori_stream_enc_type::ASCII;

        copyFrom( rhs );

        rhs.mLen = 0;
        rhs.mType = doori_stream_enc_type::ASCII;
        rhs.mDooridata ="";

        return *this;
    }

    auto Stream::operator=(const Stream& rhs)-> Stream&
    {
        if( this == &rhs )
            return *this;

        mLen = 0;
        mType = doori_stream_enc_type::ASCII;

        copyFrom( rhs );

        return *this;
    }

    auto Stream::getString() const -> std::string
    {
        return mDooridata;
    }

    uint8_t DataStream::Stream::getHeaderSize() const {
        return 0;
    }

    uint8_t DataStream::Stream::getTotalSize() const {
        return 0;
    }

    std::string DataStream::Stream::serialize() const {
        string Temp;
        string lenPart(5, '0');
        string lenStr = to_string( mLen+ 1/*ETX*/ + kEncordingBuffLen/*encording str*/ );
        auto len = lenStr.length();
        lenPart.replace( 5-len, len, lenStr );

        Temp += 0x02; //STX
        Temp += lenPart;

        bitset<kEncordingBuffLen> encBits( static_cast<unsigned int>(mType) );
        string encPart = encBits.to_string<char,std::string::traits_type, std::string::allocator_type>();

        Temp += encPart;
        Temp += mDooridata;
        Temp += 0x03; //ETX

        return Temp;
    }

    bool DataStream::Stream::unserialzie(DataStream::IStream &stream) {
        // Header : STX, tail : ETX, check
        if (byteStream[0] != 0x02 && byteStream[byteStream.size()-1] != 0x03)
        {
            LOG(ERROR, "ByteStream, bad pos(STX or ETX)", byteStream[0], byteStream[byteStream.size()-1]);
            return -1;
        }
        // Length Info
        unsigned int lengthInfo = std::stoi( byteStream.substr(1, 5) );
        LOG(DEBUG, "Header's Len column[", lengthInfo,"]");
        if (lengthInfo>kMaxSendBuffLen)
        {
            LOG(ERROR, "Length column in Header is too big[",lengthInfo,"]" );
            return -1;
        }
        // Encording Info
        auto encordingInfo = byteStream.substr(6, kEncordingBuffLen);
        mType = getEncType(std::stoi(encordingInfo));
        if (mType == doori_stream_enc_type::UNKNOWN)
        {
            LOG(ERROR, "Stream unknown encording type");
            return -1;
        }
        mLen = lengthInfo;
        mDooridata = byteStream.substr(1+5+8, byteStream.size()-(1+5+8+1)/*STX + LEN + ENC + ETX*/ );
        LOG(DEBUG, "Data[", mDooridata, "]");

        return 0;
    }
}//namespace doori
