#include "globaldef.hpp"
#include "parser.hpp"
#include "gtest/gtest.h"
#include <iostream>
using namespace std;

TEST(ParserTest, padimm) {
    Parser P("dataset/array_test1.data");
    taddr t1  = 0b00000000000000000000001000101101,
          t1a = 0b11111111111111111111111000101101,
          t2  = 0b00000000000000000011010001001101,
          t2a = 0b00000000000000000011010001001101,
          t3  = 0b11111111111111111111111111111111,
          t3a = 0b11111111111111111111111111111111,
          t4  = 0b01111111111111111111111111111111,
          t4a = 0b11111111111111111111111111111111;
    P.padimm(t1, 10);
    EXPECT_EQ(t1, t1a);
    P.padimm(t2, 15);
    EXPECT_EQ(t2, t2a);
    P.padimm(t3, 32);
    EXPECT_EQ(t3, t3a);
    P.padimm(t4, 31);
    EXPECT_EQ(t4, t4a);
}

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

TEST(ParserTest, RConstructor_XOR) {
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

TEST(ParserTest, IConstructor_ADDI_1_Positive) {
    Parser P("dataset/array_test2.data");
    //addi	a0,a0,173 ADDI rd,rs1,imm
    auto z = P.IConstructor(0x0ad50513, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::I);
    EXPECT_EQ(z.rd, 0b01010);
    EXPECT_EQ(z.funct3, 0b000);
    EXPECT_EQ(z.rs1, 0b01010);
    EXPECT_EQ(z.imm, 0b00000000000000000000000010101101);
}
TEST(ParserTest, IConstructor_ADDI_2_Negative) {
    Parser P("dataset/array_test2.data");
    //addi	sp,sp,-32
    auto z = P.IConstructor(0xfe010113, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::I);
    EXPECT_EQ(z.rd, 0b00010);
    EXPECT_EQ(z.funct3, 0b000);
    EXPECT_EQ(z.rs1, 0b00010);
    EXPECT_EQ(z.imm, 0b11111111111111111111111111100000);
}

TEST(ParserTest, IsConstructor_SRLI_1) {
    Parser P("dataset/array_test2.data");
    //srli	a3,a3,0x1
    auto z = P.IsConstructor(0x0016d693, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::Is);
    EXPECT_EQ(z.rd, 0b01101);
    EXPECT_EQ(z.funct3, 0b101);
    EXPECT_EQ(z.rs1, 0b01101);
    EXPECT_EQ(z.imm, 0b1);
    EXPECT_EQ(z.funct7, 0);
}
TEST(ParserTest, IsConstructor_SRLI_2) {
    Parser P("dataset/array_test2.data");
    //srli	a2,a2,0x1
    auto z = P.IsConstructor(0x00165613, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::Is);
    EXPECT_EQ(z.rd, 0b01100);
    EXPECT_EQ(z.funct3, 0b101);
    EXPECT_EQ(z.rs1, 0b01100);
    EXPECT_EQ(z.imm, 0b1);
    EXPECT_EQ(z.funct7, 0);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}