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

typedef command excute;

const taddr Loadbits[] = {8, 16, 32, 0, 8, 16};
const bool Loadsign[]  = {true, true, true, false, false, false};

struct shiftreg {
    unsigned char digit = 4;
    taddr reg;
    taddr get() const{
        return reg - (reg >> digit << digit);
    }
    void ins(bool value) {
        reg = ((reg << 1) | value);
        return;
    }
    shiftreg& operator=(const shiftreg& rhs) {
        if (&rhs == this)
            return *this;
        digit = rhs.digit;
        reg   = rhs.reg;
        return *this;
    }
};

#endif