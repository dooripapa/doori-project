//
// Created by jaeseong on 22. 6. 2..
//

#include <gtest/gtest.h>
#include <Bigdecimal.h>
#include <iostream>
#include "DooriTest.h"

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

    doori::Bigdecimal v19{"-0.8999999"};
    EXPECT_EQ("-0.8999999", v19.toString());

    doori::Bigdecimal v20{"034"};
    EXPECT_EQ("34", v20.toString());
}

TEST(Bigdecimal, GT)
{
    doori::Bigdecimal v1{"348"};
    doori::Bigdecimal v2{"123"};
    // value1 > value2
    EXPECT_GT( v1, v2 );

    doori::Bigdecimal v11{"123"};
    doori::Bigdecimal v22{"-123"};
    // value1 > value2
    EXPECT_GT( v11, v22);

    doori::Bigdecimal v111{"0"};
    doori::Bigdecimal v222{"-99999999999999999999999999999999999999999999999999999999999999999999"};
    // value1 > value2
    EXPECT_GT( v111, v222);

    doori::Bigdecimal f1{"0.110000"};
    doori::Bigdecimal f2{"0.100001"};
    // value1 > value2
    EXPECT_GT( f1, f2);

    doori::Bigdecimal f11{"0.110000"};
    doori::Bigdecimal f22{"-0.100001"};
    // value1 > value2
    EXPECT_GT( f11, f22);

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

    doori::Bigdecimal value3{"192394959192394959192394959192394959192394959192394959192394959192394959"};
    doori::Bigdecimal value4{"192394959192394959192394959192394959192394959192394959192394959192394959"};
    // value1 >= value2
    EXPECT_GE( value3, value4 );

    doori::Bigdecimal v5{"-123.0234"};
    doori::Bigdecimal v6{"-123.1111"};
    EXPECT_GE(v5,v6);
}

TEST(Bigdecimal, MINUS)
{
    doori::Bigdecimal v1{"348.87"};
    doori::Bigdecimal v2{"123.9"};
    auto r=v1-v2;
    EXPECT_EQ(doori::Bigdecimal("224.97"), r);

    doori::Bigdecimal v3{"1.0000001"};
    doori::Bigdecimal v4{"1.9"};
    auto k = v3-v4;
    EXPECT_EQ(doori::Bigdecimal("-0.8999999"), k);

    doori::Bigdecimal m1{"23"};
    doori::Bigdecimal m2{"348"};
    EXPECT_EQ(doori::Bigdecimal("-325"), m1-m2);

    EXPECT_EQ(doori::Bigdecimal("1"),  doori::Bigdecimal("-1")-doori::Bigdecimal("-2"));
    auto v = doori::Bigdecimal("100")-doori::Bigdecimal("66");
    EXPECT_EQ(doori::Bigdecimal("34"),  v);
    EXPECT_EQ(doori::Bigdecimal("10"),  doori::Bigdecimal("-1")-doori::Bigdecimal("-11"));
}

TEST(Bigdecimal, PLUS)
{
    doori::Bigdecimal v1{"1"};
    doori::Bigdecimal v2{"2"};
    auto r1=v1+v2;
    EXPECT_EQ(doori::Bigdecimal("3"), r1);

    doori::Bigdecimal v3{"99291929394"};
    doori::Bigdecimal v4{"99291929394"};
    auto r2=v3+v4;
    EXPECT_EQ(doori::Bigdecimal("198583858788"), r2);

    doori::Bigdecimal v5{"1"};
    doori::Bigdecimal v6{"-1000"};
    auto r3=v5+v6;
    EXPECT_EQ(doori::Bigdecimal("-999"), r3);

    doori::Bigdecimal v7{"120.1234"};
    doori::Bigdecimal v8{"1273.123909"};
    auto r4=v7+v8;
    EXPECT_EQ(doori::Bigdecimal("1393.247309"), r4);

    doori::Bigdecimal v9{"-120.1234"};
    doori::Bigdecimal v10{"1273.123909"};
    auto r5=v9+v10;
    EXPECT_EQ(doori::Bigdecimal("1153.000509"), r5);

    doori::Bigdecimal v11{"120.1234"};
    doori::Bigdecimal v12{"-1273.123909"};
    auto r6=v11+v12;
    EXPECT_EQ(doori::Bigdecimal("-1153.000509"), r6);

    doori::Bigdecimal v13{"0.1"};
    doori::Bigdecimal v14{"0.1"};
    auto r7=v13+v14;
    EXPECT_EQ(doori::Bigdecimal("0.2"), r7);

    doori::Bigdecimal v15{"0"};
    doori::Bigdecimal v16{"0"};
    auto r8=v15+v16;
    EXPECT_EQ(doori::Bigdecimal("0"), r8);

    doori::Bigdecimal v17{"0"};
    doori::Bigdecimal v18{"-1"};
    auto r9=v17+v18;
    EXPECT_EQ(doori::Bigdecimal("-1"), r9);

    doori::Bigdecimal v19{"-1"};
    doori::Bigdecimal v20{"-1"};
    auto r10=v19+v20;
    EXPECT_EQ(doori::Bigdecimal("-2"), r10);
}

TEST(Bigdecimal, MULTIPLY)
{
    doori::Bigdecimal v1{"1"};
    doori::Bigdecimal v2{"2"};
    auto r1=v1*v2;
    EXPECT_EQ(doori::Bigdecimal("2"), r1);

    doori::Bigdecimal v3{"100"};
    doori::Bigdecimal v4{"222"};
    auto r2=v3*v4;
    EXPECT_EQ(doori::Bigdecimal("22200"), r2);

    doori::Bigdecimal v5{"998929390101010"};
    doori::Bigdecimal v6{"100012030120000"};
    auto r3=v5*v6;
    EXPECT_EQ(doori::Bigdecimal("99904956250535441962421200000"), r3);

    doori::Bigdecimal v7{"100.001"};
    doori::Bigdecimal v8{"1234"};
    auto r4=v7*v8;
    EXPECT_EQ(doori::Bigdecimal("123401.234"), r4);

    doori::Bigdecimal v9{"0.9392"};
    doori::Bigdecimal v10{"-1000.00212"};
    auto r5=v9*v10;
    EXPECT_EQ(doori::Bigdecimal("-939.201991104"), r5);

    doori::Bigdecimal v11{"-120.1234"};
    doori::Bigdecimal v12{"-1273.123909"};
    auto r6=v11*v12;
    EXPECT_EQ(doori::Bigdecimal("152931.9725703706"), r6);

    doori::Bigdecimal v13{"-0.1"};
    doori::Bigdecimal v14{"0.1"};
    auto r7=v13*v14;
    EXPECT_EQ(doori::Bigdecimal("-0.01"), r7);

    doori::Bigdecimal v15{"1"};
    doori::Bigdecimal v16{"-1"};
    auto r8=v15*v16;
    EXPECT_EQ(doori::Bigdecimal("-1"), r8);

    doori::Bigdecimal v151{"-1"};
    doori::Bigdecimal v161{"1"};
    auto r81=v151*v161;
    EXPECT_EQ(doori::Bigdecimal("-1"), r81);

    doori::Bigdecimal v17{"0"};
    doori::Bigdecimal v18{"0.1"};
    auto r9=v17*v18;
    EXPECT_EQ(doori::Bigdecimal("0"), r9);

    doori::Bigdecimal v19{"0"};
    doori::Bigdecimal v20{"0"};
    auto r10=v19*v20;
    EXPECT_EQ(doori::Bigdecimal("0"), r10);
}