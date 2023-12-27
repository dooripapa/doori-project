//
// Created by jaeseong on 20. 5. 25..
//

#include <gtest/gtest.h>
#include "Common/Log.h"
#include <iostream>

using namespace doori::api;

class Myclass{
public:
    friend ostream& operator<<(ostream& os, Myclass ob)
    {
        os << "Myclass.a=[" << ob.a << "] name[" << ob.name << "]";
        return os;
    }
private:
    int a = 100;
    string name{"Myclass"};
};

TEST(Log, OutputTest) {

    Myclass s;

    PRINT(DEBUG, "leejaeseong %t", s);
}