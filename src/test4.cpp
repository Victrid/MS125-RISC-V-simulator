#include "core_pipeline.hpp"
#include "globaldef.hpp"
#include "gtest/gtest.h"
#include <iostream>

// TEST(StageTest, IFtest_seq_and_jump) {
//     core_session C("dataset/array_test1.data");
//     EXPECT_EQ(C.npc, 0);
//     C.cIF.tick();
//     EXPECT_EQ(C.npc, 4);
//     EXPECT_EQ(C.cID.ActionQueue.front().instruction, 0x37010200);
//     EXPECT_EQ(C.cID.ActionQueue.front().address, 0);
//     C.pcmod(0x1000);
//     EXPECT_EQ(C.npc, 0x1000);
//     C.cIF.tick();
//     EXPECT_EQ(C.npc, 0x1004);
//     C.cID.ActionQueue.pop();
//     EXPECT_EQ(C.cID.ActionQueue.front().instruction, 0x37170000);
//     EXPECT_EQ(C.cID.ActionQueue.front().address, 0x1000);
// }

// TEST(StageTest, IFtest_stall) {
//     core_session C("dataset/array_test1.data");
//     EXPECT_EQ(C.npc, 0);
//     C.jumpstall();
//     C.cIF.tick();
//     EXPECT_EQ(C.npc, 0);
//     EXPECT_TRUE(C.cID.ActionQueue.empty());
//     C.pcmod(0x1000);
//     EXPECT_EQ(C.npc, 0x1000);
//     C.cIF.tick();
//     EXPECT_EQ(C.npc, 0x1000);
//     C.cIF.stall = false;
//     C.cIF.tick();
//     EXPECT_EQ(C.npc, 0x1004);
//     EXPECT_EQ(C.cID.ActionQueue.front().instruction, 0x37170000);
//     EXPECT_EQ(C.cID.ActionQueue.front().address, 0x1000);
// }

// TEST(StageTest, IDtest) {
//     core_session C("dataset/array_test1.data");
//     C.reg[5]  = 0x1873F;
//     C.reg[10] = 0x187EF;
//     EXPECT_TRUE(C.cID.ActionQueue.empty());
//     C.cID.enqueue(mempair{0, 0x37010200});
//     EXPECT_FALSE(C.cID.ActionQueue.empty());
//     C.cID.tick();
//     EXPECT_TRUE(C.cID.ActionQueue.empty());
//     EXPECT_EQ(C.cEX.ActionQueue.front().addr, 0);
//     EXPECT_EQ(C.cEX.ActionQueue.front().instruction, instr::U);
//     EXPECT_EQ(C.cEX.ActionQueue.front().funct3, 0);
//     EXPECT_EQ(C.cEX.ActionQueue.front().funct7, 0);
//     EXPECT_EQ(C.cEX.ActionQueue.front().imm, 0x20000);
//     EXPECT_EQ(C.cEX.ActionQueue.front().rs1, 0);
//     EXPECT_EQ(C.cEX.ActionQueue.front().rs2, 0);
//     EXPECT_EQ(C.cEX.ActionQueue.front().rd, 2);
//     C.cEX.ActionQueue.pop();
//     EXPECT_TRUE(C.cEX.ActionQueue.empty());
//     C.cID.enqueue(mempair{0x1357ABCD, 0x1305d50a});
//     C.jumpstall();
//     C.cID.tick();
//     EXPECT_FALSE(C.cID.ActionQueue.empty());
//     EXPECT_TRUE(C.cEX.ActionQueue.empty());
//     C.cID.stall = false;
//     C.cID.tick();
//     EXPECT_TRUE(C.cID.ActionQueue.empty());
//     EXPECT_EQ(C.cEX.ActionQueue.front().addr, 0x1357ABCD);
//     EXPECT_EQ(C.cEX.ActionQueue.front().instruction, instr::I);
//     EXPECT_EQ(C.cEX.ActionQueue.front().funct3, 0b000);
//     EXPECT_EQ(C.cEX.ActionQueue.front().funct7, 0);
//     EXPECT_EQ(C.cEX.ActionQueue.front().imm, 0b00000000000000000000000010101101);
//     EXPECT_EQ(C.cEX.ActionQueue.front().rs1, 0x187EF);
//     EXPECT_EQ(C.cEX.ActionQueue.front().rs2, 0);
//     EXPECT_EQ(C.cEX.ActionQueue.front().rd, 0b01010);
// }

// TEST(StageTest, EXTest) {
//     core_session C("dataset/array_test1.data");
// }

TEST(TotalTest, Test1) {
    core_session C("dataset/sample.data");
    cout << C.run();
}

int main(int argc, char** argv) {
    // testing::InitGoogleTest(&argc, argv);
    // return RUN_ALL_TESTS();
    core_session C("dataset/sample.data");
    cout << C.run();
}