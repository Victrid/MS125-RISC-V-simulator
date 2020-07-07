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
    command Constructor(taddr operation, taddr baseaddr);
    command RConstructor(taddr operation, taddr baseaddr);
    command IConstructor(taddr operation, taddr baseaddr);
    command IsConstructor(taddr operation, taddr baseaddr);
    command SConstructor(taddr operation, taddr baseaddr);
    command UConstructor(taddr operation, taddr baseaddr);
    command BConstructor(taddr operation, taddr baseaddr);
    static taddr getdigits(taddr content, int low, int high);

public:
    Parser() : file() { file = &cin; }
    Parser(const char* filepath);
    mempair getline();
};

#endif