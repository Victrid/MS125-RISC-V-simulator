#ifndef __MEMORY_HPP__
#define __MEMORY_HPP__
#include "globaldef.hpp"
#include "parser.hpp"
#include <iostream>
#include <map>
class loader {
public:
    std::istream* file;
    taddr baseaddr;
    char inputline[100];
    int hextoint(char c);
    mempair getline();
    loader(const char* filepath);
    loader() : file() { file = &std::cin; }
};

class Memory {
public:
    std::map<taddr, unsigned char*> page;
    int load(taddr address, taddr c);
    taddr get(taddr address);
    void pagetest(taddr address);
    int memload(const char* c);
    int memload();

public:
    Memory(){};
};

#endif