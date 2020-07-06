#ifndef __parser_hpp__
#define __parser_hpp__

#include "globaldef.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

class Parser {
private:
    fstream file;
    taddr baseaddr;
    char inputline[100];
    int hextoint(char c);
    command Constructor(taddr operation, taddr baseaddr);
    command RConstructor(taddr operation, taddr baseaddr);
    command IConstructor(taddr operation, taddr baseaddr);
    command SConstructor(taddr operation, taddr baseaddr);
    command UConstructor(taddr operation, taddr baseaddr);
    command BConstructor(taddr operation, taddr baseaddr);
    taddr getdigits(taddr content, int low, int high);

public:
    Parser() : file() {}
    Parser(const char* filepath);
    mempair getline();
};

#endif