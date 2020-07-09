#include "core.hpp"
#include <stdio.h>
core_session::core_session(const char* c) : M(), P() {
    M.memload(c);
    pc = 0;
    memset(reg, 0, 32 * sizeof(taddr));
    round   = 0;
    loadmem = 0;
    fmemory = 0;
}
int core_session::tick() {
    switch (round) {
    case 0: //IF
        loadmem = M.get(pc);
        break;
    case 1: //ID
        C = P.Splitter(loadmem, pc);
        break;
    case 2: //EX
        switch (C.instruction) {
        case command::B:
            switch (C.funct3) {
            case 0b000: //BEQ
                if (reg[C.rs1] == reg[C.rs2])
                    pc += P.fint(C.imm);
                else
                    pc += 4;
                break;
            case 0b001: //BNE
                if (reg[C.rs1] != reg[C.rs2])
                    pc += P.fint(C.imm);
                else
                    pc += 4;
                break;
            case 0b100: //BLT
                if (P.fint(reg[C.rs1]) < P.fint(reg[C.rs2]))
                    pc += P.fint(C.imm);
                else
                    pc += 4;
                break;
            case 0b101: //BGE
                if (P.fint(reg[C.rs1]) >= P.fint(reg[C.rs2]))
                    pc += P.fint(C.imm);
                else
                    pc += 4;
                break;
            case 0b110: //BLTU
                if (reg[C.rs1] < reg[C.rs2])
                    pc += P.fint(C.imm);
                else
                    pc += 4;
                break;
            case 0b111: //BGEU
                if (reg[C.rs1] >= reg[C.rs2])
                    pc += P.fint(C.imm);
                else
                    pc += 4;
                break;
            }
            break;
        case command::J: //JAL
            reg[C.rd] = pc + 4;
            pc += P.fint(C.imm);
            break;
            break;
        case command::U: //LUI
            reg[C.rd] = C.imm;
            pc += 4;
            break;
        case command::Ua: //AUIPC
            reg[C.rd] = C.imm;
            reg[C.rd] += pc;
            pc += 4;
            break;
        case command::S:
            fmemory = 1;
            switch (C.funct3) {
            case 0b000: //SB
                mmod.addr    = reg[C.rs1] + P.fint(C.imm);
                mmod.bits    = 8;
                mmod.content = reg[C.rs2];
                break;
            case 0b001: //SH
                mmod.addr    = reg[C.rs1] + P.fint(C.imm);
                mmod.bits    = 16;
                mmod.content = reg[C.rs2];
                break;
            case 0b010: //SW
                mmod.addr    = reg[C.rs1] + P.fint(C.imm);
                mmod.bits    = 32;
                mmod.content = reg[C.rs2];
                break;
            }
            pc += 4;
            break;
        case command::Il:
            fmemory = -1;
            P.padimm(C.imm, 12);
            switch (C.funct3) { //LB
            case 0b000:
                mreq.addr = reg[C.rs1] + P.fint(C.imm);
                mreq.bits = 8;
                mreq.regp = C.rd;
                mreq.sign = true;
                break;
            case 0b001: //LH
                mreq.addr = reg[C.rs1] + P.fint(C.imm);
                mreq.bits = 16;
                mreq.regp = C.rd;
                mreq.sign = true;
                break;
            case 0b010: //LW
                mreq.addr = reg[C.rs1] + P.fint(C.imm);
                mreq.bits = 32;
                mreq.regp = C.rd;
                mreq.sign = true;
                break;
            case 0b100: //LBU
                mreq.addr = reg[C.rs1] + P.fint(C.imm);
                mreq.bits = 8;
                mreq.regp = C.rd;
                mreq.sign = false;
                break;
            case 0b101: //LHU
                mreq.addr = reg[C.rs1] + P.fint(C.imm);
                mreq.bits = 16;
                mreq.regp = C.rd;
                mreq.sign = false;
                break;
            }
            pc += 4;
            break;
        case command::I:
            switch (C.funct3) {
            case 0b000: //ADDI
                if ((C.rd == 10) && (C.rs1 == 0) && C.imm == 255) {
                    //TERM
                    retval = reg[0b01010] & 255u;
                    return -1;
                }
                reg[C.rd] = P.ftaddr(P.fint(reg[C.rs1]) + P.fint(C.imm));
                break;
            case 0b010: //SLTI
                P.padimm(C.imm, 12);
                if (P.fint(reg[C.rs1]) < P.fint(C.imm))
                    reg[C.rd] = 1;
                else
                    reg[C.rd] = 0;
                break;
            case 0b011: //SLTIU
                P.padimm(C.imm, 12);
                if (reg[C.rs1] < C.imm)
                    reg[C.rd] = 1;
                else
                    reg[C.rd] = 0;
                break;
            case 0b100: //XORI
                P.padimm(C.imm, 12);
                reg[C.rd] = reg[C.rs1] ^ C.imm;
                break;
            case 0b110: //ORI
                P.padimm(C.imm, 12);
                reg[C.rd] = reg[C.rs1] | C.imm;
                break;
            case 0b111: //ANDI
                P.padimm(C.imm, 12);
                reg[C.rd] = reg[C.rs1] & C.imm;
                break;
            case 0b001: //SLLI
                reg[C.rd] = reg[C.rs1] << C.imm;
                break;
            case 0b101:
                if (C.funct7 == 0) //SRLI
                    reg[C.rd] = reg[C.rs1] >> C.imm;
                else //SRAI
                    reg[C.rd] = P.ftaddr(P.fint(reg[C.rs1]) >> C.imm);
                break;
            }
            pc += 4;
            break;
        case command::Ij: //JALR
            reg[C.rd] = pc + 4;
            pc        = reg[C.rs1] + P.fint(C.imm);
            break;
        case command::R:
            switch (C.funct3) {
            case 0b000:
                if (C.funct7 == 0) //ADD
                    reg[C.rd] = P.ftaddr(P.fint(reg[C.rs1]) + P.fint(reg[C.rs2]));
                else //SUB
                    reg[C.rd] = P.ftaddr(P.fint(reg[C.rs1]) - P.fint(reg[C.rs2]));
                break;
            case 0b001: //SLL
                reg[C.rd] = reg[C.rs1] << (reg[C.rs2] & 0b11111);
                break;
            case 0b010: //SLT
                if (P.fint(reg[C.rs1]) < P.fint(reg[C.rs2]))
                    reg[C.rd] = 1;
                else
                    reg[C.rd] = 0;
                break;
            case 0b011: //SLTU
                if (reg[C.rs1] < reg[C.rs2])
                    reg[C.rd] = 1;
                else
                    reg[C.rd] = 0;
                break;
            case 0b100: //XOR
                reg[C.rd] = reg[C.rs1] ^ reg[C.rs2];
                break;
            case 0b101:
                if (C.funct7 == 0) //SRL
                    reg[C.rd] = reg[C.rs1] >> (reg[C.rs2] & 0b11111);
                else //SRA
                    reg[C.rd] = P.ftaddr(P.fint(reg[C.rs1]) >> (reg[C.rs2] & 0b11111));
                break;
            case 0b110: //OR
                reg[C.rd] = reg[C.rs1] | reg[C.rs2];
                break;
            case 0b111: //AND
                reg[C.rd] = reg[C.rs1] & reg[C.rs2];
                break;
            }
            pc += 4;
            break;
        }
        break;
    case 3:
        break;
    case 4:
        break;
    case 5: //MEM
        if (!fmemory)
            break;
        if (fmemory == 1) {
            taddr mask = P.rearrange(M.get(mmod.addr));
            switch (mmod.bits) {
            case 8:
                mask         = mask & 0xFFFFFF00;
                mmod.content = mmod.content & 0x000000FF;
                break;
            case 16:
                mask         = mask & 0xFFFF0000;
                mmod.content = mmod.content & 0x0000FFFF;
                break;
            case 32:
                mask         = mask & 0x00000000;
                mmod.content = mmod.content & 0xFFFFFFFF;
                break;
            }
            M.get(mmod.addr) = P.rearrange(mask | mmod.content);
            fmemory          = 0;
        } else {
            taddr mask = P.rearrange(M.get(mreq.addr));
            switch (mreq.bits) {
            case 8:
                mask = P.getdigits(mask, -1, 7);
                if (mreq.sign)
                    P.padimm(mask, 8);
                break;
            case 16:
                mask = P.getdigits(mask, -1, 15);
                if (mreq.sign)
                    P.padimm(mask, 16);
                break;
            case 32:
                break;
            }
            reg[mreq.regp] = mask;
            fmemory        = 0;
        }
        break;
    case 6: //WB
        reg[0] = 0;
        break;
    }
    round++;
    if (round == 7)
        round = 0;
    return 0;
}
ostream& core_session::printmem(ostream& os) {
    cout << "\x1B[2J\x1B[H";
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            cout << "x" << std::dec << i * 8 + j << "\t";
        }
        cout << std::hex << endl;
        for (int j = 0; j < 8; j++) {
            cout << reg[i * 8 + j] << "\t";
        }
        cout << endl;
    }
    const char* roundc[] = {"WB", "IF", "ID", "EX", "MEM-1", "MEM-2", "MEM-3"};
    cout << "PC\t\t0x" << std::hex << pc << "\tMEMF\t\t" << fmemory << endl;
    cout << "MEM\t\t0x" << std::hex << loadmem << "\t" << roundc[round] << endl;
    cout << "ASM \t\t";
    P.displayer(C, cout) << endl;
    return os;
}
int core_session::cycle() {
    int tickret = 0;
    for (int i = 0; i < 7 && !tickret; i++)
        tickret = tick();
    return tickret;
}

int core_session::run() {
    int tickret = 0;
    while (!tickret)
        tickret = cycle();
    return retval;
}

int core_session::debug_run() {
    int tickret    = 0;
    char c         = '\0';
    unsigned vvred = 0;
    while (!tickret) {
        tickret = cycle();
        printmem(cout);
        if (!vvred) {
            vvred--;
        } else {
            c = getchar();
        }
        if (c == '3')
            vvred = 1000;
    }
    return retval;
}
