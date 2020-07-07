#ifndef __MEMORY_HPP__
#define __MEMORY_HPP__
#include "globaldef.hpp"
#include "parser.hpp"
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class memory {
public:
    map<taddr, command*> page;
    int load(taddr address, command c);
    command& get(taddr address);
    int memload(const char* c);

public:
    memory(){};
};

#endif