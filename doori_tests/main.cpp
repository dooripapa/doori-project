//
// Created by jaeseong on 20. 5. 7..
//
#include <gtest/gtest.h>

int sum(int a, int b){
    return (a+b);
}

TEST(MyTestCase, PositiveTest)
{
    EXPECT_EQ(3, sum(1,2));
}


int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

