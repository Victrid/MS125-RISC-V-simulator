#ifndef __CORE_TOMASULO_HPP__
#define __CORE_TOMASULO_HPP__
#include "globaldef.hpp"
#include "memory.hpp"
#include "parser.hpp"
#include <queue>
class core_session;

struct rentab {
    taddr tab[16];
    taddr& operator[](const shiftreg& branch);
    //selecting a branch will copy the whole rename table.
};

class resstation {
    unsigned int number;
    core_session* core;
    command exc;
    bool waiting_rs1 = true;
    bool waiting_rs2 = true;

public:
    bool empty = true;
    bool ready = false;
    bool busy  = false;
    shiftreg branchselect;
    void CDBcall(taddr regname, taddr value);
    resstation(core_session* c);
    void load(excute command, shiftreg branchselect);
    void issue(int alu);
    void release();
};

class ALU {
public:
    core_session* core;
    Parser P;
    excute Action;
    taddr restation;
    shiftreg branchselect;
    bool empty;
    int load(excute m, taddr restation, shiftreg branchselect);
    int tick();
    ALU(core_session* c);
};

class CDB {
    std::queue<std::pair<taddr, taddr>> branch_res_file[16];

public:
    void publish(taddr address, taddr value, shiftreg branchselect);
    void nowselection(shiftreg branchselect);
};

class MEM {
    std::queue<std::pair<taddr, memmanip>> q;
    void enqueue(memmanip);
};

class core_session {
public:
    bool termflag;
    bool terminated;

    taddr reg[32];
    taddr renametable[32];

    int branchunsolve = 0;

    rentab rt[32];
    resstation RS[32];
    ALU A[4];

    Memory memory;
    MEM memqueue;

    CDB bus;

    core_session(const char* ch);
    core_session();

    void term();

    int tick();
    int run();
};

#endif
