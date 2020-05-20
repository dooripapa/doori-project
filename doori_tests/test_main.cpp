//
// Created by jaeseong on 20. 5. 7..
//
#include <gtest/gtest.h>
#include <Json.h>
#include <iostream>

TEST(MyTestCase, PositiveTest)
{
    doori::Json json, sub_json;
    json.append("main_json", doori::Json_value(1111));
    EXPECT_EQ("{\"main_json\":\"1111\"}", json.toString());
    sub_json.append("sub_json", doori::Json_value("2222"));
    EXPECT_EQ("{\"sub_json\":\"2222\"}", sub_json.toString());
    json.append("test", sub_json);
    EXPECT_EQ("{\"my name\":\"leejaeseong\"}", json.toString());
}


int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

