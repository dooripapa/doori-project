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

TEST(StreamTemplate, Usage_Sending) {

    Tnsd::Header header{};
    Tnsd::Body<Data::Json> body{};

    header.SetProtocol(Tnsd::PROTOCOL::ALIVE);

    std::size_t hashValue = std::hash<std::string>{}("leejaeseong");

    std::ostringstream oss;

    oss << std::hex << hashValue;

    body.Alive(oss.str());

    /**
     * 참조로 Tnsd Header, Tnsd Body을 StreamTemplate 구성함.
     * 그래서, header, body를 변경이 발생되면, ToStream의 출력값이 변경됨.
     */
    Stream::StreamTemplate< Tnsd::Header, Tnsd::Body<Data::Json> > stream{Stream::ENCODING_TYPE::ASCII, Stream::ENDIAN::LITTLE, Stream::DATA_FORMAT::JSON, header, body};
    auto ret = stream.ToStream();
    cout<< "start 1:[";
    for(const auto& m: ret)
    {
        cout<< m;
    }
    cout<< "] end 1" << endl;

    header.SetProtocol(Tnsd::PROTOCOL::ANWSER);
    body.Anwser(oss.str());

    ret = stream.ToStream();
    cout<< "start 2:[";
    for(const auto& m: ret)
    {
        cout<< m;
    }
    cout<< "] end 2" << endl;

}
TEST(StreamTemplate, Usage_Receiving) {
    auto outStream = R"(ASCII   LITTLE  JSON    ALIVE           {"key":"881d6b416a5b83d7"})";

    Tnsd::Header header{};
    Tnsd::Body<Data::Json> body{};

    Stream::StreamTemplate< Tnsd::Header, Tnsd::Body<Data::Json> > stream{ header, body};

    stream.FromStream({outStream});
    EXPECT_EQ(Tnsd::PROTOCOL::ALIVE, header.GetProtocol());

    auto bodyStream = body.ToStream();

    string compareString{begin(bodyStream), end(bodyStream)};
    EXPECT_EQ(R"({"key":"881d6b416a5b83d7"})", compareString );
}
