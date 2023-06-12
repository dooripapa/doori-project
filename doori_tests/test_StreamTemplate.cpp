//
// Created by jaeseong on 23. 6. 8.
//

#include <gtest/gtest.h>
#include <thread>
#include "Data/Json.h"
#include "Stream/StreamTemplate.h"
#include "Tnsd/Header.h"
#include "Tnsd/Body.h"

using namespace std;
using namespace doori;

TEST(StreamTemplate, Usage) {

    Data::Json json{};
    Tnsd::Header header{};
    Tnsd::Body body{};

    header.SetProtocol(Tnsd::PROTOCOL::ALIVE);

    std::size_t hashValue = std::hash<std::string>{}("leejaeseong");

    std::ostringstream oss;

    oss << std::hex << hashValue;

    body.Alive(oss.str());

    Stream::StreamTemplate< Tnsd::Header, Tnsd::Body > stream{Stream::CODER::ASCII, Stream::ENDIAN::LITTLE, Stream::DATA_FORMAT::JSON, header, body};
    auto ret = stream.ToStream();
    cout<< "start 1:";
    for(const auto& m: ret)
    {
        cout<< m;
    }
    cout<< "end 1" << endl;

    ret = stream.ToStream();
    cout<< "start 2:";
    for(const auto& m: ret)
    {
        cout<< m;
    }
    cout<< "end 2" << endl;

}
