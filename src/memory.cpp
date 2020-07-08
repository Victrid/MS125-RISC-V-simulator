#include "memory.hpp"

int memory::load(taddr address, taddr c) {
    if (page.find(address / 0x1000) == page.end()) {
        page.insert(pair<taddr, taddr*>(address / 0x1000, new taddr[0x1000]()));
    }
    page[address / 0x1000][address % 0x1000] = c;
    return 0;
}

taddr& memory::get(taddr address) {
    pagetest(address);
    return page[address / 0x1000][address % 0x1000];
}

void memory::pagetest(taddr address) {
    if (page.find(address / 0x1000) == page.end()) {
        page.insert(pair<taddr, taddr*>(address / 0x1000, new taddr[0x1000]()));
    }
}

int memory::memload(const char* c) {
    loader l(c);
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
            baseaddr += inputline[i + 1] - '0';
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

loader::loader(const char* filepath) : file(new fstream(filepath)) {
    if (!(*file))
        throw runtime_error("File error");
}