#include "parser.hpp"
#include "globaldef.hpp"

Parser::Parser(const char* filepath) : file(filepath) {
    if (!file)
        throw runtime_error("File error");
}

int Parser::hextoint(char c) {
    if ('0' <= c && c <= '9')
        return c - '0';
    else
        return c - 'A' + 10;
}

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
    Constructor(operation, baseaddr);
    baseaddr += 4;
}
command Parser::Constructor(unsigned int operation, unsigned int baseaddr) {
    taddr opcode = (operation - (operation >> 7 << 7)) >> 2;
    //TODO
}
taddr Parser::getdigits(taddr content, int l, int r) {
    return (content - (content >> r << r)) >> l;
};
command Parser::RConstructor(unsigned int operation, unsigned int baseaddr) {
    command c;
    c.addr        = baseaddr;
    c.instruction = command::R;
    c.rd          = getdigits(operation, 6, 11);
    c.funct3      = getdigits(operation, 11, 14);
    c.rs1         = getdigits(operation, 14, 19);
    c.rs2         = getdigits(operation, 19, 24);
    c.funct7      = getdigits(operation, 24, 31);
    return c;
}
command Parser::IConstructor(unsigned int operation, unsigned int baseaddr) {
    command c;
    c.addr        = baseaddr;
    c.instruction = command::I;
    c.rd          = getdigits(operation, 6, 11);
    c.funct3      = getdigits(operation, 11, 14);
    c.rs1         = getdigits(operation, 14, 19);
    c.imm         = getdigits(operation, 19, 31);
    return c;
}
command Parser::SConstructor(unsigned int operation, unsigned int baseaddr) {
    command c;
    c.addr        = baseaddr;
    c.instruction = command::S;
    c.funct3      = getdigits(operation, 11, 14);
    c.rs1         = getdigits(operation, 14, 19);
    c.rs2         = getdigits(operation, 19, 24);
    c.imm         = (getdigits(operation, 24, 31) << 5) + getdigits(operation, 6, 11);
    return c;
}
