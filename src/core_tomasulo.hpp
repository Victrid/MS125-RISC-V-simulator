#ifndef __CORE_TOMASULO_HPP__
#define __CORE_TOMASULO_HPP__
#include "globaldef.hpp"
#include "memory.hpp"
#include "parser.hpp"
#include "predictor.hpp"
#include <queue>
class core_session;

struct rentab {
    taddr tab[16];
    taddr& operator[](const branchcnt& branch);
    void branching(branchcnt& father);
    rentab();
    //selecting a branch will copy the whole rename table.
};

class resstation {
    unsigned int number;
    command exc;
    bool waiting_rs1 = true;
    bool waiting_rs2 = true;

public:
    core_session* core;
    bool empty = true;
    bool ready = false;
    bool busy  = false;
    branchcnt branchselect;
    void CDBcall(taddr regname, taddr value);
    resstation(core_session* c);
    resstation(){};
    void load(excute command, branchcnt branchselect);
    void issue(int alu);
    void release();
};

class ALU {
public:
    core_session* core;
    Parser P;
    excute Action;
    taddr restation;
    branchcnt branchselect;
    bool empty;
    int load(excute m, taddr restation, branchcnt branchselect);
    int tick();
    ALU(core_session* c);
    ALU(){};
};

class CDB {
    std::queue<std::pair<taddr, taddr>> commondatabus;

public:
    void publish(taddr resstation, taddr value);
    void nowselection(branchcnt branchselect);
};

class MEM {
    std::queue<std::pair<taddr, memmanip>> q;

public:
    void enqueue(taddr,memmanip);
};

class core_session {
public:
    bool termflag;
    bool terminated;

    taddr reg[32];

    int branchunsolve = 0;
    branchcnt current;

    rentab rt[32];
    resstation RS[32];
    ALU A[4];

    Memory memory;
    MEM memqueue;

    CDB bus;

    taddr PCfile[16];

    Predictor Pr;

    int prediction[16];
    int stall[16];

    //FETCH 4 predict command then 1 predict fail branch command
    //to have some kind of speculation.

    core_session(const char* ch);
    core_session();

    void term();

    int tick();
    int run();

    void pcmodandrelease(taddr,branchcnt);
    void releasejalr();

    void branch_select(branchcnt s);

    int superfetch();

    std::pair<bool, branchcnt> getunocpy(branchcnt);
};

#endif
