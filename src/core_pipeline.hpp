#ifndef __CORE_PIPELINE_HPP__
#define __CORE_PIPELINE_HPP__
#include "core_stage.hpp"
#include "globaldef.hpp"
#include "memory.hpp"
#include "parser.hpp"
#include <cstring>
#include <queue>

class core_session {
public:
    bool query(command t);
    void occupy(command t);
    void release(taddr t);
    void releasemem(taddr t);
    void jumpstall();
    void datastall();
    void pcmod(taddr pc);
    void term();
    taddr reg[32];
    taddr npc;
    memory memory;

    WB WB;
    MEM MEM;
    EX EX;
    ID ID;
    IF IF;
    
    core_session(const char* ch);
};

#endif