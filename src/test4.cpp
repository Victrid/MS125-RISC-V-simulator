#include "core_pipeline.hpp"
#include "globaldef.hpp"
#include "gtest/gtest.h"
#include <iostream>

TEST(StageTest, IFtest_seq_and_jump) {
    core_session C("dataset/array_test1.data");
    EXPECT_EQ(C.npc, 0);
    C.cIF.tick();
    EXPECT_EQ(C.npc, 4);
    EXPECT_EQ(C.cID.ActionQueue.front().instruction, 0x37010200);
    EXPECT_EQ(C.cID.ActionQueue.front().address, 0);
    C.pcmod(0x1000);
    EXPECT_EQ(C.npc, 0x1000);
    C.cIF.tick();
    EXPECT_EQ(C.npc, 0x1004);
    C.cID.ActionQueue.pop();
    EXPECT_EQ(C.cID.ActionQueue.front().instruction, 0x37170000);
    EXPECT_EQ(C.cID.ActionQueue.front().address, 0x1000);
}

TEST(StageTest, IFtest_stall) {
    core_session C("dataset/array_test1.data");
    EXPECT_EQ(C.npc, 0);
    C.jumpstall();
    C.cIF.tick();
    EXPECT_EQ(C.npc, 0);
    EXPECT_TRUE(C.cID.ActionQueue.empty());
    C.pcmod(0x1000);
    EXPECT_EQ(C.npc, 0x1000);
    C.cIF.tick();
    EXPECT_EQ(C.npc, 0x1000);
    C.cIF.stall = false;
    C.cIF.tick();
    EXPECT_EQ(C.npc, 0x1004);
    EXPECT_EQ(C.cID.ActionQueue.front().instruction, 0x37170000);
    EXPECT_EQ(C.cID.ActionQueue.front().address, 0x1000);
}

TEST(StageTest, IDtest) {
    core_session C("dataset/array_test1.data");
    C.reg[5]  = 0x1873F;
    C.reg[10] = 0x187EF;
    EXPECT_TRUE(C.cID.ActionQueue.empty());
    C.cID.enqueue(mempair{0, 0x37010200});
    EXPECT_FALSE(C.cID.ActionQueue.empty());
    C.cID.tick();
    EXPECT_TRUE(C.cID.ActionQueue.empty());
    EXPECT_EQ(C.cEX.ActionQueue.front().addr, 0);
    EXPECT_EQ(C.cEX.ActionQueue.front().instruction, instr::U);
    EXPECT_EQ(C.cEX.ActionQueue.front().funct3, 0);
    EXPECT_EQ(C.cEX.ActionQueue.front().funct7, 0);
    EXPECT_EQ(C.cEX.ActionQueue.front().imm, 0x20000);
    EXPECT_EQ(C.cEX.ActionQueue.front().rs1, 0);
    EXPECT_EQ(C.cEX.ActionQueue.front().rs2, 0);
    EXPECT_EQ(C.cEX.ActionQueue.front().rd, 2);
    C.cEX.ActionQueue.pop();
    EXPECT_TRUE(C.cEX.ActionQueue.empty());
    C.cID.enqueue(mempair{0x1357ABCD, 0x1305d50a});
    C.jumpstall();
    C.cID.tick();
    EXPECT_FALSE(C.cID.ActionQueue.empty());
    EXPECT_TRUE(C.cEX.ActionQueue.empty());
    C.cID.stall = false;
    C.cID.tick();
    EXPECT_TRUE(C.cID.ActionQueue.empty());
    EXPECT_EQ(C.cEX.ActionQueue.front().addr, 0x1357ABCD);
    EXPECT_EQ(C.cEX.ActionQueue.front().instruction, instr::I);
    EXPECT_EQ(C.cEX.ActionQueue.front().funct3, 0b000);
    EXPECT_EQ(C.cEX.ActionQueue.front().funct7, 0);
    EXPECT_EQ(C.cEX.ActionQueue.front().imm, 0b00000000000000000000000010101101);
    EXPECT_EQ(C.cEX.ActionQueue.front().rs1, 0x187EF);
    EXPECT_EQ(C.cEX.ActionQueue.front().rs2, 0);
    EXPECT_EQ(C.cEX.ActionQueue.front().rd, 0b01010);
}

TEST(StageTest, EXTest) {
    core_session C("dataset/array_test1.data");
    EXPECT_TRUE(C.cEX.ActionQueue.empty());
    excute e;
    e.addr        = 0;
    e.instruction = instr::J;
    e.rd          = 0;
    e.imm         = 0x8;
    C.cEX.ActionQueue.push(e);
    C.cEX.tick();
    EXPECT_EQ(C.npc, 8);
    EXPECT_TRUE(C.cEX.empty());
    EXPECT_FALSE(C.cMEM.empty());
    EXPECT_TRUE(C.jumpstallflag);
    EXPECT_EQ(C.cMEM.ActionQueue.front().rdproc, true);
    EXPECT_EQ(C.cMEM.ActionQueue.front().rdwr.instruction, 0x4);
    EXPECT_EQ(C.cMEM.ActionQueue.front().rdwr.address, 0x0);
}

TEST(StageTest, MEMTest) {
    core_session C("dataset/array_test1.data");
    EXPECT_TRUE(C.cMEM.ActionQueue.empty());
    memmanip m;
    m.rdproc           = true;
    m.rdwr.instruction = 0x4;
    m.rdwr.address     = 0x0;
    C.cMEM.ActionQueue.push(m);
    C.cMEM.tick();
    EXPECT_TRUE(C.cMEM.empty());
    EXPECT_FALSE(C.cWB.empty());
    EXPECT_EQ(C.cWB.ActionQueue.front().address, 0x0);
    EXPECT_EQ(C.cWB.ActionQueue.front().instruction, 0x4);
    C.cWB.ActionQueue.pop();

    m.rdproc  = false;
    m.read    = false;
    m.address = 0xDEADBEEF;
    m.bits    = 8;
    m.content = 0x12345678;
    C.cMEM.ActionQueue.push(m);
    C.cMEM.tick();
    EXPECT_TRUE(C.cMEM.empty());
    EXPECT_TRUE(C.cWB.empty());
    EXPECT_EQ(C.memory.get(0xDEADBEEF), 0x78000000);

    m.rdproc  = false;
    m.read    = true;
    m.address = 0xDEADBEEF;
    m.bits    = 16;
    m.rd      = 1;
    m.sign    = true;
    C.cMEM.ActionQueue.push(m);
    C.cMEM.tick();
    EXPECT_TRUE(C.cMEM.empty());
    EXPECT_FALSE(C.cWB.empty());
    EXPECT_EQ(C.cWB.ActionQueue.front().address, 1);
    EXPECT_EQ(C.cWB.ActionQueue.front().instruction, 0x78);
    C.cWB.ActionQueue.pop();
}

TEST(StageTest, WBTest) {
    core_session C("dataset/array_test1.data");
    EXPECT_TRUE(C.cWB.ActionQueue.empty());
    C.cWB.ActionQueue.push(mempair{0x0, 0x5});
    C.cWB.tick();
    EXPECT_EQ(C.reg[0], 0);
    C.cWB.ActionQueue.push(mempair{0x1, 0x5});
    C.regoccupy[1] = 1;
    C.cWB.tick();
    EXPECT_EQ(C.reg[1], 5);
    EXPECT_EQ(C.regoccupy[1], 0);
}

TEST(TotalTest, DataHazardTest) {
    core_session C("dataset/pipeline-testset/test-datahazard.data");
    int ret;
    ret = C.tick();
    EXPECT_FALSE(C.datastallflag);
    EXPECT_FALSE(C.termflag);
    ret = C.tick();
    EXPECT_FALSE(C.datastallflag);
    EXPECT_FALSE(C.termflag);
    ret = C.tick();
    EXPECT_FALSE(C.datastallflag);
    EXPECT_FALSE(C.termflag);
    ret = C.tick();
    EXPECT_FALSE(C.datastallflag);
    EXPECT_FALSE(C.termflag);
    ret = C.tick();
    EXPECT_FALSE(C.datastallflag);
    EXPECT_FALSE(C.termflag);
    ret = C.tick();
    EXPECT_FALSE(C.datastallflag);
    EXPECT_FALSE(C.termflag);
    ret = C.tick();
    EXPECT_TRUE(C.datastallflag);
    EXPECT_FALSE(C.termflag);
    ret = C.tick();
    EXPECT_TRUE(C.datastallflag);
    EXPECT_FALSE(C.termflag);
    ret = C.tick();
    EXPECT_FALSE(C.datastallflag);
    EXPECT_FALSE(C.termflag);
    ret = C.tick();
    EXPECT_FALSE(C.datastallflag);
    EXPECT_FALSE(C.termflag);
    ret = C.tick();
    EXPECT_FALSE(C.datastallflag);
    EXPECT_TRUE(C.termflag);
    ret = C.tick();
    EXPECT_FALSE(C.datastallflag);
    EXPECT_TRUE(C.termflag);
    EXPECT_EQ(ret, 3);
}

TEST(TotalTest, DataHazard_Run) {
    core_session C("dataset/pipeline-testset/test-datahazard.data");
    EXPECT_EQ(C.run(), 3);
}

TEST(TotalTest, SampleTest) {
    core_session C("dataset/sample.data");
    EXPECT_EQ(C.run(), 94);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}