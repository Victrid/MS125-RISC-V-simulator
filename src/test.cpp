#include "globaldef.hpp"
#include "parser.hpp"
#include "gtest/gtest.h"
#include <iostream>
using namespace std;

TEST(ParserTest, getline1) {
    Parser P("dataset/array_test1.data");
    auto t1 = P.getline();
    ASSERT_EQ(t1.address, 0);
    ASSERT_EQ(t1.instruction, 0x00020137);
    t1 = P.getline();
    ASSERT_EQ(t1.address, 4);
    ASSERT_EQ(t1.instruction, 0x040010ef);
    P.getline();
    P.getline();
    P.getline();
    P.getline();
    t1 = P.getline();
    ASSERT_EQ(t1.address, 0x00001000);
    ASSERT_EQ(t1.instruction, 0x00001737);
}

TEST(ParserTest, getline2) {
    Parser P("dataset/array_test2.data");
    P.getline();
    P.getline();
    P.getline();
    P.getline();
    P.getline();
    P.getline();
    P.getline();
    P.getline();
    P.getline();
    auto t1 = P.getline();
    ASSERT_EQ(t1.address, 0x0000100c);
    ASSERT_EQ(t1.instruction, 0x0ad50513);
    t1 = P.getline();
    ASSERT_EQ(t1.address, 0x00001010);
    ASSERT_EQ(t1.instruction, 0x1ca72023);
    P.getline();
    P.getline();
    P.getline();
    P.getline();
    t1 = P.getline();
    ASSERT_EQ(t1.address, 0x00001024);
    ASSERT_EQ(t1.instruction, 0x1c072683);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}