//
// Created by jaeseong on 22. 6. 2..
//

#include <gtest/gtest.h>
#include <Bigdecimal.h>
#include <iostream>

TEST(Bigdecimal, ConstructTest)
{
    doori::Bigdecimal value1{"5"};
    EXPECT_EQ("5", value1.toString());

    doori::Bigdecimal value2{"9"};
    EXPECT_EQ("9", value2.toString());

    doori::Bigdecimal value3{"-9"};
    EXPECT_EQ("-9", value3.toString());
}

TEST(Bigdecimal, Minus)
{
    doori::Bigdecimal value1{"123"};
    doori::Bigdecimal value2{"348"};
    EXPECT_EQ(doori::Bigdecimal("-225"), value1-value2 );

    EXPECT_EQ(doori::Bigdecimal("34"),  "34");
    EXPECT_EQ(std::string("34"),  doori::Bigdecimal("100")-doori::Bigdecimal("66"));
}
