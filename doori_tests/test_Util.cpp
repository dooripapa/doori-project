//
// Created by jaeseong on 24. 3. 10.
//
#include <gtest/gtest.h>
#include "Common/Util.h"
#include <iostream>

using namespace doori::api::Common;
using namespace std;

struct A{
   char header[10];
   char topic[10];
   char name[20];
};

TEST(Util, Template)
{
    struct A a{"jae","lee","seong"};

    auto bytes = ToBytes(a);

    int i=0;
    std::for_each(bytes.begin(), bytes.end(), [&](auto v){
        i++;
        std::cout << v << '|';
    });
    std::cout << std::endl;

    std::cout << "count:"<<i<<endl;

}
