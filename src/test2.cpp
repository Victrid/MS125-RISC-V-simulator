#include "core.hpp"
#include "globaldef.hpp"
#include "memory.hpp"
#include "parser.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <sstream>
using namespace std;
Parser P;

TEST(CoreTest, TickTest_Immidiates) {
    //This is aimed to test
    //commands interacting with immidiates.
    core_session C("dataset/basic-testset/test-1.data");
    C.cycle();
    EXPECT_EQ(C.reg[2], 0x00020000);
    C.cycle();
    EXPECT_EQ(C.reg[2], 0x00020020);
    C.cycle();
    EXPECT_EQ(C.reg[2], 0x0002001D);
    C.cycle();
    EXPECT_EQ(C.reg[3], 0x00000210);
    C.cycle();
    EXPECT_EQ(C.reg[2], 0x00000000);
    EXPECT_EQ(C.reg[3], 0x00000210);

    C.cycle();
    EXPECT_EQ(C.reg[2], 0x00000000);
    EXPECT_EQ(C.reg[3], 0x00000210);
    C.cycle();
    EXPECT_EQ(C.reg[2], 0x00000001);
    EXPECT_EQ(C.reg[3], 0x00000210);
    C.cycle();
    EXPECT_EQ(C.reg[5], 0xFFFFF000);
    C.cycle();
    EXPECT_EQ(C.reg[2], 0x00000001);
    C.cycle();
    EXPECT_EQ(C.reg[2], 0x00000000);

    C.cycle();
    EXPECT_EQ(C.reg[2], 0x00000001);
    C.cycle();
    EXPECT_EQ(C.reg[2], 0x00000000);
    EXPECT_EQ(C.reg[5], 0xFFFFF000);
    C.cycle();
    EXPECT_EQ(C.reg[2], 0xFFFFF00F);
    EXPECT_EQ(C.reg[5], 0xFFFFF000);
    C.cycle();
    EXPECT_EQ(C.reg[2], 0xFFFFF0FF);
    EXPECT_EQ(C.reg[5], 0xFFFFF000);
    C.cycle();
    EXPECT_EQ(C.reg[2], 0xFFFFE000);

    C.cycle();
    EXPECT_EQ(C.reg[2], 0x7FFFF800);
    C.cycle();
    EXPECT_EQ(C.reg[2], 0xFFFFF800);
    C.cycle();
    EXPECT_EQ(C.reg[2], 0xFFFFF044);
}

TEST(CoreTest, TickTest_Registers) {
    //This is aimed to test
    //commands interacting with immidiates.
    core_session C("dataset/basic-testset/test-2.data");
    C.cycle();
    EXPECT_EQ(C.reg[1], 0xFFFFFFFF);
    C.cycle();
    EXPECT_EQ(C.reg[2], 0x0000000F);
    C.cycle();
    EXPECT_EQ(C.reg[3], 0x0000000E);
    EXPECT_EQ(C.reg[1], 0xFFFFFFFF);
    EXPECT_EQ(C.reg[2], 0x0000000F);
    C.cycle();
    EXPECT_EQ(C.reg[0], 0x00000000);
    EXPECT_EQ(C.reg[1], 0xFFFFFFFF);
    EXPECT_EQ(C.reg[2], 0x0000000F);
    C.cycle();
    EXPECT_EQ(C.reg[3], 0xFFFFFFF0);
    EXPECT_EQ(C.reg[1], 0xFFFFFFFF);
    EXPECT_EQ(C.reg[2], 0x0000000F);

    C.cycle();
    EXPECT_EQ(C.reg[3], 0x00000010);
    EXPECT_EQ(C.reg[1], 0xFFFFFFFF);
    EXPECT_EQ(C.reg[2], 0x0000000F);
    C.cycle();
    EXPECT_EQ(C.reg[3], 0x00000001);
    C.cycle();
    EXPECT_EQ(C.reg[3], 0x00000000);
    C.cycle();
    EXPECT_EQ(C.reg[3], 0x0000000F);
    C.cycle();
    EXPECT_EQ(C.reg[4], 0xFFFFFFF0);

    C.cycle();
    EXPECT_EQ(C.reg[5], 0x0000000E);
    C.cycle();
    EXPECT_EQ(C.reg[3], 0xFFFFFFFE);
    C.cycle();
    EXPECT_EQ(C.reg[3], 0x00000001);
    C.cycle();
    EXPECT_EQ(C.reg[3], 0x80000000);
    C.cycle();
    EXPECT_EQ(C.reg[3], 0x00010000);

    C.cycle();
    EXPECT_EQ(C.reg[3], 0x0001FFFF);
    C.cycle();
    EXPECT_EQ(C.reg[3], 0xFFFFFFF0);
    C.cycle();
    EXPECT_EQ(C.reg[2], 0x00000002);
    C.cycle();
    EXPECT_EQ(C.reg[3], 0xFFFFFFFC);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}