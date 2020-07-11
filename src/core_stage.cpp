#include "core_stage.hpp"
stage::stage(core_session& c) : core(c), stall(false){};
int WB::enqueue(mempair m) {
    WriteReg.push(m);
    return 0;
}
int WB::tick() {
    if (stall)
        return 0;
    if (!WriteReg.empty()) {
        towrite = WriteReg.front();
        WriteReg.pop();
        core.reg[towrite.address] = towrite.instruction;
        core.release(towrite.address);
    }
}
WB::WB(core_session& c) : stage(c){};

int MEM::enqueue(memmanip m) {
    MemMan.push(m);
    return 0;
}
int MEM::tick() {
    if (stall)
        return 0;
    if (!MemMan.empty()) {
        tomanip = MemMan.front();
        MemMan.pop();
        if (tomanip.rdproc) {
            core.WB.enqueue(tomanip.rdwr);
            return 0;
        }
        if (!tomanip.read) {
            taddr mask = P.rearrange(core.memory.get(tomanip.address));
            switch (tomanip.bits) {
            case 8:
                mask            = mask & 0xFFFFFF00;
                tomanip.content = tomanip.content & 0x000000FF;
                break;
            case 16:
                mask            = mask & 0xFFFF0000;
                tomanip.content = tomanip.content & 0x0000FFFF;
                break;
            case 32:
                mask            = mask & 0x00000000;
                tomanip.content = tomanip.content & 0xFFFFFFFF;
                break;
            }
            core.memory.get(tomanip.address) = P.rearrange(mask | tomanip.content);
            core.releasemem(tomanip.address);
        } else {
            taddr mask = P.rearrange(core.memory.get(tomanip.address));
            switch (tomanip.bits) {
            case 8:
                mask = P.getdigits(mask, -1, 7);
                if (tomanip.sign)
                    P.padimm(mask, 8);
                break;
            case 16:
                mask = P.getdigits(mask, -1, 15);
                if (tomanip.sign)
                    P.padimm(mask, 16);
                break;
            case 32:
                break;
            }
            core.WB.enqueue(mempair{tomanip.rd, mask});
        }
    }
}

MEM::MEM(core_session& c) : stage(c) {}

int EX::enqueue(excute m) {
    ComEX.push(m);
}

int EX::tick() {
    if (!ComEX.empty()) {
        toex = ComEX.front();
        ComEX.pop();
        memmanip man;
        switch (toex.instruction) {
        case instr::T:
            core.term();
            break;
        case instr::B:
            switch (toex.funct3) {
            case 0b000: //BEQ
                if (toex.rs1 == toex.rs2) {
                    core.jumpstall();
                    core.pcmod(toex.addr + P.fint(toex.imm));
                }
                break;
            case 0b001: //BNE
                if (toex.rs1 != toex.rs2) {
                    core.jumpstall();
                    core.pcmod(toex.addr + P.fint(toex.imm));
                }
                break;
            case 0b100: //BLT
                if (P.fint(toex.rs1) < P.fint(toex.rs2)) {
                    core.jumpstall();
                    core.pcmod(toex.addr + P.fint(toex.imm));
                }
                break;
            case 0b101: //BGE
                if (P.fint(toex.rs1) >= P.fint(toex.rs2)) {
                    core.jumpstall();
                    core.pcmod(toex.addr + P.fint(toex.imm));
                }
                break;
            case 0b110: //BLTU
                if (toex.rs1 < toex.rs2) {
                    core.jumpstall();
                    core.pcmod(toex.addr + P.fint(toex.imm));
                }
                break;
            case 0b111: //BGEU
                if (toex.rs1 >= toex.rs2) {
                    core.jumpstall();
                    core.pcmod(toex.addr + P.fint(toex.imm));
                }
                break;
            }
            break;
        case instr::J: //JAL
            core.jumpstall();
            core.pcmod(toex.addr + P.fint(toex.imm));
            man.rdproc = true;
            man.rdwr   = mempair{toex.rd, toex.addr + 4};
            core.MEM.enqueue(man);
            break;
        case instr::U: //LUI
            man.rdproc = true;
            man.rdwr   = mempair{toex.rd, toex.imm};
            core.MEM.enqueue(man);
            break;
        case instr::Ua: //AUIPC
            man.rdproc = true;
            man.rdwr   = mempair{toex.rd, toex.imm + toex.addr};
            core.MEM.enqueue(man);
            break;
        case instr::S:
            switch (toex.funct3) {
            case 0b000: //SB
                man.rdproc  = false;
                man.read    = false;
                man.address = toex.rs1 + P.fint(toex.imm);
                man.bits    = 8;
                man.content = toex.rs2;
                core.MEM.enqueue(man);
                break;
            case 0b001: //SH
                man.rdproc  = false;
                man.read    = false;
                man.address = toex.rs1 + P.fint(toex.imm);
                man.bits    = 16;
                man.content = toex.rs2;
                core.MEM.enqueue(man);
                break;
            case 0b010: //SW
                man.rdproc  = false;
                man.read    = false;
                man.address = toex.rs1 + P.fint(toex.imm);
                man.bits    = 32;
                man.content = toex.rs2;
                core.MEM.enqueue(man);
                break;
            }
            break;
        case instr::Il:
            P.padimm(toex.imm, 12);
            switch (toex.funct3) { //LB
            case 0b000:
                man.rdproc  = false;
                man.read    = true;
                man.address = toex.rs1 + P.fint(toex.imm);
                man.bits    = 8;
                man.rd      = toex.rd;
                man.sign    = true;
                man.content = toex.rs2;
                core.MEM.enqueue(man);
                break;
            case 0b001: //LH
                man.rdproc  = false;
                man.read    = true;
                man.address = toex.rs1 + P.fint(toex.imm);
                man.bits    = 16;
                man.rd      = toex.rd;
                man.sign    = true;
                man.content = toex.rs2;
                core.MEM.enqueue(man);
                break;
            case 0b010: //LW
                man.rdproc  = false;
                man.read    = true;
                man.address = toex.rs1 + P.fint(toex.imm);
                man.bits    = 32;
                man.rd      = toex.rd;
                man.sign    = true;
                man.content = toex.rs2;
                core.MEM.enqueue(man);
                break;
            case 0b100: //LBU
                man.rdproc  = false;
                man.read    = true;
                man.address = toex.rs1 + P.fint(toex.imm);
                man.bits    = 8;
                man.rd      = toex.rd;
                man.sign    = false;
                man.content = toex.rs2;
                core.MEM.enqueue(man);
                break;
            case 0b101: //LHU
                man.rdproc  = false;
                man.read    = true;
                man.address = toex.rs1 + P.fint(toex.imm);
                man.bits    = 16;
                man.rd      = toex.rd;
                man.sign    = false;
                man.content = toex.rs2;
                core.MEM.enqueue(man);
                break;
            }
            break;
        case instr::I:
            switch (toex.funct3) {
            case 0b000: //ADDI
                man.rdproc = false;
                man.rdwr   = mempair{toex.rd, P.ftaddr(P.fint(toex.rs1) + P.fint(toex.imm))};
                core.MEM.enqueue(man);
                break;
            case 0b010: //SLTI
                P.padimm(toex.imm, 12);
                man.rdproc = false;
                man.rdwr   = mempair{toex.rd, (P.fint(toex.rs1) < P.fint(toex.imm))};
                core.MEM.enqueue(man);
                break;
            case 0b011: //SLTIU
                P.padimm(toex.imm, 12);
                man.rdproc = false;
                man.rdwr   = mempair{toex.rd, (toex.rs1 < toex.imm)};
                core.MEM.enqueue(man);
                break;
            case 0b100: //XORI
                P.padimm(toex.imm, 12);
                man.rdproc = false;
                man.rdwr   = mempair{toex.rd, (toex.rs1 ^ toex.imm)};
                core.MEM.enqueue(man);
                break;
            case 0b110: //ORI
                P.padimm(toex.imm, 12);
                man.rdproc = false;
                man.rdwr   = mempair{toex.rd, (toex.rs1 | toex.imm)};
                core.MEM.enqueue(man);
                break;
            case 0b111: //ANDI
                P.padimm(toex.imm, 12);
                man.rdproc = false;
                man.rdwr   = mempair{toex.rd, (toex.rs1 & toex.imm)};
                core.MEM.enqueue(man);
                break;
            case 0b001: //SLLI
                P.padimm(toex.imm, 12);
                man.rdproc = false;
                man.rdwr   = mempair{toex.rd, (toex.rs1 << toex.imm)};
                core.MEM.enqueue(man);
                break;
            case 0b101:
                man.rdproc = false;
                if (toex.funct7 == 0) //SRLI
                    man.rdwr = mempair{toex.rd, (toex.rs1 >> toex.imm)};
                else //SRAI
                    man.rdwr = mempair{toex.rd, P.ftaddr(P.fint(toex.rs1) >> toex.imm)};
                core.MEM.enqueue(man);
                break;
            }
            break;
        case instr::Ij: //JALR
            core.jumpstall();
            core.pcmod(toex.addr + P.fint(toex.imm));
            man.rdproc = true;
            man.rdwr   = mempair{toex.rd, toex.addr + 4};
            core.MEM.enqueue(man);
            break;
        case instr::R:
            switch (toex.funct3) {
            case 0b000:
                if (toex.funct7 == 0) //ADD
                {
                    man.rdproc = false;
                    man.rdwr   = mempair{toex.rd, P.ftaddr(P.fint(toex.rs1) + P.fint(toex.rs2))};
                    core.MEM.enqueue(man);
                } else //SUB{
                    man.rdproc = false;
                man.rdwr = mempair{toex.rd, P.ftaddr(P.fint(toex.rs1) - P.fint(toex.rs2))};
                core.MEM.enqueue(man);
                break;
            case 0b001: //SLL
                man.rdproc = false;
                man.rdwr   = mempair{toex.rd, toex.rs1 << (toex.rs2 & 0b11111)};
                core.MEM.enqueue(man);
                break;
            case 0b010: //SLT
                man.rdproc = false;
                man.rdwr   = mempair{toex.rd, (P.fint(toex.rs1) < P.fint(toex.rs2))};
                core.MEM.enqueue(man);
                break;
            case 0b011: //SLTU
                man.rdproc = false;
                man.rdwr   = mempair{toex.rd, (toex.rs1 < toex.rs2)};
                core.MEM.enqueue(man);
                break;
            case 0b100: //XOR
                man.rdproc = false;
                man.rdwr   = mempair{toex.rd, (toex.rs1 ^ toex.rs2)};
                core.MEM.enqueue(man);
                break;
            case 0b101:
                man.rdproc = false;
                if (toex.funct7 == 0) //SRL
                    man.rdwr = mempair{toex.rd, (toex.rs1 >> (toex.rs2 & 0b11111))};
                else //SRA
                    man.rdwr = mempair{toex.rd, P.ftaddr(P.fint(toex.rs1) >> (toex.rs2 & 0b11111))};
                core.MEM.enqueue(man);
                break;
            case 0b110: //OR
                man.rdproc = false;
                man.rdwr   = mempair{toex.rd, toex.rs1 | toex.rs2};
                core.MEM.enqueue(man);
                break;
            case 0b111: //AND
                man.rdproc = false;
                man.rdwr   = mempair{toex.rd, toex.rs1 & toex.rs2};
                core.MEM.enqueue(man);
                break;
            }
            break;
        }
    }
}

EX::EX(core_session& c) : stage(c) {}

int ID::enqueue(mempair m) {
    decodelst.push(m);
    return 0;
}
int ID::tick() {
    if (stall)
        return 0;
    if (!decodelst.empty()) {
        getdecode = decodelst.front();
        command C = P.Splitter(getdecode.instruction, getdecode.address);
        if (!core.query(C)) {
            core.occupy(C);
            excute ex = C;
            ex.rs1    = core.reg[C.rs1];
            ex.rs2    = core.reg[C.rs2];
            core.EX.enqueue(ex);
            decodelst.pop();
        } else {
            core.datastall();
        }
    }
}
ID::ID(core_session& c) : stage(c){};

int IF::tick() {
    if (stall)
        return 0;
    mempair m;
    m.address     = core.npc;
    m.instruction = core.memory.get(core.npc);
    core.ID.enqueue(m);
    return 0;
}
IF::IF(core_session& c) : stage(c){};