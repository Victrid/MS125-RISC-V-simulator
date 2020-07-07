#ifndef __globaldef_hpp__
#define __globaldef_hpp__
using taddr = unsigned int;

struct mempair {
    taddr address;
    taddr instruction;
};

struct command {
    taddr addr;
    enum { R,
           I,
           Il,
           Ij,
           S,
           U,
           Ua,
           J,
           B } instruction;
    taddr funct3;
    taddr funct7;
    taddr imm;
    taddr rs1;
    taddr rs2;
    taddr rd;
    command() : addr(0), instruction(R), funct3(0), funct7(0), imm(0), rs1(0), rs2(0), rd(0){};
};

#endif