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
    unsigned int baseaddr;
    char inputline[100];
    int hextoint(char c);
    static int Bseries[] = {};

public:
    Parser() : file() {}
    Parser(const char* filepath);
    command getline();
};

#endif