//
// Created by jaeseong on 22. 6. 2..
//

#include <gtest/gtest.h>
#include "Etc/Bigdecimal.h"
#include <iostream>

using namespace std;
using namespace doori::api::Etc;

TEST(Bigdecimal, ConstructTest) {
    Bigdecimal v1{"5"};
    EXPECT_EQ("5", v1.toString());

    Bigdecimal v2{"999999999999999999999999999999999999999999999999999999999"};
    EXPECT_EQ("999999999999999999999999999999999999999999999999999999999", v2.toString());

    Bigdecimal v3{"-9"};
    EXPECT_EQ("-9", v3.toString());

    Bigdecimal v4{"-00000000000000000000000000000000000000"};
    EXPECT_EQ("0", v4.toString());

    Bigdecimal v5{"+0"};
    EXPECT_EQ("0", v5.toString());

    Bigdecimal v6{"+12345"};
    EXPECT_EQ("12345", v6.toString());

    Bigdecimal v7{"-123456"};
    EXPECT_EQ("-123456", v7.toString());

    Bigdecimal v8{"   -123456"};
    EXPECT_EQ("-123456", v8.toString());

    Bigdecimal v9{"   +123456"};
    EXPECT_EQ("123456", v9.toString());

    Bigdecimal v10{"   +00000000000000000000000"};
    EXPECT_EQ("0", v10.toString());

    Bigdecimal v11{"   -00000"};
    EXPECT_EQ("0", v11.toString());

    Bigdecimal v12{"   -0.000"};
    EXPECT_EQ("0", v12.toString());

    Bigdecimal v13{"   +0.000"};
    EXPECT_EQ("0", v13.toString());

    Bigdecimal v14{"   00.1234"};
    EXPECT_EQ("0.1234", v14.toString());

    Bigdecimal v15{"+00.1234"};
    EXPECT_EQ("0.1234", v15.toString());

    Bigdecimal v16{"100.0000"};
    EXPECT_EQ("100", v16.toString());

    Bigdecimal v17{"100.1000"};
    EXPECT_EQ("100.1", v17.toString());

    Bigdecimal v18{"0.000000"};
    EXPECT_EQ("0", v18.toString());

    Bigdecimal v19{"-0.8999999"};
    EXPECT_EQ("-0.8999999", v19.toString());

    Bigdecimal v20{"034"};
    EXPECT_EQ("34", v20.toString());
}

TEST(Bigdecimal, GT) {
    Bigdecimal v1{"348"};
    Bigdecimal v2{"123"};
    // value1 > value2
    EXPECT_GT(v1, v2);

    Bigdecimal v11{"123"};
    Bigdecimal v22{"-123"};
    // value1 > value2
    EXPECT_GT(v11, v22);

    Bigdecimal v111{"0"};
    Bigdecimal v222{"-99999999999999999999999999999999999999999999999999999999999999999999"};
    // value1 > value2
    EXPECT_GT(v111, v222);

    Bigdecimal f1{"0.110000"};
    Bigdecimal f2{"0.100001"};
    // value1 > value2
    EXPECT_GT(f1, f2);

    Bigdecimal f11{"0.110000"};
    Bigdecimal f22{"-0.100001"};
    // value1 > value2
    EXPECT_GT(f11, f22);

    Bigdecimal f3{"1"};
    Bigdecimal f4{"0.100001"};
    // value1 > value2
    EXPECT_GT(f3, f4);
}

TEST(Bigdecimal, GE) {
    Bigdecimal v1{"0"};
    Bigdecimal v2{"0"};
    // value1 >= value2
    EXPECT_GE(v1, v2);

    Bigdecimal v3{"0"};
    Bigdecimal v4{"-99999999999999"};
    // value1 >= value2
    EXPECT_GE(v1, v2);

    Bigdecimal value3{"192394959192394959192394959192394959192394959192394959192394959192394959"};
    Bigdecimal value4{"192394959192394959192394959192394959192394959192394959192394959192394959"};
    // value1 >= value2
    EXPECT_GE(value3, value4);

    Bigdecimal v5{"-123.0234"};
    Bigdecimal v6{"-123.1111"};
    EXPECT_GE(v5, v6);
}

TEST(Bigdecimal, MINUS) {
    Bigdecimal v1{"348.87"};
    Bigdecimal v2{"123.9"};
    auto r = v1 - v2;
    EXPECT_EQ(Bigdecimal("224.97"), r);

    Bigdecimal v3{"1.0000001"};
    Bigdecimal v4{"1.9"};
    auto k = v3 - v4;
    EXPECT_EQ(Bigdecimal("-0.8999999"), k);

    Bigdecimal m1{"23"};
    Bigdecimal m2{"348"};
    EXPECT_EQ(Bigdecimal("-325"), m1 - m2);

    EXPECT_EQ(Bigdecimal("1"), Bigdecimal("-1") - Bigdecimal("-2"));
    auto v = Bigdecimal("100") - Bigdecimal("66");
    EXPECT_EQ(Bigdecimal("34"), v);
    EXPECT_EQ(Bigdecimal("10"), Bigdecimal("-1") - Bigdecimal("-11"));
}

TEST(Bigdecimal, PLUS) {
    Bigdecimal v1{"1"};
    Bigdecimal v2{"2"};
    auto r1 = v1 + v2;
    EXPECT_EQ(Bigdecimal("3"), r1);

    Bigdecimal v3{"99291929394"};
    Bigdecimal v4{"99291929394"};
    auto r2 = v3 + v4;
    EXPECT_EQ(Bigdecimal("198583858788"), r2);

    Bigdecimal v5{"1"};
    Bigdecimal v6{"-1000"};
    auto r3 = v5 + v6;
    EXPECT_EQ(Bigdecimal("-999"), r3);

    Bigdecimal v7{"120.1234"};
    Bigdecimal v8{"1273.123909"};
    auto r4 = v7 + v8;
    EXPECT_EQ(Bigdecimal("1393.247309"), r4);

    Bigdecimal v9{"-120.1234"};
    Bigdecimal v10{"1273.123909"};
    auto r5 = v9 + v10;
    EXPECT_EQ(Bigdecimal("1153.000509"), r5);

    Bigdecimal v11{"120.1234"};
    Bigdecimal v12{"-1273.123909"};
    auto r6 = v11 + v12;
    EXPECT_EQ(Bigdecimal("-1153.000509"), r6);

    Bigdecimal v13{"0.1"};
    Bigdecimal v14{"0.1"};
    auto r7 = v13 + v14;
    EXPECT_EQ(Bigdecimal("0.2"), r7);

    Bigdecimal v15{"0"};
    Bigdecimal v16{"0"};
    auto r8 = v15 + v16;
    EXPECT_EQ(Bigdecimal("0"), r8);

    Bigdecimal v17{"0"};
    Bigdecimal v18{"-1"};
    auto r9 = v17 + v18;
    EXPECT_EQ(Bigdecimal("-1"), r9);

    Bigdecimal v19{"-1"};
    Bigdecimal v20{"-1"};
    auto r10 = v19 + v20;
    EXPECT_EQ(Bigdecimal("-2"), r10);

    Bigdecimal a1{"832"};
    Bigdecimal b1{"-465"};
    EXPECT_EQ(Bigdecimal{"367"}, a1 + b1);

    Bigdecimal a2{"-988"};
    Bigdecimal b2{"612"};
    EXPECT_EQ(Bigdecimal{"-376"}, a2 + b2);

    Bigdecimal a3{"285"};
    Bigdecimal b3{"-409"};
    EXPECT_EQ(Bigdecimal{"-124"}, a3 + b3);

    Bigdecimal a4{"514"};
    Bigdecimal b4{"-836"};
    EXPECT_EQ(Bigdecimal{"-322"}, a4 + b4);

    Bigdecimal a5{"162"};
    Bigdecimal b5{"216"};
    EXPECT_EQ(Bigdecimal{"378"}, a5 + b5);

    Bigdecimal a6{"926"};
    Bigdecimal b6{"-296"};
    EXPECT_EQ(Bigdecimal{"630"}, a6 + b6);

    Bigdecimal a7{"-993"};
    Bigdecimal b7{"-978"};
    EXPECT_EQ(Bigdecimal{"-1971"}, a7 + b7);

    Bigdecimal a8{"244"};
    Bigdecimal b8{"943"};
    EXPECT_EQ(Bigdecimal{"1187"}, a8 + b8);

    Bigdecimal a9{"852"};
    Bigdecimal b9{"79"};
    EXPECT_EQ(Bigdecimal{"931"}, a9 + b9);

    Bigdecimal a10{"-310"};
    Bigdecimal b10{"-783"};
    EXPECT_EQ(Bigdecimal{"-1093"}, a10 + b10);

    Bigdecimal a11{"-194"};
    Bigdecimal b11{"815"};
    EXPECT_EQ(Bigdecimal{"621"}, a11 + b11);

    Bigdecimal a12{"359"};
    Bigdecimal b12{"-133"};
    EXPECT_EQ(Bigdecimal{"226"}, a12 + b12);

    Bigdecimal a13{"573"};
    Bigdecimal b13{"468"};
    EXPECT_EQ(Bigdecimal{"1041"}, a13 + b13);

    Bigdecimal a14{"-315"};
    Bigdecimal b14{"-843"};
    EXPECT_EQ(Bigdecimal{"-1158"}, a14 + b14);

    Bigdecimal a15{"257"};
    Bigdecimal b15{"544"};
    EXPECT_EQ(Bigdecimal{"801"}, a15 + b15);

    Bigdecimal a16{"808"};
    Bigdecimal b16{"867"};
    EXPECT_EQ(Bigdecimal{"1675"}, a16 + b16);

    Bigdecimal a17{"293"};
    Bigdecimal b17{"925"};
    EXPECT_EQ(Bigdecimal{"1218"}, a17 + b17);

    Bigdecimal a18{"431"};
    Bigdecimal b18{"-814"};
    EXPECT_EQ(Bigdecimal{"-383"}, a18 + b18);

    Bigdecimal a19{"67"};
    Bigdecimal b19{"-315"};
    EXPECT_EQ(Bigdecimal{"-248"}, a19 + b19);

    Bigdecimal a20{"93"};
    Bigdecimal b20{"-821"};
    EXPECT_EQ(Bigdecimal{"-728"}, a20 + b20);

    Bigdecimal a21{"-923"};
    Bigdecimal b21{"221"};
    EXPECT_EQ(Bigdecimal{"-702"}, a21 + b21);

    Bigdecimal a22{"-631"};
    Bigdecimal b22{"-567"};
    EXPECT_EQ(Bigdecimal{"-1198"}, a22 + b22);

    Bigdecimal a23{"673"};
    Bigdecimal b23{"560"};
    EXPECT_EQ(Bigdecimal{"1233"}, a23 + b23);

    Bigdecimal a24{"-506"};
    Bigdecimal b24{"246"};
    EXPECT_EQ(Bigdecimal{"-260"}, a24 + b24);

    Bigdecimal a25{"740"};
    Bigdecimal b25{"-592"};
    EXPECT_EQ(Bigdecimal{"148"}, a25 + b25);

    Bigdecimal a26{"-967"};
    Bigdecimal b26{"310"};
    EXPECT_EQ(Bigdecimal{"-657"}, a26 + b26);

    Bigdecimal a27{"501"};
    Bigdecimal b27{"-562"};
    EXPECT_EQ(Bigdecimal{"-61"}, a27 + b27);

    Bigdecimal a28{"-786"};
    Bigdecimal b28{"783"};
    EXPECT_EQ(Bigdecimal{"-3"}, a28 + b28);

    Bigdecimal a29{"321"};
    Bigdecimal b29{"-864"};
    EXPECT_EQ(Bigdecimal{"-543"}, a29 + b29);

    Bigdecimal a30{"491"};
    Bigdecimal b30{"-620"};
    EXPECT_EQ(Bigdecimal{"-129"}, a30 + b30);

    Bigdecimal a31{"-184"};
    Bigdecimal b31{"60"};
    EXPECT_EQ(Bigdecimal{"-124"}, a31 + b31);

    Bigdecimal a32{"966"};
    Bigdecimal b32{"816"};
    EXPECT_EQ(Bigdecimal{"1782"}, a32 + b32);

    Bigdecimal a33{"-87"};
    Bigdecimal b33{"845"};
    EXPECT_EQ(Bigdecimal{"758"}, a33 + b33);

    Bigdecimal a34{"-263"};
    Bigdecimal b34{"-244"};
    EXPECT_EQ(Bigdecimal{"-507"}, a34 + b34);

    Bigdecimal a35{"933"};
    Bigdecimal b35{"-602"};
    EXPECT_EQ(Bigdecimal{"331"}, a35 + b35);

    Bigdecimal a36{"94"};
    Bigdecimal b36{"187"};
    EXPECT_EQ(Bigdecimal{"281"}, a36 + b36);

    Bigdecimal a37{"-72"};
    Bigdecimal b37{"-428"};
    EXPECT_EQ(Bigdecimal{"-500"}, a37 + b37);

    Bigdecimal a38{"913"};
    Bigdecimal b38{"-3"};
    EXPECT_EQ(Bigdecimal{"910"}, a38 + b38);

    Bigdecimal a39{"-718"};
    Bigdecimal b39{"693"};
    EXPECT_EQ(Bigdecimal{"-25"}, a39 + b39);

    Bigdecimal a40{"-761"};
    Bigdecimal b40{"-906"};
    EXPECT_EQ(Bigdecimal{"-1667"}, a40 + b40);

    Bigdecimal a41{"878"};
    Bigdecimal b41{"494"};
    EXPECT_EQ(Bigdecimal{"1372"}, a41 + b41);

    Bigdecimal a42{"-339"};
    Bigdecimal b42{"-851"};
    EXPECT_EQ(Bigdecimal{"-1190"}, a42 + b42);

    Bigdecimal a43{"-918"};
    Bigdecimal b43{"-180"};
    EXPECT_EQ(Bigdecimal{"-1098"}, a43 + b43);

    Bigdecimal a44{"-265"};
    Bigdecimal b44{"622"};
    EXPECT_EQ(Bigdecimal{"357"}, a44 + b44);

    Bigdecimal a45{"-689"};
    Bigdecimal b45{"269"};
    EXPECT_EQ(Bigdecimal{"-420"}, a45 + b45);

    Bigdecimal a46{"-847"};
    Bigdecimal b46{"72"};
    EXPECT_EQ(Bigdecimal{"-775"}, a46 + b46);

    Bigdecimal a47{"941"};
    Bigdecimal b47{"-848"};
    EXPECT_EQ(Bigdecimal{"93"}, a47 + b47);

    Bigdecimal a48{"781"};
    Bigdecimal b48{"-781"};
    EXPECT_EQ(Bigdecimal{"0"}, a48 + b48);

    Bigdecimal a49{"978"};
    Bigdecimal b49{"-965"};
    EXPECT_EQ(Bigdecimal{"13"}, a49 + b49);

    Bigdecimal a50{"-120"};
    Bigdecimal b50{"692"};
    EXPECT_EQ(Bigdecimal{"572"}, a50 + b50);

    Bigdecimal a51{"707"};
    Bigdecimal b51{"177"};
    EXPECT_EQ(Bigdecimal{"884"}, a51 + b51);

    Bigdecimal a52{"-517"};
    Bigdecimal b52{"-625"};
    EXPECT_EQ(Bigdecimal{"-1142"}, a52 + b52);

    Bigdecimal a53{"834"};
    Bigdecimal b53{"307"};
    EXPECT_EQ(Bigdecimal{"1141"}, a53 + b53);

    Bigdecimal a54{"422"};
    Bigdecimal b54{"-482"};
    EXPECT_EQ(Bigdecimal{"-60"}, a54 + b54);

    Bigdecimal a55{"870"};
    Bigdecimal b55{"-326"};
    EXPECT_EQ(Bigdecimal{"544"}, a55 + b55);

    Bigdecimal a56{"157"};
    Bigdecimal b56{"-891"};
    EXPECT_EQ(Bigdecimal{"-734"}, a56 + b56);

    Bigdecimal a57{"-264"};
    Bigdecimal b57{"-654"};
    EXPECT_EQ(Bigdecimal{"-918"}, a57 + b57);

    Bigdecimal a58{"-702"};
    Bigdecimal b58{"-640"};
    EXPECT_EQ(Bigdecimal{"-1342"}, a58 + b58);

    Bigdecimal a59{"-338"};
    Bigdecimal b59{"664"};
    EXPECT_EQ(Bigdecimal{"326"}, a59 + b59);

    Bigdecimal a60{"-474"};
    Bigdecimal b60{"665"};
    EXPECT_EQ(Bigdecimal{"191"}, a60 + b60);

    Bigdecimal a61{"568"};
    Bigdecimal b61{"578"};
    EXPECT_EQ(Bigdecimal{"1146"}, a61 + b61);

    Bigdecimal a62{"-912"};
    Bigdecimal b62{"-528"};
    EXPECT_EQ(Bigdecimal{"-1440"}, a62 + b62);

    Bigdecimal a63{"-294"};
    Bigdecimal b63{"-392"};
    EXPECT_EQ(Bigdecimal{"-686"}, a63 + b63);

    Bigdecimal a64{"-892"};
    Bigdecimal b64{"-911"};
    EXPECT_EQ(Bigdecimal{"-1803"}, a64 + b64);

    Bigdecimal a65{"-501"};
    Bigdecimal b65{"430"};
    EXPECT_EQ(Bigdecimal{"-71"}, a65 + b65);

    Bigdecimal a66{"976"};
    Bigdecimal b66{"695"};
    EXPECT_EQ(Bigdecimal{"1671"}, a66 + b66);

    Bigdecimal a67{"-643"};
    Bigdecimal b67{"863"};
    EXPECT_EQ(Bigdecimal{"220"}, a67 + b67);

    Bigdecimal a68{"787"};
    Bigdecimal b68{"-95"};
    EXPECT_EQ(Bigdecimal{"692"}, a68 + b68);

    Bigdecimal a69{"868"};
    Bigdecimal b69{"50"};
    EXPECT_EQ(Bigdecimal{"918"}, a69 + b69);

    Bigdecimal a70{"301"};
    Bigdecimal b70{"-187"};
    EXPECT_EQ(Bigdecimal{"114"}, a70 + b70);

    Bigdecimal a71{"-734"};
    Bigdecimal b71{"-493"};
    EXPECT_EQ(Bigdecimal{"-1227"}, a71 + b71);

    Bigdecimal a72{"-252"};
    Bigdecimal b72{"-228"};
    EXPECT_EQ(Bigdecimal{"-480"}, a72 + b72);

    Bigdecimal a73{"755"};
    Bigdecimal b73{"-157"};
    EXPECT_EQ(Bigdecimal{"598"}, a73 + b73);

    Bigdecimal a74{"859"};
    Bigdecimal b74{"-647"};
    EXPECT_EQ(Bigdecimal{"212"}, a74 + b74);

    Bigdecimal a75{"269"};
    Bigdecimal b75{"591"};
    EXPECT_EQ(Bigdecimal{"860"}, a75 + b75);

    Bigdecimal a76{"868"};
    Bigdecimal b76{"819"};
    EXPECT_EQ(Bigdecimal{"1687"}, a76 + b76);

    Bigdecimal a77{"-523"};
    Bigdecimal b77{"589"};
    EXPECT_EQ(Bigdecimal{"66"}, a77 + b77);

    Bigdecimal a78{"-925"};
    Bigdecimal b78{"928"};
    EXPECT_EQ(Bigdecimal{"3"}, a78 + b78);

    Bigdecimal a79{"-229"};
    Bigdecimal b79{"-369"};
    EXPECT_EQ(Bigdecimal{"-598"}, a79 + b79);

    Bigdecimal a80{"-443"};
    Bigdecimal b80{"760"};
    EXPECT_EQ(Bigdecimal{"317"}, a80 + b80);

    Bigdecimal a81{"974"};
    Bigdecimal b81{"-226"};
    EXPECT_EQ(Bigdecimal{"748"}, a81 + b81);

    Bigdecimal a82{"-195"};
    Bigdecimal b82{"909"};
    EXPECT_EQ(Bigdecimal{"714"}, a82 + b82);

    Bigdecimal a83{"743"};
    Bigdecimal b83{"767"};
    EXPECT_EQ(Bigdecimal{"1510"}, a83 + b83);

    Bigdecimal a84{"213"};
    Bigdecimal b84{"-74"};
    EXPECT_EQ(Bigdecimal{"139"}, a84 + b84);

    Bigdecimal a85{"622"};
    Bigdecimal b85{"-847"};
    EXPECT_EQ(Bigdecimal{"-225"}, a85 + b85);

    Bigdecimal a86{"627"};
    Bigdecimal b86{"547"};
    EXPECT_EQ(Bigdecimal{"1174"}, a86 + b86);

    Bigdecimal a87{"917"};
    Bigdecimal b87{"598"};
    EXPECT_EQ(Bigdecimal{"1515"}, a87 + b87);

    Bigdecimal a88{"-268"};
    Bigdecimal b88{"336"};
    EXPECT_EQ(Bigdecimal{"68"}, a88 + b88);

    Bigdecimal a89{"685"};
    Bigdecimal b89{"602"};
    EXPECT_EQ(Bigdecimal{"1287"}, a89 + b89);

    Bigdecimal a90{"440"};
    Bigdecimal b90{"688"};
    EXPECT_EQ(Bigdecimal{"1128"}, a90 + b90);

    Bigdecimal a91{"-874"};
    Bigdecimal b91{"-158"};
    EXPECT_EQ(Bigdecimal{"-1032"}, a91 + b91);

    Bigdecimal a92{"161"};
    Bigdecimal b92{"-243"};
    EXPECT_EQ(Bigdecimal{"-82"}, a92 + b92);

    Bigdecimal a93{"-369"};
    Bigdecimal b93{"-443"};
    EXPECT_EQ(Bigdecimal{"-812"}, a93 + b93);

    Bigdecimal a94{"743"};
    Bigdecimal b94{"-194"};
    EXPECT_EQ(Bigdecimal{"549"}, a94 + b94);

    Bigdecimal a95{"145"};
    Bigdecimal b95{"-572"};
    EXPECT_EQ(Bigdecimal{"-427"}, a95 + b95);

    Bigdecimal a96{"435"};
    Bigdecimal b96{"323"};
    EXPECT_EQ(Bigdecimal{"758"}, a96 + b96);

    Bigdecimal a97{"726"};
    Bigdecimal b97{"436"};
    EXPECT_EQ(Bigdecimal{"1162"}, a97 + b97);

    Bigdecimal a98{"-739"};
    Bigdecimal b98{"-612"};
    EXPECT_EQ(Bigdecimal{"-1351"}, a98 + b98);

    Bigdecimal a99{"-30"};
    Bigdecimal b99{"-771"};
    EXPECT_EQ(Bigdecimal{"-801"}, a99 + b99);
}

TEST(Bigdecimal, float_multiply) {
    Bigdecimal a1{"50.81"};
    Bigdecimal b1{"-49.98"};
    EXPECT_EQ(Bigdecimal{"-2539.4838"}, a1 * b1);

    Bigdecimal a2{"97.92"};
    Bigdecimal b2{"-47.73"};
    EXPECT_EQ(Bigdecimal{"-4673.7216"}, a2 * b2);

    Bigdecimal a3{"55.02"};
    Bigdecimal b3{"-89.51"};
    EXPECT_EQ(Bigdecimal{"-4924.8402"}, a3 * b3);

    Bigdecimal a4{"76.01"};
    Bigdecimal b4{"-99.8"};
    EXPECT_EQ(Bigdecimal{"-7585.798"}, a4 * b4);

    Bigdecimal a5{"17.17"};
    Bigdecimal b5{"-5.85"};
    EXPECT_EQ(Bigdecimal{"-100.4445"}, a5 * b5);

    Bigdecimal a6{"90.87"};
    Bigdecimal b6{"23.55"};
    EXPECT_EQ(Bigdecimal{"2139.9885"}, a6 * b6);

    Bigdecimal a7{"-28.44"};
    Bigdecimal b7{"81.93"};
    EXPECT_EQ(Bigdecimal{"-2330.0892"}, a7 * b7);

    Bigdecimal a8{"-33.6"};
    Bigdecimal b8{"94.49"};
    EXPECT_EQ(Bigdecimal{"-3174.864"}, a8 * b8);

    Bigdecimal a9{"-59.7"};
    Bigdecimal b9{"-47.1"};
    EXPECT_EQ(Bigdecimal{"2811.87"}, a9 * b9);

    Bigdecimal a10{"-7.28"};
    Bigdecimal b10{"2.42"};
    EXPECT_EQ(Bigdecimal{"-17.6176"}, a10 * b10);

    Bigdecimal a11{"87.25"};
    Bigdecimal b11{"-57.35"};
    EXPECT_EQ(Bigdecimal{"-5003.7875"}, a11 * b11);

    Bigdecimal a12{"-60.72"};
    Bigdecimal b12{"-81.41"};
    EXPECT_EQ(Bigdecimal{"4943.2152"}, a12 * b12);

    Bigdecimal a13{"-99.38"};
    Bigdecimal b13{"49.38"};
    EXPECT_EQ(Bigdecimal{"-4907.3844"}, a13 * b13);

    Bigdecimal a14{"89.25"};
    Bigdecimal b14{"-50.7"};
    EXPECT_EQ(Bigdecimal{"-4524.975"}, a14 * b14);

    Bigdecimal a15{"-83.34"};
    Bigdecimal b15{"-67.7"};
    EXPECT_EQ(Bigdecimal{"5642.118"}, a15 * b15);

    Bigdecimal a16{"-41.93"};
    Bigdecimal b16{"-71.59"};
    EXPECT_EQ(Bigdecimal{"3001.7687"}, a16 * b16);

    Bigdecimal a17{"-98.86"};
    Bigdecimal b17{"-5.34"};
    EXPECT_EQ(Bigdecimal{"527.9124"}, a17 * b17);

    Bigdecimal a18{"4.53"};
    Bigdecimal b18{"75.61"};
    EXPECT_EQ(Bigdecimal{"342.5133"}, a18 * b18);

    Bigdecimal a19{"-93.01"};
    Bigdecimal b19{"77.62"};
    EXPECT_EQ(Bigdecimal{"-7219.4362"}, a19 * b19);

    Bigdecimal a20{"-36.46"};
    Bigdecimal b20{"92.55"};
    EXPECT_EQ(Bigdecimal{"-3374.373"}, a20 * b20);

    Bigdecimal a21{"-22.59"};
    Bigdecimal b21{"-59.43"};
    EXPECT_EQ(Bigdecimal{"1342.5237"}, a21 * b21);

    Bigdecimal a22{"-47.55"};
    Bigdecimal b22{"-18.28"};
    EXPECT_EQ(Bigdecimal{"869.214"}, a22 * b22);

    Bigdecimal a23{"-27.33"};
    Bigdecimal b23{"30.09"};
    EXPECT_EQ(Bigdecimal{"-822.3597"}, a23 * b23);

    Bigdecimal a24{"-53.02"};
    Bigdecimal b24{"91"};
    EXPECT_EQ(Bigdecimal{"-4824.82"}, a24 * b24);

    Bigdecimal a25{"82.21"};
    Bigdecimal b25{"26.54"};
    EXPECT_EQ(Bigdecimal{"2181.8534"}, a25 * b25);

    Bigdecimal a26{"-40.84"};
    Bigdecimal b26{"-21.62"};
    EXPECT_EQ(Bigdecimal{"882.9608"}, a26 * b26);

    Bigdecimal a27{"-53.4"};
    Bigdecimal b27{"-95.4"};
    EXPECT_EQ(Bigdecimal{"5094.36"}, a27 * b27);

    Bigdecimal a28{"34.84"};
    Bigdecimal b28{"46.41"};
    EXPECT_EQ(Bigdecimal{"1616.9244"}, a28 * b28);

    Bigdecimal a29{"26.04"};
    Bigdecimal b29{"59.09"};
    EXPECT_EQ(Bigdecimal{"1538.7036"}, a29 * b29);

    Bigdecimal a30{"-28.21"};
    Bigdecimal b30{"9.21"};
    EXPECT_EQ(Bigdecimal{"-259.8141"}, a30 * b30);
}

TEST(Bigdecimal, integer_multiply) {
    Bigdecimal a1{"9658"};
    Bigdecimal b1{"-4829"};
    EXPECT_EQ(Bigdecimal{"-46638482"}, a1 * b1);

    Bigdecimal a2{"8295"};
    Bigdecimal b2{"-9983"};
    EXPECT_EQ(Bigdecimal{"-82808985"}, a2 * b2);

    Bigdecimal a3{"-4427"};
    Bigdecimal b3{"-9798"};
    EXPECT_EQ(Bigdecimal{"43375746"}, a3 * b3);

    Bigdecimal a4{"-7085"};
    Bigdecimal b4{"9107"};
    EXPECT_EQ(Bigdecimal{"-64523095"}, a4 * b4);

    Bigdecimal a5{"-8149"};
    Bigdecimal b5{"-4509"};
    EXPECT_EQ(Bigdecimal{"36743841"}, a5 * b5);

    Bigdecimal a6{"-5954"};
    Bigdecimal b6{"2340"};
    EXPECT_EQ(Bigdecimal{"-13932360"}, a6 * b6);

    Bigdecimal a7{"-2631"};
    Bigdecimal b7{"-5280"};
    EXPECT_EQ(Bigdecimal{"13891680"}, a7 * b7);

    Bigdecimal a8{"-3161"};
    Bigdecimal b8{"-7891"};
    EXPECT_EQ(Bigdecimal{"24943451"}, a8 * b8);

    Bigdecimal a9{"-9613"};
    Bigdecimal b9{"8434"};
    EXPECT_EQ(Bigdecimal{"-81076042"}, a9 * b9);

    Bigdecimal a10{"4707"};
    Bigdecimal b10{"-95"};
    EXPECT_EQ(Bigdecimal{"-447165"}, a10 * b10);

    Bigdecimal a11{"-7095"};
    Bigdecimal b11{"4525"};
    EXPECT_EQ(Bigdecimal{"-32104875"}, a11 * b11);

    Bigdecimal a12{"-9441"};
    Bigdecimal b12{"-9412"};
    EXPECT_EQ(Bigdecimal{"88858692"}, a12 * b12);

    Bigdecimal a13{"-4885"};
    Bigdecimal b13{"-1207"};
    EXPECT_EQ(Bigdecimal{"5896195"}, a13 * b13);

    Bigdecimal a14{"-2409"};
    Bigdecimal b14{"4148"};
    EXPECT_EQ(Bigdecimal{"-9992532"}, a14 * b14);

    Bigdecimal a15{"2661"};
    Bigdecimal b15{"249"};
    EXPECT_EQ(Bigdecimal{"662589"}, a15 * b15);

    Bigdecimal a16{"-3954"};
    Bigdecimal b16{"-7044"};
    EXPECT_EQ(Bigdecimal{"27851976"}, a16 * b16);

    Bigdecimal a17{"-4945"};
    Bigdecimal b17{"5886"};
    EXPECT_EQ(Bigdecimal{"-29106270"}, a17 * b17);

    Bigdecimal a18{"-7240"};
    Bigdecimal b18{"-4136"};
    EXPECT_EQ(Bigdecimal{"29944640"}, a18 * b18);

    Bigdecimal a19{"-4993"};
    Bigdecimal b19{"-4346"};
    EXPECT_EQ(Bigdecimal{"21699578"}, a19 * b19);

    Bigdecimal a20{"-5058"};
    Bigdecimal b20{"-865"};
    EXPECT_EQ(Bigdecimal{"4375170"}, a20 * b20);

    Bigdecimal a21{"3209"};
    Bigdecimal b21{"-7812"};
    EXPECT_EQ(Bigdecimal{"-25068708"}, a21 * b21);

    Bigdecimal a22{"6119"};
    Bigdecimal b22{"5304"};
    EXPECT_EQ(Bigdecimal{"32455176"}, a22 * b22);

    Bigdecimal a23{"-4963"};
    Bigdecimal b23{"3170"};
    EXPECT_EQ(Bigdecimal{"-15732710"}, a23 * b23);

    Bigdecimal a24{"-7450"};
    Bigdecimal b24{"-2385"};
    EXPECT_EQ(Bigdecimal{"17768250"}, a24 * b24);

    Bigdecimal a25{"-7366"};
    Bigdecimal b25{"-937"};
    EXPECT_EQ(Bigdecimal{"6901942"}, a25 * b25);

    Bigdecimal a26{"-858"};
    Bigdecimal b26{"81"};
    EXPECT_EQ(Bigdecimal{"-69498"}, a26 * b26);

    Bigdecimal a27{"1430"};
    Bigdecimal b27{"-6937"};
    EXPECT_EQ(Bigdecimal{"-9919910"}, a27 * b27);

    Bigdecimal a28{"6570"};
    Bigdecimal b28{"3293"};
    EXPECT_EQ(Bigdecimal{"21635010"}, a28 * b28);

    Bigdecimal a29{"-3036"};
    Bigdecimal b29{"-2452"};
    EXPECT_EQ(Bigdecimal{"7444272"}, a29 * b29);

    Bigdecimal a30{"-462"};
    Bigdecimal b30{"-6322"};
    EXPECT_EQ(Bigdecimal{"2920764"}, a30 * b30);
}

TEST(Bigdecimal, float_divide) {
    Bigdecimal a1{"-19.39"};
    Bigdecimal b1{"94.21"};
    EXPECT_EQ(Bigdecimal{"-0.2058167922"}, a1 / b1);

    Bigdecimal a2{"-6.78"};
    Bigdecimal b2{"61.53"};
    EXPECT_EQ(Bigdecimal{"-0.1101901511"}, a2 / b2);

    Bigdecimal a3{"83.69"};
    Bigdecimal b3{"34.58"};
    EXPECT_EQ(Bigdecimal{"2.420185078"}, a3 / b3);

    Bigdecimal a4{"97.19"};
    Bigdecimal b4{"63.91"};
    EXPECT_EQ(Bigdecimal{"1.5207322797"}, a4 / b4);

    Bigdecimal a5{"88.89"};
    Bigdecimal b5{"44.96"};
    EXPECT_EQ(Bigdecimal{"1.9770907473"}, a5 / b5);

    Bigdecimal a6{"2.46"};
    Bigdecimal b6{"-92.79"};
    EXPECT_EQ(Bigdecimal{"-0.0265114775"}, a6 / b6);

    Bigdecimal a7{"82.22"};
    Bigdecimal b7{"33.78"};
    EXPECT_EQ(Bigdecimal{"2.4339846062"}, a7 / b7);

    Bigdecimal a8{"-50.83"};
    Bigdecimal b8{"89.57"};
    EXPECT_EQ(Bigdecimal{"-0.5674891146"}, a8 / b8);

    Bigdecimal a9{"-43.58"};
    Bigdecimal b9{"-89.91"};
    EXPECT_EQ(Bigdecimal{"0.4847069291"}, a9 / b9);

    Bigdecimal a10{"-10.01"};
    Bigdecimal b10{"1.81"};
    EXPECT_EQ(Bigdecimal{"-5.5303867403"}, a10 / b10);

    Bigdecimal a11{"-8.57"};
    Bigdecimal b11{"20.95"};
    EXPECT_EQ(Bigdecimal{"-0.4090692124"}, a11 / b11);

    Bigdecimal a12{"-23.31"};
    Bigdecimal b12{"59.83"};
    EXPECT_EQ(Bigdecimal{"-0.3896038776"}, a12 / b12);

    Bigdecimal a13{"-69.5"};
    Bigdecimal b13{"79.53"};
    EXPECT_EQ(Bigdecimal{"-0.8738840689"}, a13 / b13);

    Bigdecimal a14{"17.28"};
    Bigdecimal b14{"79.51"};
    EXPECT_EQ(Bigdecimal{"0.2173311533"}, a14 / b14);

    Bigdecimal a15{"-62.3"};
    Bigdecimal b15{"-57.56"};
    EXPECT_EQ(Bigdecimal{"1.0823488533"}, a15 / b15);

    Bigdecimal a16{"83.38"};
    Bigdecimal b16{"6.05"};
    EXPECT_EQ(Bigdecimal{"13.7818181818"}, a16 / b16);

    Bigdecimal a17{"41.62"};
    Bigdecimal b17{"38"};
    EXPECT_EQ(Bigdecimal{"1.0952631578"}, a17 / b17);

    Bigdecimal a18{"-75.58"};
    Bigdecimal b18{"12.81"};
    EXPECT_EQ(Bigdecimal{"-5.900078064"}, a18 / b18);

    Bigdecimal a19{"31.45"};
    Bigdecimal b19{"15.8"};
    EXPECT_EQ(Bigdecimal{"1.9905063291"}, a19 / b19);

    Bigdecimal a20{"26.75"};
    Bigdecimal b20{"16.68"};
    EXPECT_EQ(Bigdecimal{"1.6037170263"}, a20 / b20);

    Bigdecimal a21{"-16.6"};
    Bigdecimal b21{"31.1"};
    EXPECT_EQ(Bigdecimal{"-0.5337620578"}, a21 / b21);

    Bigdecimal a22{"-76.8"};
    Bigdecimal b22{"71.7"};
    EXPECT_EQ(Bigdecimal{"-1.0711297071"}, a22 / b22);

    Bigdecimal a23{"28.96"};
    Bigdecimal b23{"46.39"};
    EXPECT_EQ(Bigdecimal{"0.6242724725"}, a23 / b23);

    Bigdecimal a24{"-55.3"};
    Bigdecimal b24{"34.05"};
    EXPECT_EQ(Bigdecimal{"-1.624082232"}, a24 / b24);

    Bigdecimal a25{"-35.53"};
    Bigdecimal b25{"-62.32"};
    EXPECT_EQ(Bigdecimal{"0.5701219512"}, a25 / b25);

    Bigdecimal a26{"54.8"};
    Bigdecimal b26{"-81.92"};
    EXPECT_EQ(Bigdecimal{"-0.6689453125"}, a26 / b26);

    Bigdecimal a27{"-65.11"};
    Bigdecimal b27{"-38.14"};
    EXPECT_EQ(Bigdecimal{"1.7071316203"}, a27 / b27);

    Bigdecimal a28{"-24.62"};
    Bigdecimal b28{"-50.49"};
    EXPECT_EQ(Bigdecimal{"0.4876213111"}, a28 / b28);

    Bigdecimal a29{"39.04"};
    Bigdecimal b29{"64.7"};
    EXPECT_EQ(Bigdecimal{"0.6034003091"}, a29 / b29);

    Bigdecimal a30{"-82.35"};
    Bigdecimal b30{"16.32"};
    EXPECT_EQ(Bigdecimal{"-5.0459558823"}, a30 / b30);
}

TEST(Bigdecimal, design_multify) {
    Bigdecimal v1{"1"};
    Bigdecimal v2{"2"};
    auto r1 = v1 * v2;
    EXPECT_EQ(Bigdecimal("2"), r1);

    Bigdecimal v3{"100"};
    Bigdecimal v4{"222"};
    auto r2 = v3 * v4;
    EXPECT_EQ(Bigdecimal("22200"), r2);

    Bigdecimal v5{"998929390101010"};
    Bigdecimal v6{"100012030120000"};
    auto r3 = v5 * v6;
    EXPECT_EQ(Bigdecimal("99904956250535441962421200000"), r3);

    Bigdecimal v7{"100.001"};
    Bigdecimal v8{"1234"};
    auto r4 = v7 * v8;
    EXPECT_EQ(Bigdecimal("123401.234"), r4);

    Bigdecimal v9{"0.9392"};
    Bigdecimal v10{"-1000.00212"};
    auto r5 = v9 * v10;
    EXPECT_EQ(Bigdecimal("-939.201991104"), r5);

    Bigdecimal v11{"-120.1234"};
    Bigdecimal v12{"-1273.123909"};
    auto r6 = v11 * v12;
    EXPECT_EQ(Bigdecimal("152931.9725703706"), r6);

    Bigdecimal v13{"-0.1"};
    Bigdecimal v14{"0.1"};
    auto r7 = v13 * v14;
    EXPECT_EQ(Bigdecimal("-0.01"), r7);

    Bigdecimal v15{"1"};
    Bigdecimal v16{"-1"};
    auto r8 = v15 * v16;
    EXPECT_EQ(Bigdecimal("-1"), r8);

    Bigdecimal v151{"-1"};
    Bigdecimal v161{"1"};
    auto r81 = v151 * v161;
    EXPECT_EQ(Bigdecimal("-1"), r81);

    Bigdecimal v17{"0"};
    Bigdecimal v18{"0.1"};
    auto r9 = v17 * v18;
    EXPECT_EQ(Bigdecimal("0"), r9);

    Bigdecimal v19{"0"};
    Bigdecimal v20{"0"};
    auto r10 = v19 * v20;
    EXPECT_EQ(Bigdecimal("0"), r10);

    Bigdecimal v21{"2394293593245"};
    Bigdecimal v22{"16477"};
    auto r11 = v21 * v22;
    EXPECT_EQ(Bigdecimal{"39452130943849234"}, r11 + Bigdecimal{"1355407951369"});
}

TEST(Bigdecimal, design_divide) {
    Bigdecimal v3{"1"};
    Bigdecimal v4{"0"};
    EXPECT_ANY_THROW(v3 / v4);

    Bigdecimal v1{"1"};
    Bigdecimal v2{"1"};
    auto r1 = v1 / v2;
    EXPECT_EQ(Bigdecimal("1"), r1);

    Bigdecimal v11{"-1"};
    Bigdecimal v21{"1"};
    auto r11 = v11 / v21;
    EXPECT_EQ(Bigdecimal("-1"), r11);

    Bigdecimal v111{"-1"};
    Bigdecimal v211{"-1"};
    auto r111 = v111 / v211;
    EXPECT_EQ(Bigdecimal("1"), r111);

    Bigdecimal v1111{"1"};
    Bigdecimal v2111{"-1"};
    auto r1111 = v1111 / v2111;
    EXPECT_EQ(Bigdecimal("-1"), r1111);

    Bigdecimal v5{"2"};
    Bigdecimal v6{"7"};
    auto r2 = v5 / v6;
    EXPECT_EQ(Bigdecimal("0.2857142857"), r2);

    Bigdecimal v72{"5"};
    Bigdecimal v82{"2"};
    auto r32 = v72 / v82;
    EXPECT_EQ(Bigdecimal("2.5"), r32);

    Bigdecimal v7{"2"};
    Bigdecimal v8{"5"};
    auto r3 = v7 / v8;
    EXPECT_EQ(Bigdecimal("0.4"), r3);

    Bigdecimal v71{"-2"};
    Bigdecimal v81{"5"};
    auto r31 = v71 / v81;
    EXPECT_EQ(Bigdecimal("-0.4"), r31);

    Bigdecimal v9{"19293"};
    Bigdecimal v10{"7777"};
    auto r4 = v9 / v10;
    EXPECT_EQ(Bigdecimal("2.480776649"), r4);
}

TEST(Bigdecimal, design_complex) {
    EXPECT_EQ(
            Bigdecimal{"-10000000000000000000000000000"},
            Bigdecimal{"-9123912938129382934829348239"} / Bigdecimal{"0.9123912938129382934829348239"}
    );

    EXPECT_EQ(
            Bigdecimal{"9123912938129382934829348239"},
            Bigdecimal{"8324560980431615848446804538.000241607397553188962"} /
            Bigdecimal{"0.912389348394894584958"}
    );

    EXPECT_EQ(
            Bigdecimal{"0.9123893483"},
            Bigdecimal{"8324560980431615848446804538.000241607397553188962"} /
            Bigdecimal{"9123912938129382934829348239"}
    );

    EXPECT_EQ(
            Bigdecimal{"8324560980431615848446804538000241607397553188962"},
            Bigdecimal{"9123912938129382934829348239"} * Bigdecimal{"912389348394894584958"}
    );

    EXPECT_EQ(
            Bigdecimal{"9123912938129382934829348239"},
            (Bigdecimal{"9123912938129382934829348239"} + Bigdecimal{"9123912938129382934829348239"} +
             Bigdecimal{"9123912938129382934829348239"} + Bigdecimal{"9123912938129382934829348239"}) /
            Bigdecimal{"4"}
    );

    EXPECT_EQ(
            Bigdecimal{"9123912938129382934829348239"},
            Bigdecimal{"9123912938129382934829348239"} * Bigdecimal{"9123912938129382934829348239"} /
            Bigdecimal{"9123912938129382934829348239"}
    );
}
