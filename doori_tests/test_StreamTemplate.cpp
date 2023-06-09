//
// Created by jaeseong on 23. 6. 8.
//

#include <gtest/gtest.h>
#include <thread>
#include "Stream/StreamTemplate.h"
#include "Stream/IHeader.h"

using namespace std;
using namespace doori::Stream;

struct UserStruct{
    char mIp[16];
    char mPort[8];
    char mName[64];

    UserStruct() = default;

    UserStruct(string ip, string port, string name){
        CopyToArray(mIp, ip);
        CopyToArray(mPort, port);
        CopyToArray(mName, name);
    };
};

struct SolidStruct{
    char mLen[8];
    char mCoder[8];
    char mEndian[8];
    char mDataFormat[8];
    struct UserStruct mUserStruct;


    SolidStruct(string len, string coder, string endian, string dataFormant, UserStruct user)
    {
        CopyToArray(mLen, len);
        CopyToArray(mCoder, coder);
        CopyToArray(mEndian, endian);
        CopyToArray(mDataFormat, dataFormant);
        memcpy(&mUserStruct, &user, sizeof(struct UserStruct));
    };

//    SolidStruct(SolidStruct&& solidStruct)
//    {
//        memcpy(this, &solidStruct, sizeof(*this));
//    };
//
//    SolidStruct(const SolidStruct& solidStruct)
//    {
//        memcpy(this, &solidStruct, sizeof(*this));
//    };

//    SolidStruct(const SolidStruct& solidStruct) = delete;
//    SolidStruct(SolidStruct&& solidStruct) = delete;

    SolidStruct()  = default;

};

TEST(StreamTemplate, Usage) {

    SolidStruct a = { "0", "ASCII", "LITTLE", "JSON", {"127.0.0.1","8888","leejaeseong"}};

    StreamTemplate<SolidStruct> protocol{ a };

    auto ret = ToBytes(protocol.);

    cout<< "start" << endl;
    for(const auto& m: ret)
    {
        cout<< "["<< m<< "]" << endl;
    }
    cout<< "end" << endl;
}
