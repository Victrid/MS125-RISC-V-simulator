#ifndef __CORE_HPP__
#define __CORE_HPP__
#include "globaldef.hpp"
#include "memory.hpp"
#include "parser.hpp"
#include <cstring>
class core_session {
public:
    taddr pc;
    taddr reg[32];
    memory M;
    Parser P;

    int round;
    taddr loadmem;
    command C;
    int fmemory;
    struct memory_mod {
        taddr addr;
        taddr bits;
        taddr content;
    } mmod;
    struct memory_req {
        taddr addr;
        taddr bits;
        taddr regp;
        bool sign;
    } mreq;
    core_session(const char* c) : M() {
        M.memload(c);
        pc = 0;
        memset(reg, 0, 32 * sizeof(taddr));
        round = 0;
    }
    int tick() {
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
                    if (reg[C.rs1] == reg[C.rs2]) {
                        pc += P.fint(C.imm);
                    } else
                        pc += 4;
                    break;
                case 0b001: //BNE
                    if (reg[C.rs1] != reg[C.rs2]) {
                        pc += P.fint(C.imm);
                    } else
                        pc += 4;
                    break;
                case 0b100: //BLT
                    if (P.fint(reg[C.rs1]) < P.fint(reg[C.rs2])) {
                        pc += P.fint(C.imm);
                    } else
                        pc += 4;
                    break;
                case 0b101: //BGE
                    if (P.fint(reg[C.rs1]) >= P.fint(reg[C.rs2])) {
                        pc += P.fint(C.imm);
                    } else
                        pc += 4;
                    break;
                case 0b110: //BLTU
                    if (reg[C.rs1] < reg[C.rs2]) {
                        pc += P.fint(C.imm);
                    } else
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
                break;
            case command::Ua: //AUIPC
                reg[C.rd] = C.imm;
                reg[C.rd] += pc;
                break;
            case command::S:
                fmemory = 1;
                switch (C.funct3) {
                case 0b000: //SB
                    mmod.addr    = reg[C.rs1] + P.fint(C.imm);
                    mmod.bits    = 8;
                    mmod.content = reg[C.rs2];
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
                break;
            case command::Il:
                fmemory = -1;
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
                    mreq.bits = 8;
                    mreq.regp = C.rd;
                    mreq.sign = false;
                    break;
                }
                break;
            case command::I:
                switch (C.funct3) {
                case 0b000: //ADDI
                    reg[C.rd] = P.ftaddr(P.fint(reg[C.rs1]) + P.fint(C.imm));
                    break;
                case 0b010: //SLTI
                    if (reg[C.rs1] < C.imm)
                        reg[C.rd] = 1;
                    else
                        reg[C.rd] = 0;
                    break;
                case 0b100: //XORI
                    reg[C.rd] = reg[C.rs1] ^ C.imm;
                    break;
                case 0b110: //ORI
                    reg[C.rd] = reg[C.rs1] | C.imm;
                    break;
                case 0b111: //ANDI
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
                break;
            }
            break;
        case 3:
        case 4:
        case 5: //MEM
        //TODO
        case 6: //WB
            break;
        }
        round++;
        if (round == 7)
            round = 0;
        return 0;
    }
    //fetch
};

#endif