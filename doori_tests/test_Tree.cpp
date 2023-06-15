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
    //나무를 만듭니다.
    Tree<int> tree{};

    //"two"이름으로 정의한 나뭇가지를 만듭니다.
    Branch<int> oneBranch{"two"};

    // oneBranch 나뭇가지에, 잎을 답니다. 잎은 -10 입니다.
    oneBranch.AttachLeaf(-10);

    // ILeafKey를 상속받은 Tnsd::Topic를 선언합니다.
    Topic topic{"one", "two"};

    // 나무에 ILeafKey에 맞는 나뭇가지를 구성합니다. 구성된 나뭇가지에 oneBranch 나뭇가지를 엮습니다.
    // 중복이면 fail 입니다.
    cout << "true면, 중복이 아닙니다.:" << boolalpha << tree.attachBranch(topic, oneBranch, INSERT::CHECK) << endl;

    // 동일하게 한번 더 실행하면 이제는 중복입니다.
    cout << "false이면 중복입니다.:" << boolalpha << tree.attachBranch(topic, oneBranch, INSERT::CHECK) << endl;

    // ILeafKey값으로 나뭇가지를 찾고, 그 나뭇가지에 잎을 답니다. 잎은 1입니다.
    tree.attachLeaf(topic, 1);

    auto findIt = tree.getBranch(topic);
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