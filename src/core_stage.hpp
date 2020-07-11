#ifndef __CORE_STAGE_HPP__
#define __CORE_STAGE_HPP__

#include "core_pipeline.hpp"
#include "globaldef.hpp"
#include "memory.hpp"
#include "parser.hpp"
#include <cstring>
#include <queue>

class stage {
public:
    core_session& core;
    stage(core_session& c);
    bool stall;
};

class WB : public stage {
public:
    queue<mempair> WriteReg;
    mempair towrite;
    int enqueue(mempair m);
    int tick();
    WB(core_session& c);
};

class MEM : public stage {
public:
    Parser P;
    queue<memmanip> MemMan;
    memmanip tomanip;
    int enqueue(memmanip m);
    int tick();
    MEM(core_session& c);
};

class EX : public stage {
public:
    Parser P;
    queue<excute> ComEX;
    excute toex;
    int enqueue(excute m);
    int tick();
    EX(core_session& c);
};

class ID : public stage {
    Parser P;

public:
    queue<mempair> decodelst;
    mempair getdecode;
    int enqueue(mempair m);
    int tick();
    ID(core_session& c);
};

class IF : public stage {
public:
    int tick();
    IF(core_session& c);
};

#endif