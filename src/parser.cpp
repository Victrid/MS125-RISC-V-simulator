#include "parser.hpp"
#include "globaldef.hpp"
#include <bitset>
Parser::Parser(const char* filepath) : file(filepath) {
    if (!file)
        throw runtime_error("File error");
}
int Parser::hextoint(char c) {
    if ('0' <= c && c <= '9')
        return c - '0';
    else
        return c - 'A' + 10;
};
command Parser::getline() {
    file >> inputline;
    if (inputline[0] == '@') {
        baseaddr = 0;
        for (int i = 0; i < 8; i++) {
            baseaddr <<= 4;
            baseaddr += inputline[i + 1] - '0';
        }
        return getline();
    }
    file >> (inputline + 2) >> (inputline + 4) >> (inputline + 6);
    unsigned int operation = 0;
    for (int i = 3; i >= 0; i--) {
        operation <<= 8;
        operation += (hextoint(inputline[i << 1]) << 4) + hextoint(inputline[i << 1 | 1]);
    }
    baseaddr += 4;
}
