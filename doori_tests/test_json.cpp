//
// Created by jaeseong on 20. 5. 25..
//

#include <gtest/gtest.h>
#include <Json.h>
#include <iostream>

TEST(Json_valueTestCase, PositiveTest)
{
    doori::Json_value value{1111};
    EXPECT_EQ("1111", value.toString());

    doori::Json_value value1{"const char*"};
    EXPECT_EQ("\"const char*\"", value1.toString());

    std::string str{"std::string"};
    doori::Json_value value2{str};
    EXPECT_EQ("\"std::string\"", value2.toString());

    doori::Json_value value3{"const char*"};
    EXPECT_EQ("\"const char*\"", value3.toString());

    doori::Json_value value4{0.1234f};
    EXPECT_EQ("0.1234", value4.toString());

    doori::Json_value value5{doori::Json_value{2222}};
    EXPECT_EQ("2222", value5.toString());

    doori::Json_value value6{ value };
    EXPECT_EQ("1111", value6.toString());

    doori::Json_value value7{ true };
    EXPECT_EQ("true", value7.toString());
}

TEST(Json_valueAppendTest, PositiveTest)
{
    doori::Json_value value;
    value.append({1111});
    value.append({"leejaeseong"});
    value.append({0.2f});
    value.append({true});

    auto result=R"([1111,"leejaeseong",0.2,true])";
    EXPECT_EQ(result,value.toString() );

    doori::Json json;
    json["1"] = 1234;
    json["2"] = "leejaeseong";

    value.append(json);
    auto result2=R"([1111,"leejaeseong",0.2,true,{"1":1234,"2":"leejaeseong"}])";
    EXPECT_EQ(result2,value.toString() );
}

TEST(Json_valueSetTest, PositiveTest)
{
    doori::Json_value   value;
    value.set(1111);
    EXPECT_EQ("1111", value.toString());

    value.set("const char*");
    EXPECT_EQ("\"const char*\"", value.toString());

    value.set("const\"leejaeseong");
    EXPECT_EQ("\"const\\\"leejaeseong\"", value.toString());

    std::string str{"std::string"};
    value.set(str);
    EXPECT_EQ("\"std::string\"", value.toString());

    value.set(0.1234f);
    EXPECT_EQ("0.1234", value.toString());

    value.set(doori::Json_value{2222});
    EXPECT_EQ("2222", value.toString());

    value.set( value );
    EXPECT_EQ("2222", value.toString());

    value.set(true);
    EXPECT_EQ("true", value.toString());
}

TEST(JsonTest, PositiveTest)
{
    doori::Json json, another;
    json["key"] = 1234;
    EXPECT_EQ("{\"key\":1234}", json.serialize());

    json["float"] = 0.1234f;
    EXPECT_EQ("{\"key\":1234,\"float\":0.1234}", json.serialize());

    json["leejaeseong"] = "string";
    EXPECT_EQ("{\"key\":1234,\"float\":0.1234,\"leejaeseong\":\"string\"}", json.serialize());

    another["haha"] = "lee,\":here";
    EXPECT_EQ("{\"haha\":\"lee,\\\":here\"}", another.serialize());
}

TEST(JsonMixTest, PositiveTest)
{
    doori::Json json, sub_json;

    std::string literalString=R"({"1":1111})";
    json.append("1", doori::Json_value(1111));
    EXPECT_EQ(literalString, json.serialize());
    json.clear();

    literalString=R"({"2":"2222"})";
    json.append("2", doori::Json_value("2222"));
    EXPECT_EQ(literalString, json.serialize());
    json.clear();

    literalString=R"({"3":0.1})";
    json.append("3", doori::Json_value(0.1f));
    EXPECT_EQ(literalString, json.serialize());
    json.clear();

    literalString=R"({"4":"leejaeseong"})";
    json.append("4", doori::Json_value("leejaeseong"));
    EXPECT_EQ(literalString, json.serialize());
    json.clear();

    literalString=R"({"1":1,"2":{"2":"2"}})";
    json.append("1", doori::Json_value(1));
    sub_json.append("2","2");
    json["2"]=sub_json;
    EXPECT_EQ(literalString,json.serialize());
    json.clear();
}

TEST(JsonConvertedTest, PositiveTest)
{
//    auto literalString=R"({"1"  :  ",:1" , "2":{"2":"2"}})";
//    auto literalString2=R"({"1":",:1","2":{"2":"2"}})";
//    doori::Json json;
//    json.unserialize(literalString);
//
//    EXPECT_EQ(literalString2, json.serialize());
//
//    json.clear();
//    auto l1=R"({"leejaeseong":{"2":"2"},"1":11111})";
//    json.unserialize(l1);
//    EXPECT_EQ(l1,json.serialize());
//
//    json.clear();
//    auto l2=R"({"glossary":{"title":"exampleglossary","GlossDiv":{"title":"S","GlossList":{"GlossEntry":{"ID":"SGML","SortAs":"SGML","GlossTerm":"StandardGeneralizedMarkupLanguage","Acronym":"SGML","Abbrev":"ISO8879:1986","GlossDef":{"para":"Ameta-markuplanguage,usedtocreatemarkuplanguagessuchasDocBook."},"GlossSee":"markup"}}}}})";
//    json.unserialize(l2);
//    EXPECT_EQ(l2,json.serialize());
//
//    std::cout<<"================"<<std::endl;
//    std::cout<<l2<<std::endl;
//    std::cout<<json.serialize()<<std::endl;
//    std::cout<<"================"<<std::endl;

    doori::Json json;
    json.clear();
    auto l3=R"({"title":"exampleglossary","GlossDiv":{"title":"S","GlossList":"GlossEntry"},"para":"A meta-markup language, used to create markup languages such as DocBook.", "GlossSeeAlso": ["GML", "XML"] } )";
    auto l4=R"({"title":"exampleglossary","GlossDiv":{"title":"S","GlossList":"GlossEntry"},"para":"A meta-markup language, used to create markup languages such as DocBook.", "GlossSeeAlso": ["GML", "XML"] })";
    json.unserialize(l3);
    EXPECT_EQ(l4,json.serialize());
}
