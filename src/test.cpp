#include "globaldef.hpp"
#include "parser.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <sstream>
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

TEST(ParserTest, RSplitter_XOR) {
    Parser P("dataset/array_test2.data");
    //xor	a0,a0,a5
    auto z = P.RSplitter(0x00f54533, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::R);
    EXPECT_EQ(z.rd, 0b01010) << z.rd;
    EXPECT_EQ(z.funct3, 0b100);
    EXPECT_EQ(z.rs1, 0b01010);
    EXPECT_EQ(z.rs2, 0b01111);
    EXPECT_EQ(z.funct7, 0);
}

TEST(ParserTest, Splitter_1) {
    Parser P("dataset/array_test2.data");
    //xor	a0,a0,a5
    auto z = P.Splitter(0x00f54533, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::R);
    EXPECT_EQ(z.rd, 0b01010) << z.rd;
    EXPECT_EQ(z.funct3, 0b100);
    EXPECT_EQ(z.rs1, 0b01010);
    EXPECT_EQ(z.rs2, 0b01111);
    EXPECT_EQ(z.funct7, 0);
}

TEST(ParserTest, ISplitter_ADDI_1_Positive) {
    Parser P("dataset/array_test2.data");
    //addi	a0,a0,173 ADDI rd,rs1,imm
    auto z = P.ISplitter(0x0ad50513, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::I);
    EXPECT_EQ(z.rd, 0b01010);
    EXPECT_EQ(z.funct3, 0b000);
    EXPECT_EQ(z.rs1, 0b01010);
    EXPECT_EQ(z.imm, 0b00000000000000000000000010101101);
}

TEST(ParserTest, ISplitter_ADDI_2_Negative) {
    Parser P("dataset/array_test2.data");
    //addi	sp,sp,-32
    auto z = P.ISplitter(0xfe010113, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::I);
    EXPECT_EQ(z.rd, 0b00010);
    EXPECT_EQ(z.funct3, 0b000);
    EXPECT_EQ(z.rs1, 0b00010);
    EXPECT_EQ(z.imm, 0b11111111111111111111111111100000);
}

TEST(ParserTest, Splitter_2) {
    Parser P("dataset/array_test2.data");
    //addi	a0,a0,173 ADDI rd,rs1,imm
    auto z = P.Splitter(0x0ad50513, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::I);
    EXPECT_EQ(z.rd, 0b01010);
    EXPECT_EQ(z.funct3, 0b000);
    EXPECT_EQ(z.rs1, 0b01010);
    EXPECT_EQ(z.imm, 0b00000000000000000000000010101101);
}

TEST(ParserTest, Splitter_3) {
    Parser P("dataset/array_test2.data");
    //addi	sp,sp,-32
    auto z = P.Splitter(0xfe010113, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::I);
    EXPECT_EQ(z.rd, 0b00010);
    EXPECT_EQ(z.funct3, 0b000);
    EXPECT_EQ(z.rs1, 0b00010);
    EXPECT_EQ(z.imm, 0b11111111111111111111111111100000);
}

TEST(ParserTest, IsSplitter_SRLI_1) {
    Parser P("dataset/array_test2.data");
    //srli	a3,a3,0x1
    auto z = P.IsSplitter(0x0016d693, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::I);
    EXPECT_EQ(z.rd, 0b01101);
    EXPECT_EQ(z.funct3, 0b101);
    EXPECT_EQ(z.rs1, 0b01101);
    EXPECT_EQ(z.imm, 0b1);
    EXPECT_EQ(z.funct7, 0);
}

TEST(ParserTest, IsSplitter_SRLI_2) {
    Parser P("dataset/array_test2.data");
    //srli	a2,a2,0x1
    auto z = P.IsSplitter(0x00165613, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::I);
    EXPECT_EQ(z.rd, 0b01100);
    EXPECT_EQ(z.funct3, 0b101);
    EXPECT_EQ(z.rs1, 0b01100);
    EXPECT_EQ(z.imm, 0b1);
    EXPECT_EQ(z.funct7, 0);
}

TEST(ParserTest, Splitter_4) {
    Parser P("dataset/array_test2.data");
    //srli	a3,a3,0x1
    auto z = P.Splitter(0x0016d693, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::I);
    EXPECT_EQ(z.rd, 0b01101);
    EXPECT_EQ(z.funct3, 0b101);
    EXPECT_EQ(z.rs1, 0b01101);
    EXPECT_EQ(z.imm, 0b1);
    EXPECT_EQ(z.funct7, 0);
}

TEST(ParserTest, Splitter_5) {
    Parser P("dataset/array_test2.data");
    //srli	a2,a2,0x1
    auto z = P.Splitter(0x00165613, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::I);
    EXPECT_EQ(z.rd, 0b01100);
    EXPECT_EQ(z.funct3, 0b101);
    EXPECT_EQ(z.rs1, 0b01100);
    EXPECT_EQ(z.imm, 0b1);
    EXPECT_EQ(z.funct7, 0);
}

TEST(ParserTest, SSplitter_1) {
    Parser P("dataset/array_test2.data");
    //sb	a2,4(a3)
    auto z = P.SSplitter(0x00c68223, 0x1357ABCD);
    //0b 00000 01100 01101 000 00100 0100011;
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::S);
    EXPECT_EQ(z.funct3, 0b000);
    EXPECT_EQ(z.rs1, 0b01101);
    EXPECT_EQ(z.rs2, 0b01100);
    EXPECT_EQ(z.imm, 0b00000000000000000000000000000100);
}

TEST(ParserTest, SSplitter_2) {
    Parser P("dataset/array_test2.data");
    //sw	zero,416(s0)
    auto z = P.SSplitter(0x1a042023, 0x1357ABCD);
    //0b 0001101 00000 01000 010 00000 0100011
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::S);
    EXPECT_EQ(z.funct3, 0b010);
    EXPECT_EQ(z.rs1, 0b01000);
    EXPECT_EQ(z.rs2, 0b00000);
    EXPECT_EQ(z.imm, 0b00000000000000000000000110100000);
}

TEST(ParserTest, USplitter_LUI) {
    Parser P("dataset/array_test2.data");
    //lui	sp,0x20
    auto z = P.USplitter(0x00020137, 0x1357ABCD);
    //0b 100000 00010 0110111
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::U);
    EXPECT_EQ(z.rd, 0b00010);
    EXPECT_EQ(z.imm, 0b00000000000000100000000000000000);
}

TEST(ParserTest, USplitter_AUIPC) {
    Parser P("dataset/array_test2.data");
    //auipc sp,0x20
    auto z = P.USplitter(0x00020117, 0x1357ABCD);
    //0b 100000 00010 0010111
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::Ua);
    EXPECT_EQ(z.rd, 0b00010);
    EXPECT_EQ(z.imm, 0b00000000000000100000000000000000);
}

TEST(ParserTest, Splitter_11) {
    Parser P("dataset/array_test2.data");
    //auipc sp,0x20
    auto z = P.USplitter(0x00020117, 0x1357ABCD);
    //0b100000000100010111
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::Ua);
    EXPECT_EQ(z.rd, 0b00010);
    EXPECT_EQ(z.imm, 0b00000000000000100000000000000000);
}

TEST(ParserTest, Splitter_6) {
    Parser P("dataset/array_test2.data");
    //sb	a2,4(a3)
    auto z = P.Splitter(0x00c68223, 0x1357ABCD);
    //0b 00000 01100 01101 000 00100 0100011;
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::S);
    EXPECT_EQ(z.funct3, 0b000);
    EXPECT_EQ(z.rs1, 0b01101);
    EXPECT_EQ(z.rs2, 0b01100);
    EXPECT_EQ(z.imm, 0b00000000000000000000000000000100);
}

TEST(ParserTest, Splitter_7) {
    Parser P("dataset/array_test2.data");
    //sw	zero,416(s0)
    auto z = P.SSplitter(0x1a042023, 0x1357ABCD);
    //0b 0001101 00000 01000 010 00000 0100011
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::S);
    EXPECT_EQ(z.funct3, 0b010);
    EXPECT_EQ(z.rs1, 0b01000);
    EXPECT_EQ(z.rs2, 0b00000);
    EXPECT_EQ(z.imm, 0b00000000000000000000000110100000);
}

TEST(ParserTest, Splitter_8) {
    Parser P("dataset/array_test2.data");
    //sw	zero,416(s0)
    auto z = P.Splitter(0x00020137, 0x1357ABCD);
    //0b 100000 00010 0110111
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::U);
    EXPECT_EQ(z.rd, 0b00010);
    EXPECT_EQ(z.imm, 0b00000000000000100000000000000000);
}

TEST(ParserTest, BSplitter) {
    Parser P("dataset/array_test2.data");
    //beq	a4,a5,12e0
    auto z = P.BSplitter(0xfef708e3, 0x1357ABCD);
    //0b1111111 01111 01110 000 10001 1100011
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::B);
    EXPECT_EQ(z.funct3, 0b000);
    EXPECT_EQ(z.rs1, 0b01110);
    EXPECT_EQ(z.rs2, 0b01111);
    EXPECT_EQ(z.imm, 0b11111111111111111111111111110000);
}

TEST(ParserTest, Splitter_9) {
    Parser P("dataset/array_test2.data");
    //beq	a4,a5,12e0
    auto z = P.Splitter(0xfef708e3, 0x1357ABCD);
    //0b1111111 01111 01110 000 10001 1100011
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::B);
    EXPECT_EQ(z.funct3, 0b000);
    EXPECT_EQ(z.rs1, 0b01110);
    EXPECT_EQ(z.rs2, 0b01111);
    EXPECT_EQ(z.imm, 0b11111111111111111111111111110000);
}

TEST(ParserTest, JSplitter) {
    Parser P("dataset/array_test2.data");
    //jal	ra,1000
    auto z = P.JSplitter(0xf99ff0ef, 0x1357ABCD);
    //0b 1 1111001100 1 11111111 00001 1101111
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::J);
    EXPECT_EQ(z.rd, 0b00001);
    EXPECT_EQ(z.imm, 0b11111111111111111111111110011000);
}

TEST(ParserTest, Splitter_10) {
    Parser P("dataset/array_test2.data");
    //jal	ra,1000
    auto z = P.Splitter(0xf99ff0ef, 0x1357ABCD);
    //0b11111001100111111111 00001 1101111
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::J);
    EXPECT_EQ(z.rd, 0b00001);
    EXPECT_EQ(z.imm, 0b11111111111111111111111110011000);
}

TEST(ParserTest, Totaltest) {
}

int main(int argc, char** argv) {
    // Parser P("dataset/array_test1.data");
    // mempair m = P.getline();
    // while (m.instruction != 0) {
    //     auto z = P.Splitter(m.instruction, m.address);
    //    m = P.getline();
    // }
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}