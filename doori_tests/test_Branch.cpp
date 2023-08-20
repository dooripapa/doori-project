//
// Created by jaeseong on 23. 1. 28.
//

#include <gtest/gtest.h>
#include "DataStructure/Branch.h"
#include <iostream>

using namespace doori::api::DataStructure;
using namespace std;

TEST(Branch , Construct)
{
    Branch<int> i{}, i2{};
    Branch<long> l{};
    Branch<float> f{};
    Branch<double> d{};
    Branch<string> s{};
}

TEST(Branch, Usage)
{
    // 이 가지(Branch)는 int 타입만의 잎만 붙일 수 있다.
    // 이 가지의 이름은 "main branch"
    Branch<int> r{"main branch"};

    // "main branch"의 잎에 1,2,3,4,5 붙였습니다.
    r.AttachLeaf(1);
    r.AttachLeaf(2);
    r.AttachLeaf(3);
    r.AttachLeaf(4);
    r.AttachLeaf(5);

    //해당 나뭇가자에, 달려 있는 잎을 확인해 본다.
    for(auto &leaf : r.GetLeaves() ) {
        cout<< leaf << endl;
    }

    if(r.DropLeaf(10))
        cout << "10의 잎이 제거됨." << endl;
    else
        cout << "10의 잎이 존재하지 않음." << endl;

    if(r.DropLeaf(1))
        cout << "1의 잎이 제거됨." << endl;

    //해당 나뭇가자에, 달려 있는 잎을 확인해 본다.
    for(auto &v : r.GetLeaves() ) {
        cout<< v << endl;
    }

    Branch<int> r2;
    r2.SetName("second branch");
    r2.AttachLeaf(11);
    r2.AttachLeaf(12);
    r2.AttachLeaf(13);
    r2.AttachLeaf(14);
    r2.AttachLeaf(15);

    cout << boolalpha << r.Link(r2) << endl ;

    Branch<int> r3;
    r3.SetName("third branch");
    r3.AttachLeaf(21);
    r3.AttachLeaf(22);
    r3.AttachLeaf(23);
    r3.AttachLeaf(24);
    r3.AttachLeaf(25);

    cout << boolalpha << r.Link(r3) << endl ;

    vector<Branch<int>>::iterator iter{};
    // 조회되지 않음. FindLinkBranches의 함수는, 자식의 Branch를 탐색함.
    auto result = r.FindLinkBranches("main branch", iter);
    if (result)
    {
        for(auto &leaf: iter->GetLeaves() ) {
            cout << "main branch: " << leaf << endl;
        }
    }

    if(r.FindLinkBranches("second branch", iter))
    {
        for(auto &leaf: iter->GetLeaves() ) {
            cout << "second branch: " << leaf << endl;
        }
    }

    cout << "------------------------------------------" << endl;
    if (r.FindLinkBranches("third branch", iter))
    {
        for(auto &leaf: iter->GetLeaves() ) {
            cout << "third branch : " << leaf << endl;
        }
    }
}