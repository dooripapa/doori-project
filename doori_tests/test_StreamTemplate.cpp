//
// Created by jaeseong on 23. 6. 8.
//

#include <gtest/gtest.h>
#include <thread>
#include "Stream/StreamTemplate.h"
#include "Stream/IHeader.h"

using namespace std;
using namespace doori::Stream;

struct TnsdProtocol{
    char mIp[16];
    char mPort[8];
    char mName[64];

    TnsdProtocol() = default;

    TnsdProtocol(string ip, string port, string name){
        CopyToArray(mIp, ip);
        CopyToArray(mPort, port);
        CopyToArray(mName, name);
    };
};

struct Protocol{
    char mLen[8];
    char mCoder[8];
    char mEndian[8];
    char mDataFormat[8];
    struct TnsdProtocol mUserStruct;


    Protocol(string len, string coder, string endian, string dataFormant, TnsdProtocol user)
    {
        CopyToArray(mLen, len);
        CopyToArray(mCoder, coder);
        CopyToArray(mEndian, endian);
        CopyToArray(mDataFormat, dataFormant);
        memcpy(&mUserStruct, &user, sizeof(struct TnsdProtocol));
    };

    Protocol()  = default;

};

TEST(StreamTemplate, Usage) {

    Json json{};
    TnsdProtocol tnsdProtocol{"127.0.0.1", "8888", "lee.jae.seong"};

/*
    tnsdProtocol.SetSide();
    tnsdProtocol.SetIp();
    tnsdProtocol.SetPort();
    tnsdProtocol.SetTopic();
*/

    StreamTemplate< IHeader, IBody > stream{tnsdProtocol, json};
    auto ret = stream.ToStream();
    cout<< "start 1:";
    for(const auto& m: ret)
    {
        cout<< m;
    }
    cout<< "end 1" << endl;

    stream.LinkHeader(tnsdProtocol);
    stream.LinkBody(json);

    ret = stream.ToStream();
    cout<< "start 2:";
    for(const auto& m: ret)
    {
        cout<< m;
    }
    cout<< "end 2" << endl;

}
