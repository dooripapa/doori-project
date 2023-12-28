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

void ErrorOutput() {

    Myclass s;

    PRINT(DEBUG, "Insufficient argumets, You are missing one parameter.  {%t} [%t] (%t)", s, 100);
}


TEST(Log, OutputTest) {

    Myclass s;

    auto a = 9999;

    PRINT(DEBUG, "doori-project %t %t %t", s, 100, "doori-project");

    PRINT(DEBUG, "doori-project {%t} [%t] (%t)", s, 100, a);
}

TEST(Log, ExceptionCheck) {

    ASSERT_THROW(ErrorOutput(), runtime_error);

}