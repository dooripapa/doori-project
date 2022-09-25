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

TEST(Bigdecimal, SignBiggerThan)
{
    doori::Bigdecimal value1{"348"};
    doori::Bigdecimal value2{"123"};
    // value1 > value2
    EXPECT_GT( value1, value2 );

    doori::Bigdecimal value3{"192394959192394959"};
    doori::Bigdecimal value4{"192394959192394959"};
    // value1 > value2
    EXPECT_GE( value3, value4 );
}

TEST(Bigdecimal, MinusPositive)
{
    doori::Bigdecimal value1{"348"};
    doori::Bigdecimal value2{"123"};
    EXPECT_EQ(doori::Bigdecimal("225"), value1-value2 );
}

TEST(Bigdecimal, MinusNegative)
{
    doori::Bigdecimal value1{"23"};
    doori::Bigdecimal value2{"348"};
    EXPECT_EQ(doori::Bigdecimal("-325"), value1-value2 );
}

TEST(Bigdecimal, Minus)
{
    EXPECT_EQ(doori::Bigdecimal("34"),  doori::Bigdecimal("100")-doori::Bigdecimal("66"));
}

