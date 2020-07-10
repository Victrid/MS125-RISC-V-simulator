#include "core.hpp"
#include <stdio.h>
core_session::core_session(const char* c) : M(), P() {
    M.memload(c);
    pc = 0;
    memset(reg, 0, 32 * sizeof(taddr));
    loaded_memory = 0;
    fmemory       = 0;
    fterm         = 0;
    rs1           = 0;
    rs2           = 0;
    rd            = 0;
    imm           = 0;
    frd           = 0;
}
int core_session::tick() {
    switch (round) {
    case 0: //IF
        loaded_memory = M.get(pc);
        seq_pc        = pc + 4;
        mod_pc        = pc + 4;
        break;
    case 1: //ID
        C   = P.Splitter(loaded_memory, pc);
        rs1 = reg[C.rs1];
        rs2 = reg[C.rs2];
        rd  = C.rd;
        imm = C.imm;
        break;
    case 2: //EX
        switch (C.instruction) {
        case command::B:
            switch (C.funct3) {
            case 0b000: //BEQ
                if (rs1 == rs2)
                    mod_pc = pc + P.fint(imm);
                break;
            case 0b001: //BNE
                if (rs1 != rs2)
                    mod_pc = pc + P.fint(imm);
                break;
            case 0b100: //BLT
                if (P.fint(rs1) < P.fint(rs2))
                    mod_pc = pc + P.fint(imm);
                break;
            case 0b101: //BGE
                if (P.fint(rs1) >= P.fint(rs2))
                    mod_pc = pc + P.fint(imm);
                break;
            case 0b110: //BLTU
                if (rs1 < rs2)
                    mod_pc = pc + P.fint(imm);
                break;
            case 0b111: //BGEU
                if (rs1 >= rs2)
                    mod_pc = pc + P.fint(imm);
                break;
            }
            break;
        case command::J: //JAL
            ex_result = pc + 4;
            mod_pc    = pc + P.fint(imm);
            frd       = true;
            break;
        case command::U: //LUI
            ex_result = imm;
            frd       = true;
            break;
        case command::Ua: //AUIPC
            ex_result = imm;
            ex_result += pc;
            frd = true;
            break;
        case command::S:
            fmemory = 1;
            switch (C.funct3) {
            case 0b000: //SB
                mmod.addr    = rs1 + P.fint(imm);
                mmod.bits    = 8;
                mmod.content = rs2;
                break;
            case 0b001: //SH
                mmod.addr    = rs1 + P.fint(imm);
                mmod.bits    = 16;
                mmod.content = rs2;
                break;
            case 0b010: //SW
                mmod.addr    = rs1 + P.fint(imm);
                mmod.bits    = 32;
                mmod.content = rs2;
                break;
            }
            break;
        case command::Il:
            fmemory = -1;
            P.padimm(imm, 12);
            switch (C.funct3) { //LB
            case 0b000:
                mreq.addr = rs1 + P.fint(imm);
                mreq.bits = 8;
                mreq.regp = C.rd;
                mreq.sign = true;
                break;
            case 0b001: //LH
                mreq.addr = rs1 + P.fint(imm);
                mreq.bits = 16;
                mreq.regp = C.rd;
                mreq.sign = true;
                break;
            case 0b010: //LW
                mreq.addr = rs1 + P.fint(imm);
                mreq.bits = 32;
                mreq.regp = C.rd;
                mreq.sign = true;
                break;
            case 0b100: //LBU
                mreq.addr = rs1 + P.fint(imm);
                mreq.bits = 8;
                mreq.regp = C.rd;
                mreq.sign = false;
                break;
            case 0b101: //LHU
                mreq.addr = rs1 + P.fint(imm);
                mreq.bits = 16;
                mreq.regp = C.rd;
                mreq.sign = false;
                break;
            }
            break;
        case command::I:
            switch (C.funct3) {
            case 0b000: //ADDI
                if ((C.rd == 10) && (C.rs1 == 0) && imm == 255) {
                    //TERM
                    retval = reg[0b01010] & 255u;
                    fterm  = true;
                } else {
                    ex_result = P.ftaddr(P.fint(rs1) + P.fint(imm));
                    frd       = true;
                }
                break;
            case 0b010: //SLTI
                P.padimm(imm, 12);
                ex_result = (P.fint(rs1) < P.fint(imm));
                frd       = true;
                break;
            case 0b011: //SLTIU
                P.padimm(imm, 12);
                ex_result = (rs1 < imm);
                frd       = true;
                break;
            case 0b100: //XORI
                P.padimm(imm, 12);
                ex_result = rs1 ^ imm;
                frd       = true;
                break;
            case 0b110: //ORI
                P.padimm(imm, 12);
                ex_result = rs1 | imm;
                frd       = true;
                break;
            case 0b111: //ANDI
                P.padimm(imm, 12);
                ex_result = rs1 & imm;
                frd       = true;
                break;
            case 0b001: //SLLI
                ex_result = rs1 << imm;
                frd       = true;
                break;
            case 0b101:
                if (C.funct7 == 0) //SRLI
                    ex_result = rs1 >> imm;
                else //SRAI
                    ex_result = P.ftaddr(P.fint(rs1) >> imm);
                frd = true;
                break;
            }
            break;
        case command::Ij: //JALR
            ex_result = pc + 4;
            mod_pc    = rs1 + P.fint(imm);
            frd       = true;
            break;
        case command::R:
            switch (C.funct3) {
            case 0b000:
                if (C.funct7 == 0) //ADD
                    ex_result = P.ftaddr(P.fint(rs1) + P.fint(rs2));
                else //SUB
                    ex_result = P.ftaddr(P.fint(rs1) - P.fint(rs2));
                frd = true;
                break;
            case 0b001: //SLL
                ex_result = rs1 << (rs2 & 0b11111);
                frd       = true;
                break;
            case 0b010: //SLT
                ex_result = (P.fint(rs1) < P.fint(rs2));
                frd       = true;
                break;
            case 0b011: //SLTU
                ex_result = (rs1 < rs2);
                frd       = true;
                break;
            case 0b100: //XOR
                ex_result = rs1 ^ rs2;
                frd       = true;
                break;
            case 0b101:
                if (C.funct7 == 0) //SRL
                    ex_result = rs1 >> (rs2 & 0b11111);
                else //SRA
                    ex_result = P.ftaddr(P.fint(rs1) >> (rs2 & 0b11111));
                frd = true;
                break;
            case 0b110: //OR
                ex_result = rs1 | rs2;
                frd       = true;
                break;
            case 0b111: //AND
                ex_result = rs1 & rs2;
                frd       = true;
                break;
            }
            break;
        }
        break;
    case 3:
        break;
    case 4: //MEM
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
    case 5:
        if (mod_pc != seq_pc)
            pc = mod_pc;
        else
            pc = seq_pc;
        break;
    case 6: //WB
        if (frd)
            reg[rd] = ex_result;
        frd    = 0;
        reg[0] = 0;
        break;
    }
    round++;
    round %= 7;
    return (fterm);
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
    cout << "MEM\t\t0x" << std::hex << loaded_memory << "\t" << roundc[round] << endl;
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
        // printmem(cout);
        // // if (pc == 0x118c || pc == 0x1190 || pc == 0x1194 || pc == 0x1198)
        // //     ;
        // // else
        // c = getchar();
    }
    return retval;
}
