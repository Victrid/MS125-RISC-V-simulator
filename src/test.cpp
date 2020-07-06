#include "globaldef.hpp"
#include "parser.hpp"
#include "gtest/gtest.h"
#include <iostream>
using namespace std;

TEST(ParserTest, getline_1) {
    Parser P("dataset/array_test1.data");
    auto t1 = P.getline();
    EXPECT_EQ(t1.address, 0);
    EXPECT_EQ(t1.instruction, 0x00020137);
    t1 = P.getline();
    EXPECT_EQ(t1.address, 4);
    EXPECT_EQ(t1.instruction, 0x040010ef);
    P.getline();
    P.getline();
    P.getline();
    P.getline();
    t1 = P.getline();
    EXPECT_EQ(t1.address, 0x00001000);
    EXPECT_EQ(t1.instruction, 0x00001737);
}

TEST(ParserTest, getline_2) {
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
    EXPECT_EQ(t1.address, 0x0000100c);
    EXPECT_EQ(t1.instruction, 0x0ad50513);
    t1 = P.getline();
    EXPECT_EQ(t1.address, 0x00001010);
    EXPECT_EQ(t1.instruction, 0x1ca72023);
    P.getline();
    P.getline();
    P.getline();
    P.getline();
    t1 = P.getline();
    EXPECT_EQ(t1.address, 0x00001024);
    EXPECT_EQ(t1.instruction, 0x1c072683);
}

TEST(ParserTest, RConstructor) {
    Parser P("dataset/array_test2.data");
    //xor	a0,a0,a5
    auto z = P.RConstructor(0x00f54533, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::R);
    EXPECT_EQ(z.rd, 0b01010) << z.rd;
    EXPECT_EQ(z.funct3, 0b100);
    EXPECT_EQ(z.rs1, 0b01010);
    EXPECT_EQ(z.rs2, 0b01111);
    EXPECT_EQ(z.funct7, 0);
}

TEST(ParserTest, IConstructor) {
    Parser P("dataset/array_test2.data");
    //addi	a0,a0,173 ADDI rd,rs1,imm
    auto z = P.IConstructor(0x0ad50513, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::I);
    EXPECT_EQ(z.rd, 0b01010);
    EXPECT_EQ(z.funct3, 0b000);
    EXPECT_EQ(z.rs1, 0b01010);
    EXPECT_EQ(z.imm, 0b000010101101);
}
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}