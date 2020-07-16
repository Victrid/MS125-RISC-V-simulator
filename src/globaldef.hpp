#ifndef __globaldef_hpp__
#define __globaldef_hpp__
using taddr = unsigned int;

struct mempair {
    taddr address;
    taddr instruction;
};

enum instr { R,
             I,
             Il,
             Ij,
             S,
             U,
             Ua,
             J,
             B,
             T };

struct command {
    taddr addr;
    instr instruction;
    taddr funct3;
    taddr funct7;
    taddr imm;
    taddr rs1;
    taddr rs2;
    taddr rd;
    command() : addr(0), instruction(R), funct3(0), funct7(0), imm(0), rs1(0), rs2(0), rd(0){};
    command(taddr addr, bool) : addr(addr), instruction(T), funct3(0), funct7(0), imm(0), rs1(0), rs2(0), rd(0){};
};

struct memmanip {
    bool rdproc;
    bool read;
    taddr address;
    taddr bits;
    taddr content;
    taddr rd;
    bool sign;
    mempair rdwr;
};

struct tommemmanip : public memmanip {
    taddr resstationnum;
};

typedef command excute;

struct tomexcute : public excute {
    taddr resstationnum;
};

const taddr Loadbits[] = {8, 16, 32, 0, 8, 16};
const bool Loadsign[]  = {true, true, true, false, false, false};

struct branchcnt {
    //Here branch should use a loop sequence instead of shift register.
    unsigned char digit = 0;
    bool reg[4];
    taddr get() const {
        return reg[0] << 3 | reg[1] << 2 | reg[2] << 1 | reg[3];
    }
    void ins(bool value) {
        digit++;
        reg[digit] = value;
        return;
    }
    branchcnt& operator=(const branchcnt& rhs) {
        if (&rhs == this)
            return *this;
        digit = rhs.digit;
        for (int i = 0; i < 4; i++)
            reg[i] = rhs.reg[i];
        return *this;
    }
};

#endif