#ifndef __parser_hpp__
#define __parser_hpp__

#include "globaldef.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

class Parser {
public:
    static void padimm(taddr& imm, int digit);
    static int fint(const taddr& t);
    static taddr ftaddr(const int& t);
    static taddr rearrange(const taddr& t);
    static taddr getdigits(taddr content, int low, int high);
    static command Splitter(taddr operation, taddr baseaddr);
    static command RSplitter(taddr operation, taddr baseaddr);
    static command ISplitter(taddr operation, taddr baseaddr);
    static command IsSplitter(taddr operation, taddr baseaddr);
    static command IlSplitter(taddr operation, taddr baseaddr);
    static command SSplitter(taddr operation, taddr baseaddr);
    static command USplitter(taddr operation, taddr baseaddr);
    static command BSplitter(taddr operation, taddr baseaddr);
    static command JSplitter(taddr operation, taddr baseaddr);
    static std::ostream& displayer(command& c, std::ostream& os);
    Parser(){};
};

#endif