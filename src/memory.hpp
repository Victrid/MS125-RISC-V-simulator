#ifndef __MEMORY_HPP__
#define __MEMORY_HPP__
#include "globaldef.hpp"
#include "parser.hpp"
#include <iostream>
#include <map>
#include <vector>
using namespace std;
class loader {
public:
    istream* file;
    taddr baseaddr;
    char inputline[100];
    int hextoint(char c);
    mempair getline();
    loader(const char* filepath);
    loader() : file() { file = &cin; }
};

class Memory {
public:
    map<taddr, unsigned char*> page;
    int load(taddr address, taddr c);
    taddr get(taddr address);
    void pagetest(taddr address);
    int memload(const char* c);
    int memload();

public:
    Memory(){};
};

#endif