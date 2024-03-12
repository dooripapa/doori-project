//
// Created by jaeseong on 23. 6. 8.
//

#include <gtest/gtest.h>
#include <thread>
#include "Data/Json.h"
#include "Stream/IHeader.h"
#include "Stream/IBody.h"
#include "Stream/StreamTemplate.h"

using namespace std;
using namespace doori::api;

struct AHeader : public Stream::IHeader {

    std::array<char, 10> name{};
    std::array<char, 20> address{};

public:
    AHeader() = default;
    AHeader(string name, string address) {
        this->name.fill(' ');
        this->address.fill(' ');

        memcpy(this->name.data(), name.data(), name.size());
        memcpy(this->address.data(), address.data(), address.size());
    };

    /*
     * IHeader 상속 받으면 구현해야할 필수 함수.
     */
    long GetLength() const override {
        return name.size()+address.size();
    }

    /*
     * IHeader 상속 받으면 구현해야할 필수 함수.
     */
    vector<char> ToStream() override {
        vector<char> temp{};

        for_each(name.cbegin(), name.cend(), [&](char t) { temp.emplace_back(t); });

        for_each(address.cbegin(), address.cend(), [&](char t) { temp.emplace_back(t); });

        return temp;
    }

    /*
     * IHeader 상속 받으면 구현해야할 필수 함수.
     */
    int FromStream(const string &buffer) override {
        if (buffer.size()!=(name.size()+address.size()))
            return -1;

        memcpy(name.data(), buffer.data(), name.size());
        memcpy(address.data(), buffer.data()+name.size(), address.size());

        return 0;
    }
};

struct ABody : public Stream::IBody {
    std::array<char, 4> age{};
    std::array<char, 10> keyvalue{};

    ABody()=default;
    ABody(string age, string keyvalue) {
        this->age.fill(' ');
        this->keyvalue.fill(' ');

        memcpy(this->age.data(), age.data(), age.size());
        memcpy(this->keyvalue.data(), keyvalue.data(), keyvalue.size());
    };

    /*
     * IBody 상속 받으면 구현해야할 필수 함수.
     */
    long GetLength() const override {
        return age.size()+keyvalue.size();
    }

    /*
     * IBody 상속 받으면 구현해야할 필수 함수.
     */
    vector<char> ToStream() override {
        vector<char> temp{};

        for_each(age.cbegin(), age.cend(), [&](char t) { temp.emplace_back(t); });

        for_each(keyvalue.cbegin(), keyvalue.cend(), [&](char t) { temp.emplace_back(t); });

        return temp;
    }

    /*
     * IBody 상속 받으면 구현해야할 필수 함수.
     */
    int FromStream(const string &buffer) override {
        if (buffer.size()!=(age.size()+keyvalue.size()))
            return -1;

        memcpy(age.data(), buffer.data(), age.size());
        memcpy(keyvalue.data(), buffer.data()+age.size(), keyvalue.size());

        return 0;
    }
};

TEST(StreamTemplate, Usage_Sending) {

    AHeader header{"jaeseong","066854"};
    ABody body{"20","haass"};

    /**
     * 참조로 Tnsd Header, Tnsd Body을 StreamTemplate 구성함.
     * 그래서, header, body를 변경이 발생되면, ToStream의 출력값이 변경됨.
     */
    Stream::StreamTemplate< AHeader, ABody > stream{Stream::ENCODING_TYPE::ASCII, Stream::ENDIAN::LITTLE, Stream::DATA_FORMAT::JSON, header, body};
    auto ret = stream.ToStream();
    cout<< "start 1:[";
    for(const auto& m: ret)
    {
        cout<< m;
    }
    cout<< "] end 1" << endl;

    ret = stream.ToStream();
    cout<< "start 2:[";
    for(const auto& m: ret)
    {
        cout<< m;
    }
    cout<< "] end 2" << endl;

}

TEST(StreamTemplate, Usage_Receiving) {
    //                            | StreamTemplate         |  AHeader                   |   ABody     |
    auto outStream = R"(ASCII   LITTLE  JSON    jaeseong  066854              20  haass     )";

    AHeader header{};
    ABody body{};

    Stream::StreamTemplate< AHeader, ABody > stream{ header, body};

    stream.FromStream({outStream});

    auto headerStream = header.ToStream();
    auto bodyStream = body.ToStream();

    string compareHeader{begin(headerStream), end(headerStream)};
    string compareBody{begin(bodyStream), end(bodyStream)};
    EXPECT_EQ(R"(jaeseong  066854              )", compareHeader );
    EXPECT_EQ(R"(20  haass     )", compareBody );
}
