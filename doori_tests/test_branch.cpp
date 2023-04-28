//
// Created by jaeseong on 23. 1. 28.
//

#include <gtest/gtest.h>
#include "DataStructure/Branch.h"
#include <iostream>

using namespace doori;
using namespace std;

TEST(Branch , Construct)
{
    Branch<int> i{}, i2{};
    Branch<long> l{};
    Branch<float> f{};
    Branch<double> d{};
    Branch<string> s{};
}

TEST(Branch, Sample)
{
    // 이 가지는 int 형의 잎만 붙일 수 있다.
    Branch<int> r{"main branch"};
    //나무가지에 topic leejaeseong 이라고 정의하고, 잎을 붙인다.
    r.AttachLeaf(1);
    r.AttachLeaf(2);
    r.AttachLeaf(3);
    r.AttachLeaf(4);
    r.AttachLeaf(5);

    //해당 나뭇가자에, 달려 있는 잎을 확인해 본다.
    for(auto &v : r.GetLeaves() )
    {
        cout<< v << endl;
    }

    if(r.DropLeaf(10))
        cout << "10의 잎이 제거됨." << endl;
    else
        cout << "10의 잎이 존재하지 않음." << endl;

    //해당 나뭇가자에, 달려 있는 잎을 확인해 본다.
    for(auto &v : r.GetLeaves() )
    {
        cout<< v << endl;
    }

    Branch<int> r2;
    r2.SetName("second branch");
    r2.AttachLeaf(11);
    r2.AttachLeaf(12);
    r2.AttachLeaf(13);
    r2.AttachLeaf(14);
    r2.AttachLeaf(15);

    r.Link(r2);

    Branch<int> r3;
    r3.SetName("third branch");
    r3.AttachLeaf(21);
    r3.AttachLeaf(22);
    r3.AttachLeaf(23);
    r3.AttachLeaf(24);
    r3.AttachLeaf(25);

    cout << boolalpha << r.Link(r3) << endl ;

    vector<Branch<int>>::iterator secondIterator{};
    auto result = r.FindLinkBranches("main branch", secondIterator);

    //찾았다
    if (result)
    {
        for(auto &v: secondIterator->GetLeaves() ) {
            cout<< "second : "<< v << endl;
        }
    }

    cout << "------------------------------------------" << endl;
    if (r.FindLinkBranches("third branch", secondIterator))
    {
        for(auto &v: secondIterator->GetLeaves() ) {
            cout<< "second : "<< v << endl;
        }
    }
}