//
// Created by jaeseong on 22. 6. 2..
//

#include <gtest/gtest.h>
#include <Bigdecimal.h>
#include <iostream>

using namespace std;
using namespace doori;

TEST(Bigdecimal, ConstructTest)
{
    doori::Bigdecimal v1{"5"};
    EXPECT_EQ("5", v1.toString());

    doori::Bigdecimal v2{"999999999999999999999999999999999999999999999999999999999"};
    EXPECT_EQ("999999999999999999999999999999999999999999999999999999999", v2.toString());

    doori::Bigdecimal v3{"-9"};
    EXPECT_EQ("-9", v3.toString());

    doori::Bigdecimal v4{"-00000000000000000000000000000000000000"};
    EXPECT_EQ("0", v4.toString());

    doori::Bigdecimal v5{"+0"};
    EXPECT_EQ("0", v5.toString());

    doori::Bigdecimal v6{"+12345"};
    EXPECT_EQ("12345", v6.toString());

    doori::Bigdecimal v7{"-123456"};
    EXPECT_EQ("-123456", v7.toString());

    doori::Bigdecimal v8{"   -123456"};
    EXPECT_EQ("-123456", v8.toString());

    doori::Bigdecimal v9{"   +123456"};
    EXPECT_EQ("123456", v9.toString());

    doori::Bigdecimal v10{"   +00000000000000000000000"};
    EXPECT_EQ("0", v10.toString());

    doori::Bigdecimal v11{"   -00000"};
    EXPECT_EQ("0", v11.toString());

    doori::Bigdecimal v12{"   -0.000"};
    EXPECT_EQ("0", v12.toString());

    doori::Bigdecimal v13{"   +0.000"};
    EXPECT_EQ("0", v13.toString());

    doori::Bigdecimal v14{"   00.1234"};
    EXPECT_EQ("0.1234", v14.toString());

    doori::Bigdecimal v15{"+00.1234"};
    EXPECT_EQ("0.1234", v15.toString());

    doori::Bigdecimal v16{"100.0000"};
    EXPECT_EQ("100", v16.toString());

    doori::Bigdecimal v17{"100.1000"};
    EXPECT_EQ("100.1", v17.toString());

    doori::Bigdecimal v18{"0.000000"};
    EXPECT_EQ("0", v18.toString());
}

TEST(Bigdecimal, GT)
{
//    doori::Bigdecimal v1{"348"};
//    doori::Bigdecimal v2{"123"};
//    // value1 > value2
//    EXPECT_GT( v1, v2 );
//
//    doori::Bigdecimal v11{"123"};
//    doori::Bigdecimal v22{"-123"};
//    // value1 > value2
//    EXPECT_GT( v11, v22);
//
//    doori::Bigdecimal v111{"0"};
//    doori::Bigdecimal v222{"-99999999999999999999999999999999999999999999999999999999999999999999"};
//    // value1 > value2
//    EXPECT_GT( v111, v222);
//
//    doori::Bigdecimal f1{"0.110000"};
//    doori::Bigdecimal f2{"0.100001"};
//    // value1 > value2
//    EXPECT_GT( f1, f2);
//
//    doori::Bigdecimal f11{"0.110000"};
//    doori::Bigdecimal f22{"-0.100001"};
//    // value1 > value2
//    EXPECT_GT( f11, f22);

    doori::Bigdecimal f3{"1"};
    doori::Bigdecimal f4{"0.100001"};
    // value1 > value2
    EXPECT_GT( f3, f4);
}

TEST(Bigdecimal, GE)
{
    doori::Bigdecimal v1{"0"};
    doori::Bigdecimal v2{"0"};
    // value1 >= value2
    EXPECT_GE( v1, v2);

    doori::Bigdecimal v3{"0"};
    doori::Bigdecimal v4{"-99999999999999"};
    // value1 >= value2
    EXPECT_GE( v1, v2);

    doori::Bigdecimal value3{"192394959192394959"};
    doori::Bigdecimal value4{"192394959192394959"};
    // value1 >= value2
    EXPECT_GE( value3, value4 );

    doori::Bigdecimal v5{"-123.0234"};
    doori::Bigdecimal v6{"-123.1111"};
    EXPECT_GE(v5,v6);
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
    auto v = doori::Bigdecimal("-1")-doori::Bigdecimal("-2");

    EXPECT_EQ(doori::Bigdecimal("1"),  doori::Bigdecimal("-1")-doori::Bigdecimal("-2"));
//    EXPECT_EQ(doori::Bigdecimal("34"),  doori::Bigdecimal("100")-doori::Bigdecimal("66"));
//    EXPECT_EQ(doori::Bigdecimal("10"),  doori::Bigdecimal("-1")-doori::Bigdecimal("-11"));
}

TEST(Bigdecimal, temp)
{
    doori::Bigdecimal t{"0"};

    auto v=t.revisionSameString("1.01", "0.000001");

    LOG(DEBUG, "v1[", v.first, "]", "  ", "v2[", v.second, "]");
}
