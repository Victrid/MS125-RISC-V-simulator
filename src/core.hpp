#ifndef __CORE_HPP__
#define __CORE_HPP__
#include "globaldef.hpp"
#include "memory.hpp"
#include "parser.hpp"
#include <cstring>
class core_session {
public:
    //MEMORY
    memory M;
    //PARSER
    Parser P;
    //PC
    taddr pc;
    taddr seq_pc;
    taddr mod_pc;

    //REGISTER
    taddr reg[32];

    //PROC_REG
    taddr rs1;
    taddr rs2;
    taddr rd;
    taddr imm;
    bool frd;

    taddr fterm;

    taddr ex_result;

    //SEQ_STATUS INDICATOR
    int round;

    taddr loaded_memory;
    command C;

    int retval;
    int fmemory;
    struct {
        taddr addr;
        taddr bits;
        taddr content;
    } mmod;
    struct {
        taddr addr;
        taddr bits;
        taddr regp;
        bool sign;
    } mreq;
    core_session(const char* c);
    ostream& printmem(ostream& os);
    int tick();
    int cycle();
    int run();
    int debug_run();
    //fetch
};

#endif