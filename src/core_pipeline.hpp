#ifndef __CORE_PIPELINE_HPP__
#define __CORE_PIPELINE_HPP__
#include "globaldef.hpp"
#include "memory.hpp"
#include "parser.hpp"
#include <cstring>
#include <queue>
class core_session;
class stage {
public:
    core_session* core;
    stage(core_session* c);
    virtual int tick()   = 0;
    virtual bool empty() = 0;
    bool stall;
};

class WB : public stage {
public:
    queue<mempair> ActionQueue;
    mempair Action;
    int enqueue(mempair m);
    int tick();
    bool empty();
    WB(core_session* c);
};

class MEM : public stage {
public:
    Parser P;
    queue<memmanip> ActionQueue;
    memmanip Action;
    int enqueue(memmanip m);
    int tick();
    bool empty();
    MEM(core_session* c);
};

class EX : public stage {
public:
    Parser P;
    queue<excute> ActionQueue;
    excute Action;
    int enqueue(excute m);
    int tick();
    bool empty();
    EX(core_session* c);
};

class ID : public stage {
    Parser P;

public:
    queue<mempair> ActionQueue;
    mempair Action;
    int enqueue(mempair m);
    int tick();
    bool empty();
    ID(core_session* c);
};

class IF : public stage {
public:
    int tick();
    bool empty();
    IF(core_session* c);
};

class core_session {
public:
    bool query(command t);
    void occupy(command t);
    void release(taddr t);

    bool jumpstallflag;
    bool datastallflag;
    bool termflag;

    void jumpstall();
    void datastall();
    void pcmod(taddr pc);
    int term();
    taddr reg[32];
    taddr npc;
    Memory memory;

    int regoccupy[32];

    WB cWB;
    MEM cMEM;
    EX cEX;
    ID cID;
    IF cIF;

    core_session(const char* ch);
    core_session();
    int tick();
    int run();
};

#endif