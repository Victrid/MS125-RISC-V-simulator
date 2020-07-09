#ifndef __CORE_HPP__
#define __CORE_HPP__
#include "globaldef.hpp"
#include "memory.hpp"
#include "parser.hpp"
#include <cstring>
class core_session {
public:
    taddr pc;
    taddr reg[32];
    memory M;
    Parser P;

    int round;
    taddr loadmem;
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