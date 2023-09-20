//
// Created by jaeseong on 20. 5. 25..
//

#include <gtest/gtest.h>
#include "Data/Json.h"
#include <iostream>

using namespace doori::api;

TEST(Json_value, ConstructTest)
{
    int intContainer;
    Data::Json_value value{1111};
    value.IntoInt(intContainer);
    EXPECT_EQ(1111, intContainer);
    EXPECT_EQ("1111", value.ToString());

    string strContainer;
    Data::Json_value value1{"const char*"};
    value1.IntoString(strContainer);
    EXPECT_EQ("const char*", strContainer);
    EXPECT_EQ("const char*", value1.ToString());

    std::string str{"std::string"};
    Data::Json_value value2{str};
    value2.IntoString(strContainer);
    EXPECT_EQ("std::string", strContainer);
    EXPECT_EQ("std::string", value2.ToString());


    float floatContainer;
    Data::Json_value value4{0.1234f};
    value4.IntoFloat(floatContainer);
    EXPECT_FLOAT_EQ(0.1234, floatContainer);
    EXPECT_EQ("0.1234", value4.ToString());

    Data::Json_value value5{ Data::Json_value{2222} };
    value5.IntoInt(intContainer);
    EXPECT_EQ(2222, intContainer);
    EXPECT_EQ("2222", value5.ToString());

    Data::Json_value value6{ value };
    value6.IntoInt(intContainer);
    EXPECT_EQ(1111, intContainer);
    EXPECT_EQ("1111", value6.ToString());

    bool boolContainer;
    Data::Json_value value7{ true };
    value7.IntoBool(boolContainer);
    EXPECT_EQ(true, boolContainer);
    EXPECT_EQ("true", value7.ToString());
}

TEST(Json_value, ArrayTest)
{
    Data::Json_value value;
    value.append({1111});
    value.append({"name"});
    value.append({0.2f});
    value.append({true});

    auto result=R"([1111,"name",0.2,true])";
    EXPECT_EQ(result,value.ToString() );

    Data::Json json;
    json["1"] = 1234;
    json["2"] = "name";

    value.append(json);
    auto result2=R"([1111,"name",0.2,true,{"1":1234,"2":"name"}])";
    EXPECT_EQ(result2,value.ToString() );
}

TEST(Json_value, SetTest)
{
    Data::Json_value   value;
    value.set(1111);
    EXPECT_EQ("1111", value.ToString());

    value.set("const char*");
    EXPECT_EQ("const char*", value.ToString());

    value.set("const\"name");
    EXPECT_EQ("const\\\"name", value.ToString());

    std::string str{"std::string"};
    value.set(str);
    EXPECT_EQ("std::string", value.ToString());

    value.set(0.1234f);
    EXPECT_EQ("0.1234", value.ToString());

    value.set(Data::Json_value{2222});
    EXPECT_EQ("2222", value.ToString());

    value.set( value );
    EXPECT_EQ("2222", value.ToString());

    value.set(true);
    EXPECT_EQ("true", value.ToString());
}

TEST(Json, OperatorTest)
{
    Data::Json json, another;
    json["key"] = 1234;
    EXPECT_EQ("{\"key\":1234}", json.serialize());

    json["float"] = 0.1234f;
    EXPECT_EQ("{\"key\":1234,\"float\":0.1234}", json.serialize());

    json["name"] = "string";
    EXPECT_EQ("{\"key\":1234,\"float\":0.1234,\"name\":\"string\"}", json.serialize());

    another["haha"] = "lee,\":here";
    EXPECT_EQ("{\"haha\":\"lee,\\\":here\"}", another.serialize());
}

TEST(Json, ComplexTest)
{
    Data::Json json, sub_json;

    std::string literalString=R"({"1":1111})";
    json.append("1", Data::Json_value(1111));
    EXPECT_EQ(literalString, json.serialize());
    json.clear();

    literalString=R"({"2":"2222"})";
    json.append("2", Data::Json_value("2222"));
    EXPECT_EQ(literalString, json.serialize());
    json.clear();

    literalString=R"({"3":0.1})";
    json.append("3", Data::Json_value(0.1f));
    EXPECT_EQ(literalString, json.serialize());
    json.clear();

    literalString=R"({"4":"name"})";
    json.append("4", Data::Json_value("name"));
    EXPECT_EQ(literalString, json.serialize());
    json.clear();

    literalString=R"({"1":1,"2":{"2":"2"}})";
    json.append("1", Data::Json_value(1));
    sub_json.append("2","2");
    json["2"]=sub_json;
    EXPECT_EQ(literalString,json.serialize());
    json.clear();
}

TEST(Json, ParserTest)
{
    auto literalString=R"({"1"  :  ",:1" , "2":{"2":"2"}})";
    auto literalString2=R"({"1":",:1","2":{"2":"2"}})";
    Data::Json json;
    json.unserialize(literalString);
    EXPECT_EQ(literalString2, json.serialize());

    json.clear();
    literalString2=R"({"1":",:1","2":{"2":2}})";
    json.unserialize(literalString2);
    auto value = json["1"];
    EXPECT_EQ(value.TYPE, value.STRING);

    auto value1 = json["2"];
    EXPECT_EQ(value1.TYPE, value1.JSON);
    EXPECT_EQ(value1.ToString(), R"({"2":2})");
    Data::Json another;
    another.unserialize(value1.ToString());

    EXPECT_EQ("2", another["2"].ToString());

    json.clear();
    auto l1=R"({"name":{"2":"2"},"1":11111})";
    json.unserialize(l1);
    EXPECT_EQ(l1,json.serialize());

    json.clear();
    auto l2=R"({"glossary":{"title":"exampleglossary","GlossDiv":{"title":"S","GlossList":{"GlossEntry":{"ID":"SGML","SortAs":"SGML","GlossTerm":"StandardGeneralizedMarkupLanguage","Acronym":"SGML","Abbrev":"ISO8879:1986","GlossDef":{"para":"Ameta-markuplanguage,usedtocreatemarkuplanguagessuchasDocBook."},"GlossSee":"markup"}}}}})";
    json.unserialize(l2);
    EXPECT_EQ(l2,json.serialize());

    json.clear();
    auto l3=R"({"title"     :   "exampleglossary","GlossDiv":{"title":"S"   ,  "GlossList":"GlossEntry"},"para"   : "A meta-markup language, used to create markup languages such as DocBook.", "GlossSeeAlso": ["GML", "XML"] } )";
    auto l4=R"({"title":"exampleglossary","GlossDiv":{"title":"S","GlossList":"GlossEntry"},"para":"A meta-markup language, used to create markup languages such as DocBook.","GlossSeeAlso":["GML","XML"]})";
    json.unserialize(l3);
    EXPECT_EQ(l4,json.serialize());
}
