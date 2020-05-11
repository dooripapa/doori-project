//
// Created by jaeseong on 20. 5. 7..
//
#include <gtest/gtest.h>
#include <json.h>

TEST(MyTestCase, PositiveTest)
{
    doori::Json json;
    json.append("lee", doori::Json_value(1234));
//    EXPECT_EQ(3, sum(1,2));
}


int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

