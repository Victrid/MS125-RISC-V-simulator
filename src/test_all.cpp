#include "core.hpp"
#include "globaldef.hpp"
#include "memory.hpp"
#include "parser.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <sstream>
using namespace std;
Parser P;

TEST(LoaderTest, getline_1) {
    loader l("dataset/array_test1.data");
    auto t1 = l.getline();
    EXPECT_EQ(t1.address, 0);
    EXPECT_EQ(t1.instruction, 0x37010200);
    t1 = l.getline();
    EXPECT_EQ(t1.address, 4);
    EXPECT_EQ(t1.instruction, 0xef100004);
    l.getline();
    l.getline();
    l.getline();
    l.getline();
    t1 = l.getline();
    EXPECT_EQ(t1.address, 0x00001000);
    EXPECT_EQ(t1.instruction, 0x37170000);
}

TEST(LoaderTest, getline_2) {
    loader P("dataset/array_test2.data");
    auto t1 = P.getline();
    for (int i = 0; i < 9; i++)
        t1 = P.getline();
    EXPECT_EQ(t1.address, 0x0000100c);
    EXPECT_EQ(t1.instruction, 0x1305d50a);
    t1 = P.getline();
    EXPECT_EQ(t1.address, 0x00001010);
    EXPECT_EQ(t1.instruction, 0x2320a71c);
    for (int i = 0; i < 5; i++)
        t1 = P.getline();
    EXPECT_EQ(t1.address, 0x00001024);
    EXPECT_EQ(t1.instruction, 0x8326071c);
}

TEST(ParserTest, padimm) {
    taddr test[] = {0x08008241, 0x0000344D, 0xFFFFFFFF, 0x7FFFFFFF},
          ans[]  = {0xFFFFFE41, 0x344D, 0xFFFFFFFF, 0xFFFFFFFF},
          pad[]  = {10, 15, 32, 31};
    for (int i = 0; i < 4; i++) {
        P.padimm(test[i], pad[i]);
        EXPECT_EQ(test[i], ans[i]);
    }
}

TEST(ParserTest, rearrange) {
    EXPECT_EQ(P.rearrange(0x2382c600), 0x00c68223);
    EXPECT_EQ(P.rearrange(0x2320041a), 0x1A042023);
}

TEST(ParserTest, RSplitter_XOR) {
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

TEST(ParserTest, ISplitter_ADDI_1_Positive) {
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
    //addi	sp,sp,-32
    auto z = P.ISplitter(0xfe010113, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::I);
    EXPECT_EQ(z.rd, 0b00010);
    EXPECT_EQ(z.funct3, 0b000);
    EXPECT_EQ(z.rs1, 0b00010);
    EXPECT_EQ(z.imm, 0b11111111111111111111111111100000);
}

TEST(ParserTest, IsSplitter_SRLI_1) {
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

TEST(ParserTest, IlSplitter) {
    //srli	a2,a2,0x1
    //0b0 00010 000 01101 0000011
    auto z = P.IlSplitter(0x00010683, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::Il);
    EXPECT_EQ(z.rd, 0b01101);
    EXPECT_EQ(z.funct3, 0b000);
    EXPECT_EQ(z.rs1, 0b00010);
    EXPECT_EQ(z.imm, 0b0);
}

TEST(ParserTest, SSplitter_1) {
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
    //lui	sp,0x20
    auto z = P.USplitter(0x00020137, 0x1357ABCD);
    //0b 100000 00010 0110111
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::U);
    EXPECT_EQ(z.rd, 0b00010);
    EXPECT_EQ(z.imm, 0b00000000000000100000000000000000);
}

TEST(ParserTest, USplitter_AUIPC) {
    //auipc sp,0x20
    auto z = P.USplitter(0x00020117, 0x1357ABCD);
    //0b 100000 00010 0010111
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::Ua);
    EXPECT_EQ(z.rd, 0b00010);
    EXPECT_EQ(z.imm, 0b00000000000000100000000000000000);
}

TEST(ParserTest, BSplitter) {
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

TEST(ParserTest, JSplitter) {
    //jal	ra,1000
    auto z = P.JSplitter(0xf99ff0ef, 0x1357ABCD);
    //0b 1 1111001100 1 11111111 00001 1101111
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::J);
    EXPECT_EQ(z.rd, 0b00001);
    EXPECT_EQ(z.imm, 0b11111111111111111111111110011000);
}

TEST(ParserTest, Splitter_1) {
    //xor	a0,a0,a5
    auto z = P.Splitter(0x3345f500, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::R);
    EXPECT_EQ(z.rd, 0b01010) << z.rd;
    EXPECT_EQ(z.funct3, 0b100);
    EXPECT_EQ(z.rs1, 0b01010);
    EXPECT_EQ(z.rs2, 0b01111);
    EXPECT_EQ(z.funct7, 0);
}

TEST(ParserTest, Splitter_2) {
    //addi	a0,a0,173 ADDI rd,rs1,imm
    auto z = P.Splitter(0x1305d50a, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::I);
    EXPECT_EQ(z.rd, 0b01010);
    EXPECT_EQ(z.funct3, 0b000);
    EXPECT_EQ(z.rs1, 0b01010);
    EXPECT_EQ(z.imm, 0b00000000000000000000000010101101);
}

TEST(ParserTest, Splitter_3) {
    //addi	sp,sp,-32
    auto z = P.Splitter(0x130101fe, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::I);
    EXPECT_EQ(z.rd, 0b00010);
    EXPECT_EQ(z.funct3, 0b000);
    EXPECT_EQ(z.rs1, 0b00010);
    EXPECT_EQ(z.imm, 0b11111111111111111111111111100000);
}

TEST(ParserTest, Splitter_4) {
    //srli	a3,a3,0x1
    auto z = P.Splitter(0x93d61600, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::I);
    EXPECT_EQ(z.rd, 0b01101);
    EXPECT_EQ(z.funct3, 0b101);
    EXPECT_EQ(z.rs1, 0b01101);
    EXPECT_EQ(z.imm, 0b1);
    EXPECT_EQ(z.funct7, 0);
}

TEST(ParserTest, Splitter_5) {
    //srli	a2,a2,0x1
    auto z = P.Splitter(0x13561600, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::I);
    EXPECT_EQ(z.rd, 0b01100);
    EXPECT_EQ(z.funct3, 0b101);
    EXPECT_EQ(z.rs1, 0b01100);
    EXPECT_EQ(z.imm, 0b1);
    EXPECT_EQ(z.funct7, 0);
}

TEST(ParserTest, Splitter_6) {
    //sb	a2,4(a3)
    auto z = P.Splitter(0x2382c600, 0x1357ABCD);
    //0b 00000 01100 01101 000 00100 0100011;
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::S);
    EXPECT_EQ(z.funct3, 0b000);
    EXPECT_EQ(z.rs1, 0b01101);
    EXPECT_EQ(z.rs2, 0b01100);
    EXPECT_EQ(z.imm, 0b00000000000000000000000000000100);
}

TEST(ParserTest, Splitter_7) {
    //sw	zero,416(s0)
    auto z = P.Splitter(0x2320041a, 0x1357ABCD);
    //0b00011010000001000010000000100011
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::S);
    EXPECT_EQ(z.funct3, 0b010);
    EXPECT_EQ(z.rs1, 0b01000);
    EXPECT_EQ(z.rs2, 0b00000);
    EXPECT_EQ(z.imm, 0b00000000000000000000000110100000);
}

TEST(ParserTest, Splitter_8) {
    //sw	zero,416(s0)
    auto z = P.Splitter(0x37010200, 0x1357ABCD);
    //0b 100000 00010 0110111
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::U);
    EXPECT_EQ(z.rd, 0b00010);
    EXPECT_EQ(z.imm, 0b00000000000000100000000000000000);
}

TEST(ParserTest, Splitter_9) {
    //beq	a4,a5,12e0
    auto z = P.Splitter(0xe308f7fe, 0x1357ABCD);
    //0b1111111 01111 01110 000 10001 1100011
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::B);
    EXPECT_EQ(z.funct3, 0b000);
    EXPECT_EQ(z.rs1, 0b01110);
    EXPECT_EQ(z.rs2, 0b01111);
    EXPECT_EQ(z.imm, 0b11111111111111111111111111110000);
}

TEST(ParserTest, Splitter_10) {
    //jal	ra,1000
    auto z = P.Splitter(0xeff09ff9, 0x1357ABCD);
    //0b11111001100111111111 00001 1101111
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::J);
    EXPECT_EQ(z.rd, 0b00001);
    EXPECT_EQ(z.imm, 0b11111111111111111111111110011000);
}

TEST(ParserTest, Splitter_11) {
    //auipc sp,0x20
    auto z = P.Splitter(0x17010200, 0x1357ABCD);
    //0b100000000100010111
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::Ua);
    EXPECT_EQ(z.rd, 0b00010);
    EXPECT_EQ(z.imm, 0b00000000000000100000000000000000);
}

TEST(ParserTest, Splitter_12) {
    auto z = P.Splitter(0x83060100, 0x1357ABCD);
    EXPECT_EQ(z.addr, 0x1357ABCD);
    EXPECT_EQ(z.instruction, command::Il);
    EXPECT_EQ(z.rd, 0b01101);
    EXPECT_EQ(z.funct3, 0b000);
    EXPECT_EQ(z.rs1, 0b00010);
    EXPECT_EQ(z.imm, 0b0);
}

TEST(MemTest, Loadtest) {
    loader l("dataset/array_test2.data");
    memory M;
    mempair m = l.getline();
    while (m.instruction != 0) {
        M.load(m.address, m.instruction);
        EXPECT_EQ(M.page[m.address / 0x1000][m.address % 0x1000], m.instruction);
        m = l.getline();
    }
}

TEST(MemTest, MemLoad_get_test) {
    memory M;
    M.memload("dataset/array_test2.data");
    auto t = P.Splitter(M.get(0x00001018), 0x00001018);
    auto z = P.Splitter(0x83470500, 0x00001018);
    EXPECT_EQ(t.instruction, z.instruction);
    EXPECT_EQ(t.addr, z.addr);
    EXPECT_EQ(t.funct3, z.funct3);
    EXPECT_EQ(t.funct7, z.funct7);
    EXPECT_EQ(t.imm, z.imm);
    EXPECT_EQ(t.rs1, z.rs1);
    EXPECT_EQ(t.rs2, z.rs2);
    EXPECT_EQ(t.rd, z.rd);
}
#include "core.hpp"
#include "globaldef.hpp"
#include "memory.hpp"
#include "parser.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <sstream>
using namespace std;

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

TEST(CoreTest, TickTest_Jump_1) {
    //This is aimed to test
    //jumping commands.
    core_session C("dataset/basic-testset/test-3.data");
    EXPECT_EQ(C.pc, 0x0000000);
    C.cycle();
    EXPECT_EQ(C.pc, 0x0000010);
    C.cycle();
    EXPECT_EQ(C.pc, 0x0000014);
    C.cycle();
    EXPECT_EQ(C.pc, 0x0000004);
    C.cycle();
    EXPECT_EQ(C.pc, 0x000000C);
    C.cycle();
    EXPECT_EQ(C.pc, 0x0000008);
    C.cycle();
    EXPECT_EQ(C.pc, 0x0000018);
}

TEST(CoreTest, MemoryWriteTest) {
    core_session C("dataset/basic-testset/test-5.data");
    for (int i = 0; i < 7; i++)
        C.cycle();
    EXPECT_EQ(C.M.get(0x00001000), 0xEFBEADDE);
    EXPECT_EQ(C.M.get(0x00001020), 0xEFBE0000);
    EXPECT_EQ(C.M.get(0x00001040), 0xEF000000);
    C.cycle();
    EXPECT_EQ(C.M.get(0x00001000), 0xFFBEADDE);
}

TEST(CoreTest, MemoryReadTest) {
    core_session C("dataset/basic-testset/test-6.data");
    EXPECT_EQ(C.M.get(0xDEADBEEF), P.rearrange(0xDEADC0DE));
    for (int i = 0; i < 7; i++)
        C.cycle();
    EXPECT_EQ(C.M.get(0x00001000), 0xEFBEADDE);
    EXPECT_EQ(C.M.get(0x00001020), 0xEFBE0000);
    EXPECT_EQ(C.M.get(0x00001040), 0xEF000000);
    C.cycle();
    EXPECT_EQ(C.M.get(0x00001000), 0xFFBEADDE);
    for (int i = 0; i < 9; i++)
        C.cycle();
    EXPECT_EQ(C.reg[13], 0xFFFFFFFF);
    EXPECT_EQ(C.reg[14], 0x000000FF);
    EXPECT_EQ(C.reg[15], 0xDEADBEFF);
    EXPECT_EQ(C.reg[16], 0xFFFFBEEF);
    EXPECT_EQ(C.reg[17], 0x0000BEEF);
    EXPECT_EQ(C.M.get(0xDEADBEEF), P.rearrange(0xDEADC0DE));
    EXPECT_GE(C.pc, 0x3c + 4);
    EXPECT_EQ(C.reg[23], 0xDEADC0DE);
}

TEST(CoreTest, JumpTest) {
    core_session C("dataset/basic-testset/test-7.data");
    taddr pc[] = {0, 0x10, 0x14, 0x4, 0xC, 0x8, 0x18};
    for (int i = 0; i < 7; i++) {
        EXPECT_EQ(C.pc, pc[i]);
        C.cycle();
    }
}

TEST(CoreTest, BranchTest_1_BEQ) {
    core_session C("dataset/basic-testset/test-8.data");
    taddr pc[] = {0x0, 0x4, 0x8, 0x10, 0x14, 0x18, 0xc};
    for (int i = 0; i < 7; i++) {
        EXPECT_EQ(C.pc, pc[i]);
        C.cycle();
    }
}

TEST(CoreTest, BranchTest_2_BNE) {
    core_session C("dataset/basic-testset/test-9.data");
    taddr pc[] = {0x0, 0x4, 0x8, 0x10, 0x14, 0x18, 0x1c};
    for (int i = 0; i < 7; i++) {
        EXPECT_EQ(C.pc, pc[i]);
        C.cycle();
    }
}

TEST(CoreTest, BranchTest_3_BLT_U) {
    core_session C("dataset/basic-testset/test-A.data");
    taddr pc[] = {0x0, 0x4, 0x8, 0x10, 0x14, 0x18, 0x1c, 0x20, 0x24, 0x0c};
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(C.pc, pc[i]);
        C.cycle();
    }
}

TEST(CoreTest, BranchTest_4_BGE_U) {
    core_session C("dataset/basic-testset/test-B.data");
    taddr pc[] = {0x0, 0x4, 0x8, 0x10, 0x14, 0x18, 0x20, 0x28, 0x2c, 0x0c};
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(C.pc, pc[i]);
        C.cycle();
    }
}

TEST(CoreTest, ReturnTest_1) {
    core_session C("dataset/sample.data");
    EXPECT_EQ(C.run(), 94);
};
#include "core.hpp"
#include "globaldef.hpp"
#include "gtest/gtest.h"
#include <iostream>
using namespace std;
#define G(AAA, b)                                \
    TEST(FileTest, AAA) {                        \
        core_session C("dataset/" #AAA ".data"); \
        EXPECT_EQ(C.run(), b);                   \
    }

G(sample, 94);
G(array_test1, 123);
G(array_test2, 43);
G(basicopt1, 88);
G(gcd, 178);
G(lvalue2, 175);
G(manyarguments, 40);
G(naive, 94);
// G(qsort, 105);
//TIME
// G(bulgarian, 159);
// G(expr, 58);
// G(magic, 106);
// G(multiarray, 115);
// G(pi, 137);
// G(queens, 171);
// G(statement_test, 50);
// G(tak, 186);
//ERROR
// G(qsort, 105);
// G(hanoi, 20);
// G(superloop, 134);

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    // core_session C("dataset/qsort.data");
    // C.debug_run();
}
