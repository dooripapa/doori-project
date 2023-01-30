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
    r.attachLeaf(1);
    r.attachLeaf(2);
    r.attachLeaf(3);
    r.attachLeaf(4);
    r.attachLeaf(5);

    //해당 나뭇가자에, 달려 있는 잎을 확인해 본다.
    for(auto &v : r.getLeaves() )
    {
        cout<< v << endl;
    }

    if(r.dropLeaf(10))
        cout << "10의 잎이 제거됨." << endl;
    else
        cout << "10의 잎이 존재하지 않음." << endl;

    //해당 나뭇가자에, 달려 있는 잎을 확인해 본다.
    for(auto &v : r.getLeaves() )
    {
        cout<< v << endl;
    }

    Branch<int> r2;
    r2.setName("second branch");
    r2.attachLeaf(11);
    r2.attachLeaf(12);
    r2.attachLeaf(13);
    r2.attachLeaf(14);
    r2.attachLeaf(15);

    r.link(r2);

    Branch<int> r3;
    r3.setName("third branch");
    r3.attachLeaf(21);
    r3.attachLeaf(22);
    r3.attachLeaf(23);
    r3.attachLeaf(24);
    r3.attachLeaf(25);

    cout<< boolalpha << r.link(r3) << endl ;

    vector<Branch<int>>::iterator secondIterator{};
    auto result = r.findLinkBranches("main branch", secondIterator);

    //찾았다
    if (result)
    {
        for(auto &v: secondIterator->getLeaves() ) {
            cout<< "second : "<< v << endl;
        }
    }

    cout << "------------------------------------------" << endl;
    if (r.findLinkBranches("third branch", secondIterator))
    {
        for(auto &v: secondIterator->getLeaves() ) {
            cout<< "second : "<< v << endl;
        }
    }
}