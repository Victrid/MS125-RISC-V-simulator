#include "memory.hpp"
int memory::load(taddr address, command c) {
    if (page.find(address / 0x1000) == page.end()) {
        page.insert(pair<taddr, command*>(address / 0x1000, new command[0x1000]));
    }
    page[address / 0x1000][address % 0x1000] = c;
    return 0;
}
command& memory::get(taddr address) {
    return page[address / 0x1000][address % 0x1000];
}
int memory::memload(const char* c) {
    Parser P(c);
    mempair m = P.getline();
    while (m.instruction != 0) {
        auto z = P.Splitter(m.instruction, m.address);
        load(z.addr, z);
        m = P.getline();
    }
    return 0;
}