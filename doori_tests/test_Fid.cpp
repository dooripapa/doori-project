//
// Created by jaeseong on 20. 5. 25..
//

#include <gtest/gtest.h>
#include "Data/Fid.h"
#include "Data/FidSegment.h"
#include <iostream>

using namespace doori::api;

TEST(Fid, ConstructTest)
{
    Data::Fid fid{};

    Data::FidSegment fidSegment1;
    fidSegment1.set(1, 100);
    Data::Fid fid1{fidSegment1};
    EXPECT_EQ("$|1,I=100|$", fid1.toString());

    Data::FidSegment fidSegment2;
    fidSegment2.set(2, 9L);
    Data::Fid fid2{fidSegment2};
    EXPECT_EQ("$|2,L=9|$", fid2.toString());

    Data::FidSegment fidSegment3;
    fidSegment3.set(3, 0.5795f);
    Data::Fid fid3{fidSegment3};
    EXPECT_EQ("$|3,F=0.579500|$", fid3.toString());

    Data::FidSegment fidSegment4;
    fidSegment4.set(4, 0.579254545);
    Data::Fid fid4{fidSegment4};
    EXPECT_EQ("$|4,D=0.579255|$", fid4.toString());

    Data::FidSegment fidSegment5;
    fidSegment5.set(5, false);
    Data::Fid fid5{fidSegment5};
    EXPECT_EQ("$|5,B=false|$", fid5.toString());

    Data::FidSegment fidSegment6;
    fidSegment6.set(6, fid1);
    Data::Fid fid6{fidSegment6};
    EXPECT_EQ("$|6,R=$|1,I=100|$|$", fid6.toString());
}

TEST(FidSegment, ConstructTest)
{
    Data::FidSegment fidSegment1(1, 100);
    EXPECT_EQ("100", fidSegment1.getValueToString());
    EXPECT_EQ(1, fidSegment1.getFid());
    EXPECT_EQ('I', fidSegment1.getType());

    Data::FidSegment fidSegment2(2, 9L);
    EXPECT_EQ("9", fidSegment2.getValueToString());
    EXPECT_EQ(2, fidSegment2.getFid());
    EXPECT_EQ('L', fidSegment2.getType());

    Data::FidSegment fidSegment3(3, 0.5795f);
    EXPECT_EQ("0.579500", fidSegment3.getValueToString());
    EXPECT_EQ(3, fidSegment3.getFid());
    EXPECT_EQ('F', fidSegment3.getType());

    Data::FidSegment fidSegment4(4, 0.579254545);
    EXPECT_EQ("0.579255", fidSegment4.getValueToString());
    EXPECT_EQ(4, fidSegment4.getFid());
    EXPECT_EQ('D', fidSegment4.getType());

    Data::FidSegment fidSegment5(5, false);
    EXPECT_EQ("false", fidSegment5.getValueToString());
    EXPECT_EQ(5, fidSegment5.getFid());
    EXPECT_EQ('B', fidSegment5.getType());
}

TEST(Fid, Usage)
{
    Data::Fid fidStream{};

    Data::FidSegment fidSegment{};

    fidSegment.set(1, "one");
    fidStream.append(fidSegment);

    fidSegment.set(2, "two");
    fidStream.append(fidSegment);

    fidSegment.set(3, "three");
    fidStream.append(fidSegment);

    auto ret = fidStream.toString();
    EXPECT_EQ("$|1,S=one|2,S=two|3,S=three|$", ret);

    Data::Fid parser{};

    parser.fromString(ret);

/*
    1,S=one
    2,S=two
    3,S=three
*/
    parser.walk(std::cout);

    cout<< "------------------------------------" << endl;

    fidSegment.set(100, 12001100);
    parser.append(fidSegment);

/*
    1,S=one
    2,S=two
    3,S=three
    100,I=12001100
*/
    parser.walk(std::cout);

    for(auto iter = parser.cbegin(); iter != parser.cend(); iter++)
    {
        cout << "Fid:[" << iter->getFid() <<"]," << "Type:[" << iter->getType() << "],"
        << "Value:[" << iter->getValueToString() << "]" << endl;
    }

    auto nothing = parser.find_if_Fid(1000);
    if (nothing == parser.end() )
    {
        cout << "no existing" << endl;
    }

    auto findTwo= parser.find_if_Fid(2);
    if (findTwo != parser.end() )
    {
        cout << "Fid:[" << findTwo->getFid() <<"]," << "Type:[" << findTwo->getType() << "],"
             << "Value:[" << findTwo->getValueToString() << "]" << endl;
    }

/*
    Fid:[3],Type:[S],Value:[three]
*/
    ++findTwo;
    EXPECT_EQ(3, findTwo->getFid());
    EXPECT_EQ('S',findTwo->getType());
    EXPECT_EQ("three", findTwo->getValueToString());

    Data::Fid depthThree{};
    Data::Fid depthTwo{};
    Data::Fid depthOne{};

    Data::FidSegment segmentComplex{};

    segmentComplex.set(1, 'A');
    depthOne.append(segmentComplex);

    segmentComplex.set(2, "This fid is Two");
    depthOne.append(segmentComplex);

    segmentComplex.set(1, depthOne);
    depthTwo.append(segmentComplex);

    segmentComplex.set(2, depthOne);
    depthTwo.append(segmentComplex);

    segmentComplex.set(3, depthOne);
    depthTwo.append(segmentComplex);

    depthTwo.walk(cout);

    segmentComplex.set(15, depthTwo);
    depthThree.append(segmentComplex);
    depthThree.walk(cout);


    parser.fromString(depthThree.toString());

    auto r = parser.find_if_Fid(15);
    cout << "fid 15, value :"<< r->getValueToString() << endl;

    parser.clear();
    parser.fromString(r->getValueToString());

    r = parser.find_if_Fid(1);
    cout << "fid 1, value :"<< r->getValueToString() << endl;

    r = parser.find_if_Fid(2);
    cout << "fid 2, value :"<< r->getValueToString() << endl;

    r = parser.find_if_Fid(3);
    cout << "fid 3, value :"<< r->getValueToString() << endl;
}
