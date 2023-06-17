//
// Created by jaeseong on 23. 1. 28.
//

#include <gtest/gtest.h>
#include "DataStructure/Tree.h"
#include "DataStructure/Branch.h"
#include "Tnsd/Topic.h"
#include <iostream>

using namespace doori::api::DataStructure;
using namespace std;

TEST(Tree, Construct)
{
    Tree<int> intTree{};
    Tree<long> longTree{};
    Tree<char> charTree{};
    Tree<float> floatTree{};
    Tree<string> stringTree{};
}

TEST(Tree, Usage)
{
    //나무를 만듭니다. 이 나무는 int형의 데이터 잎을 가집니다.
    //또한 이 나무의 가지도(Branch<T>) int형 가지만 연결할 수 있습니다.
    Tree<int> tree{};

    //"one" 이름으로 정의한 나뭇가지를 만듭니다.
    Branch<int> oneBranch{"one"};

    // oneBranch 나뭇가지에, 잎을 답니다. 잎은 -10 입니다.
    oneBranch.AttachLeaf(-10);

    // ILeafKey를 상속받은 구현/실체화된 Topic 객체를 선언합니다.
    Topic topic{"one", "two"};

    // 나무에 ILeafKey에 맞는 나뭇가지를 구성합니다. 나무에 Tree<int> 에 oneBranch 가지를 붙입니다..
    // 중복이면 fail 입니다.
    cout << "true면, 중복이 아닙니다.:" << boolalpha << tree.attachBranch(topic, oneBranch, INSERT::CHECK) << endl;

    // 동일하게 한번 더 실행하면 이제는 중복입니다.
    cout << "false이면 중복입니다.:" << boolalpha << tree.attachBranch(topic, oneBranch, INSERT::CHECK) << endl;

    // ILeafKey 가지를 찾고, 없다면 가지를 생성 후, 잎을 달고, 기존에 그 가지가 있다면 잎만 답니다.
    // 여기에서는 "two"가지를 생성 후 잎을 답니다.
    tree.attachLeaf(topic, 1);

    auto findIt = tree.getBranch(topic);

    // 현재까지, 잎은 하나밖에 없습니다.
    for(auto& leaf:findIt.GetLeaves())
    {
        EXPECT_EQ(leaf, 1);
    }

    // oneBranch에 이렇게 해서 찾을 수도 있습니다.
    Topic topicDepth{"one","two","two"};
    findIt = tree.getBranch(topicDepth);
    for(auto& leaf:findIt.GetLeaves())
    {
        EXPECT_EQ(leaf, -10);
    }
}