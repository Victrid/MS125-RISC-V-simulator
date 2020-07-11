//This is for parser and memory load/store test.

#include "globaldef.hpp"
#include "memory.hpp"
#include "parser.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <sstream>
using namespace std;

Parser P;

TEST(LoaderTest, getline_1) {
    loader l;
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
    //Clear
    while (t1.instruction != 0) {
        t1 = l.getline();
    }
}

TEST(LoaderTest, getline_2) {
    loader P;
    auto t1 = P.getline();
    cout << t1.address;
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
    //Clear
    while (t1.instruction != 0) {
        t1 = P.getline();
    }
}

TEST(MemTest, Loadtest) {
    loader l;
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
    M.memload();
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

TEST(MemTest, MemLoad_get_test_2) {
    memory M;
    M.memload();
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

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}