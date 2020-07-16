#include "core_tomasulo.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <sstream>
using namespace std;

TEST(TomasuloPartTest, branchcnt_test) {
    branchcnt c;
    EXPECT_EQ(c.get(), 0);
    c.ins(1);
    EXPECT_EQ(c.get(), 0b1000);
    c.ins(0);
    EXPECT_EQ(c.get(), 0b1000);
    c.ins(1);
    EXPECT_EQ(c.get(), 0b1010);
    c.ins(1);
    EXPECT_EQ(c.get(), 0b1011);
    c.ins(0);
    EXPECT_EQ(c.get(), 0b0011);
    branchcnt d;
    EXPECT_EQ(d.get(), 0);
    d = c;
    EXPECT_EQ(d.get(), 0b0011);
    d.ins(1);
    EXPECT_EQ(c.get(), 0b0011);
    EXPECT_EQ(d.get(), 0b0111);
}

TEST(TomasuloPartTest, renametab_test) {
    resstation res;
    //
}

TEST(TomasuloPartTest, core_test) {
    core_session C("dataset/pipeline-testset/test-datahazard.data");
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}