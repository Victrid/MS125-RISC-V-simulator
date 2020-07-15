#include "memory.hpp"

int Memory::load(taddr address, taddr c) {
    pagetest(address);
    page[address / 0x1000][address % 0x1000]             = Parser::getdigits(c, 23, 31);
    page[(address + 1) / 0x1000][(address + 1) % 0x1000] = Parser::getdigits(c, 15, 23);
    page[(address + 2) / 0x1000][(address + 2) % 0x1000] = Parser::getdigits(c, 7, 15);
    page[(address + 3) / 0x1000][(address + 3) % 0x1000] = Parser::getdigits(c, -1, 7);
    return 0;
}

taddr Memory::get(taddr address) {
    pagetest(address);
    return ((taddr)page[address / 0x1000][address % 0x1000]) << 24 |
           ((taddr)page[(address + 1) / 0x1000][(address + 1) % 0x1000]) << 16 |
           ((taddr)page[(address + 2) / 0x1000][(address + 2) % 0x1000]) << 8 |
           ((taddr)page[(address + 3) / 0x1000][(address + 3) % 0x1000]);
}

void Memory::pagetest(taddr address) {
    if (page.find(address / 0x1000) == page.end()) {
        page.insert(std::pair<taddr, unsigned char*>(address / 0x1000, new unsigned char[0x1000]()));
    }
    if (page.find((address + 4) / 0x1000) == page.end()) {
        page.insert(std::pair<taddr, unsigned char*>((address + 4) / 0x1000, new unsigned char[0x1000]()));
    }
}

int Memory::memload(const char* c) {
    loader l(c);
    mempair m = l.getline();
    while (m.instruction != 0) {
        load(m.address, m.instruction);
        m = l.getline();
    }
    return 0;
}

int Memory::memload() {
    loader l;
    mempair m = l.getline();
    while (m.instruction != 0) {
        load(m.address, m.instruction);
        m = l.getline();
    }
    return 0;
}

mempair loader::getline() {
    if (!((*file) >> inputline))
        return mempair{0, 0};
    if (inputline[0] == '@') {
        baseaddr = 0;
        for (int i = 0; i < 8; i++) {
            baseaddr <<= 4;
            baseaddr += hextoint(inputline[i + 1]);
        }
        return getline();
    }
    (*file) >> (inputline + 2) >> (inputline + 4) >> (inputline + 6);
    unsigned int operation = 0;
    for (int i = 0; i < 4; i++) {
        operation <<= 8;
        operation += (hextoint(inputline[i << 1]) << 4) + hextoint(inputline[i << 1 | 1]);
    }
    mempair ret{baseaddr, operation};
    baseaddr += 4;
    return ret;
}

int loader::hextoint(char c) {
    if ('0' <= c && c <= '9')
        return c - '0';
    else
        return c - 'A' + 10;
}

loader::loader(const char* filepath) : file(new std::fstream(filepath)) {
    if (!(*file))
        throw std::runtime_error("File error");
}