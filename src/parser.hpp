#ifndef __parser_hpp__
#define __parser_hpp__

#include "globaldef.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

class Parser {
public:
    istream* file;
    taddr baseaddr;
    char inputline[100];
    int hextoint(char c);
    static void padimm(taddr& imm, int digit);
    command Splitter(taddr operation, taddr baseaddr);
    command RSplitter(taddr operation, taddr baseaddr);
    command ISplitter(taddr operation, taddr baseaddr);
    command IsSplitter(taddr operation, taddr baseaddr);
    command SSplitter(taddr operation, taddr baseaddr);
    command USplitter(taddr operation, taddr baseaddr);
    command BSplitter(taddr operation, taddr baseaddr);
    command JSplitter(taddr operation, taddr baseaddr);
    static taddr getdigits(taddr content, int low, int high);

public:
    Parser() : file() { file = &cin; }
    Parser(const char* filepath);
    mempair getline();
};

#endif