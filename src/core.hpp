#ifndef __CORE_HPP__
#define __CORE_HPP__
#include "globaldef.hpp"
#include "memory.hpp"
#include "parser.hpp"
#include <cstring>
#include <queue>
class stage;
class IF;
class ID;
class EX;
class MEM;
class WB;

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

    stage *IF, *ID, *EX, *MEM, *WB;

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

class stage {
public:
    virtual int tick()        = 0;
    virtual int enlist(void*) = 0;
    virtual bool checkfull()  = 0;
    virtual void setstall()   = 0;
    void* storage;
    bool stall;
    core_session& core;
    stage(core_session& core) : core(core), stall(false){};
};

class IF : public stage {
    queue<taddr>* qstore;
    IF(core_session& core) : stage(core) {
        storage = new queue<taddr>;
        ((queue<taddr>*)storage)->push(0);
        qstore = (queue<taddr>*)storage;
    }
    ~IF() { delete qstore; }
    taddr mem;
    int tick() {
        if ((!qstore->empty()) && (!core.ID->checkfull())) {
            taddr addr = qstore->front();
            mem        = core.M.get(qstore->front());
            qstore->pop();
            core.seq_pc = addr + 4;
            core.ID->enlist(new mempair{addr, mem});
        }
    }
    int enlist() { return 0; };
    bool checkfull() { return 0; };
};

#endif