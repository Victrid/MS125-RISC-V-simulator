#ifndef __globaldef_hpp__
#define __globaldef_hpp__
using taddr = unsigned int;

struct mempair {
    taddr address;
    taddr instruction;
};

struct command {
    taddr addr;
    //Is is separated from I for slli srli and srai.
    enum { R,
           I,
           Is,
           S,
           U,
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