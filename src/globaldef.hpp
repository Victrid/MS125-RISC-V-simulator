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
};

#endif