#include "core_pipeline.hpp"
core_session::core_session(const char* ch) : cWB(this), cIF(this), cID(this), cEX(this), cMEM(this) {
    memset(reg, 0, 32 * sizeof(taddr));
    memset(regoccupy, 0, 32 * sizeof(bool));
    memory.memload(ch);
    jumpstallflag = false;
    datastallflag = false;
    termflag      = false;
    npc           = 0;
};

core_session::core_session() : cWB(this), cIF(this), cID(this), cEX(this), cMEM(this) {
    memset(reg, 0, 32 * sizeof(taddr));
    memset(regoccupy, 0, 32 * sizeof(bool));
    memory.memload();
    jumpstallflag = false;
    datastallflag = false;
    termflag      = false;
    npc           = 0;
};

bool core_session::query(command toex) {
    //Hit and return true.
    switch (toex.instruction) {
    case instr::T:
    case instr::J:
    case instr::U:
    case instr::Ua:
        return false;
    case instr::B:
    case instr::R:
    case instr::S:
        return (toex.rs1 && regoccupy[toex.rs1]) || (toex.rs2 && regoccupy[toex.rs2]);
        break;
    case instr::Il:
    case instr::I:
    case instr::Ij:
        return toex.rs1 && regoccupy[toex.rs1];
        break;
    }
    return false;
}

void core_session::occupy(command toex) {
    //Hit and return true.
    switch (toex.instruction) {
    case instr::T:
    case instr::B:
    case instr::S:
        return;
    case instr::R:
    case instr::U:
    case instr::Ua:
    case instr::Il:
    case instr::I:
    case instr::Ij:
    case instr::J: //JAL
        if (toex.rd)
            regoccupy[toex.rd] = true;
        return;
    }
}

void core_session::release(taddr t) {
    if (t)
        regoccupy[t] = false;
    return;
}

stage::stage(core_session* c) : core(c), stall(false){};

int WB::enqueue(mempair m) {
    ActionQueue.push(m);
    return 0;
}

int WB::tick() {
    if (stall)
        return 0;
    if (!ActionQueue.empty()) {
        Action = ActionQueue.front();
        ActionQueue.pop();
        if (Action.address) { //To avoid 0 write
            core->reg[Action.address] = Action.instruction;
            core->release(Action.address);
        }
    }
    return 0;
}

WB::WB(core_session* c) : stage(c){};

int MEM::enqueue(memmanip m) {
    ActionQueue.push(m);
    return 0;
}

int MEM::tick() {
    if (stall)
        return 0;
    if (!ActionQueue.empty()) {
        Action = ActionQueue.front();
        ActionQueue.pop();
        if (Action.rdproc) {
            core->cWB.enqueue(Action.rdwr);
            return 0;
        }
        if (!Action.read) {
            taddr mask = P.rearrange(core->memory.get(Action.address));
            switch (Action.bits) {
            case 8:
                mask           = mask & 0xFFFFFF00;
                Action.content = Action.content & 0x000000FF;
                break;
            case 16:
                mask           = mask & 0xFFFF0000;
                Action.content = Action.content & 0x0000FFFF;
                break;
            case 32:
                mask           = mask & 0x00000000;
                Action.content = Action.content & 0xFFFFFFFF;
                break;
            }
            core->memory.get(Action.address) = P.rearrange(mask | Action.content);
        } else {
            taddr mask = P.rearrange(core->memory.get(Action.address));
            switch (Action.bits) {
            case 8:
                mask = P.getdigits(mask, -1, 7);
                if (Action.sign)
                    P.padimm(mask, 8);
                break;
            case 16:
                mask = P.getdigits(mask, -1, 15);
                if (Action.sign)
                    P.padimm(mask, 16);
                break;
            case 32:
                break;
            }
            core->cWB.enqueue(mempair{Action.rd, mask});
        }
    }
    return 0;
}

MEM::MEM(core_session* c) : stage(c) {}

int EX::enqueue(excute m) {
    ActionQueue.push(m);
    return 0;
}

int EX::tick() {
    if (!ActionQueue.empty()) {
        Action = ActionQueue.front();
        ActionQueue.pop();
        memmanip man;
        switch (Action.instruction) {
        case instr::T:
            return core->term();
        case instr::B:
            switch (Action.funct3) {
            case 0b000: //BEQ
                if (Action.rs1 == Action.rs2) {
                    core->jumpstall();
                    core->pcmod(Action.addr + P.fint(Action.imm));
                }
                break;
            case 0b001: //BNE
                if (Action.rs1 != Action.rs2) {
                    core->jumpstall();
                    core->pcmod(Action.addr + P.fint(Action.imm));
                }
                break;
            case 0b100: //BLT
                if (P.fint(Action.rs1) < P.fint(Action.rs2)) {
                    core->jumpstall();
                    core->pcmod(Action.addr + P.fint(Action.imm));
                }
                break;
            case 0b101: //BGE
                if (P.fint(Action.rs1) >= P.fint(Action.rs2)) {
                    core->jumpstall();
                    core->pcmod(Action.addr + P.fint(Action.imm));
                }
                break;
            case 0b110: //BLTU
                if (Action.rs1 < Action.rs2) {
                    core->jumpstall();
                    core->pcmod(Action.addr + P.fint(Action.imm));
                }
                break;
            case 0b111: //BGEU
                if (Action.rs1 >= Action.rs2) {
                    core->jumpstall();
                    core->pcmod(Action.addr + P.fint(Action.imm));
                }
                break;
            }
            break;
        case instr::J: //JAL
            core->jumpstall();
            core->pcmod(Action.addr + P.fint(Action.imm));
            man.rdproc = true;
            man.rdwr   = mempair{Action.rd, Action.addr + 4};
            core->cMEM.enqueue(man);
            break;
        case instr::U: //LUI
            man.rdproc = true;
            man.rdwr   = mempair{Action.rd, Action.imm};
            core->cMEM.enqueue(man);
            break;
        case instr::Ua: //AUIPC
            man.rdproc = true;
            man.rdwr   = mempair{Action.rd, Action.imm + Action.addr};
            core->cMEM.enqueue(man);
            break;
        case instr::S:
            switch (Action.funct3) {
            case 0b000: //SB
                man.rdproc  = false;
                man.read    = false;
                man.address = Action.rs1 + P.fint(Action.imm);
                man.bits    = 8;
                man.content = Action.rs2;
                core->cMEM.enqueue(man);
                break;
            case 0b001: //SH
                man.rdproc  = false;
                man.read    = false;
                man.address = Action.rs1 + P.fint(Action.imm);
                man.bits    = 16;
                man.content = Action.rs2;
                core->cMEM.enqueue(man);
                break;
            case 0b010: //SW
                man.rdproc  = false;
                man.read    = false;
                man.address = Action.rs1 + P.fint(Action.imm);
                man.bits    = 32;
                man.content = Action.rs2;
                core->cMEM.enqueue(man);
                break;
            }
            break;
        case instr::Il:
            P.padimm(Action.imm, 12);
            switch (Action.funct3) { //LB
            case 0b000:
                man.rdproc  = false;
                man.read    = true;
                man.address = Action.rs1 + P.fint(Action.imm);
                man.bits    = 8;
                man.rd      = Action.rd;
                man.sign    = true;
                core->cMEM.enqueue(man);
                break;
            case 0b001: //LH
                man.rdproc  = false;
                man.read    = true;
                man.address = Action.rs1 + P.fint(Action.imm);
                man.bits    = 16;
                man.rd      = Action.rd;
                man.sign    = true;
                core->cMEM.enqueue(man);
                break;
            case 0b010: //LW
                man.rdproc  = false;
                man.read    = true;
                man.address = Action.rs1 + P.fint(Action.imm);
                man.bits    = 32;
                man.rd      = Action.rd;
                man.sign    = true;
                core->cMEM.enqueue(man);
                break;
            case 0b100: //LBU
                man.rdproc  = false;
                man.read    = true;
                man.address = Action.rs1 + P.fint(Action.imm);
                man.bits    = 8;
                man.rd      = Action.rd;
                man.sign    = false;
                core->cMEM.enqueue(man);
                break;
            case 0b101: //LHU
                man.rdproc  = false;
                man.read    = true;
                man.address = Action.rs1 + P.fint(Action.imm);
                man.bits    = 16;
                man.rd      = Action.rd;
                man.sign    = false;
                core->cMEM.enqueue(man);
                break;
            }
            break;
        case instr::I:
            switch (Action.funct3) {
            case 0b000: //ADDI
                man.rdproc = true;
                man.rdwr   = mempair{Action.rd, P.ftaddr(P.fint(Action.rs1) + P.fint(Action.imm))};
                core->cMEM.enqueue(man);
                break;
            case 0b010: //SLTI
                P.padimm(Action.imm, 12);
                man.rdproc = true;
                man.rdwr   = mempair{Action.rd, (P.fint(Action.rs1) < P.fint(Action.imm))};
                core->cMEM.enqueue(man);
                break;
            case 0b011: //SLTIU
                P.padimm(Action.imm, 12);
                man.rdproc = true;
                man.rdwr   = mempair{Action.rd, (Action.rs1 < Action.imm)};
                core->cMEM.enqueue(man);
                break;
            case 0b100: //XORI
                P.padimm(Action.imm, 12);
                man.rdproc = true;
                man.rdwr   = mempair{Action.rd, (Action.rs1 ^ Action.imm)};
                core->cMEM.enqueue(man);
                break;
            case 0b110: //ORI
                P.padimm(Action.imm, 12);
                man.rdproc = true;
                man.rdwr   = mempair{Action.rd, (Action.rs1 | Action.imm)};
                core->cMEM.enqueue(man);
                break;
            case 0b111: //ANDI
                P.padimm(Action.imm, 12);
                man.rdproc = true;
                man.rdwr   = mempair{Action.rd, (Action.rs1 & Action.imm)};
                core->cMEM.enqueue(man);
                break;
            case 0b001: //SLLI
                P.padimm(Action.imm, 12);
                man.rdproc = true;
                man.rdwr   = mempair{Action.rd, (Action.rs1 << Action.imm)};
                core->cMEM.enqueue(man);
                break;
            case 0b101:
                man.rdproc = true;
                if (Action.funct7 == 0) //SRLI
                    man.rdwr = mempair{Action.rd, (Action.rs1 >> Action.imm)};
                else //SRAI
                    man.rdwr = mempair{Action.rd, P.ftaddr(P.fint(Action.rs1) >> Action.imm)};
                core->cMEM.enqueue(man);
                break;
            }
            break;
        case instr::Ij: //JALR
            core->jumpstall();
            core->pcmod(Action.addr + P.fint(Action.imm));
            man.rdproc = true;
            man.rdwr   = mempair{Action.rd, Action.addr + 4};
            core->cMEM.enqueue(man);
            break;
        case instr::R:
            switch (Action.funct3) {
            case 0b000:
                if (Action.funct7 == 0) //ADD
                {
                    man.rdproc = true;
                    man.rdwr   = mempair{Action.rd, P.ftaddr(P.fint(Action.rs1) + P.fint(Action.rs2))};
                    core->cMEM.enqueue(man);
                } else //SUB{
                    man.rdproc = true;
                man.rdwr = mempair{Action.rd, P.ftaddr(P.fint(Action.rs1) - P.fint(Action.rs2))};
                core->cMEM.enqueue(man);
                break;
            case 0b001: //SLL
                man.rdproc = true;
                man.rdwr   = mempair{Action.rd, Action.rs1 << (Action.rs2 & 0b11111)};
                core->cMEM.enqueue(man);
                break;
            case 0b010: //SLT
                man.rdproc = true;
                man.rdwr   = mempair{Action.rd, (P.fint(Action.rs1) < P.fint(Action.rs2))};
                core->cMEM.enqueue(man);
                break;
            case 0b011: //SLTU
                man.rdproc = true;
                man.rdwr   = mempair{Action.rd, (Action.rs1 < Action.rs2)};
                core->cMEM.enqueue(man);
                break;
            case 0b100: //XOR
                man.rdproc = true;
                man.rdwr   = mempair{Action.rd, (Action.rs1 ^ Action.rs2)};
                core->cMEM.enqueue(man);
                break;
            case 0b101:
                man.rdproc = true;
                if (Action.funct7 == 0) //SRL
                    man.rdwr = mempair{Action.rd, (Action.rs1 >> (Action.rs2 & 0b11111))};
                else //SRA
                    man.rdwr = mempair{Action.rd, P.ftaddr(P.fint(Action.rs1) >> (Action.rs2 & 0b11111))};
                core->cMEM.enqueue(man);
                break;
            case 0b110: //OR
                man.rdproc = true;
                man.rdwr   = mempair{Action.rd, Action.rs1 | Action.rs2};
                core->cMEM.enqueue(man);
                break;
            case 0b111: //AND
                man.rdproc = true;
                man.rdwr   = mempair{Action.rd, Action.rs1 & Action.rs2};
                core->cMEM.enqueue(man);
                break;
            }
            break;
        }
    }
    return 0;
}

EX::EX(core_session* c) : stage(c) {}

int ID::enqueue(mempair m) {
    ActionQueue.push(m);
    return 0;
}

int ID::tick() {
    if (stall)
        return 0;
    if (!ActionQueue.empty()) {
        Action    = ActionQueue.front();
        command C = P.Splitter(Action.instruction, Action.address);
        if (!core->query(C)) {
            core->datastallflag = false;
            core->occupy(C);
            excute ex = C;
            ex.rs1    = core->reg[C.rs1];
            ex.rs2    = core->reg[C.rs2];
            core->cEX.enqueue(ex);
            ActionQueue.pop();
        } else {
            core->datastall();
        }
    }
    return 0;
}

ID::ID(core_session* c) : stage(c){};

int IF::tick() {
    if (stall)
        return 0;
    mempair m;
    m.address     = core->npc;
    m.instruction = core->memory.get(core->npc);
    core->cID.enqueue(m);
    core->npc = m.address + 4;
    return 0;
}

IF::IF(core_session* c) : stage(c){};

void core_session::jumpstall() {
    jumpstallflag = true;
    cIF.stall     = true;
    cID.stall     = true;
    return;
};

int core_session::term() {
    termflag = true;
    return reg[11] & 255u;
}

void core_session::pcmod(taddr pc) {
    npc = pc;
    return;
}

void core_session::datastall() {
    datastallflag = true;
    cIF.stall     = true;
    return;
}

int core_session::tick() {
    int ip = 0;
    cWB.tick();
    cMEM.tick();
    ip = cEX.tick();
    if (termflag)
        return ip;
    if (jumpstallflag) {
        if (cEX.empty() && cMEM.empty() && cWB.empty()) {
            jumpstallflag = false;
            while (!cID.ActionQueue.empty())
                cID.ActionQueue.pop();
            cID.stall = false;
        } else {
            cID.stall = true;
        }
    }
    cID.tick();
    if (datastallflag || jumpstallflag) {
        cIF.stall = true;
    } else {
        cIF.stall = false;
    }
    cIF.tick();
    return 0;
}

int core_session::run() {
    int ip = 0;
    while (true) {
        ip = tick();
        if (termflag)
            return ip;
    }
}

bool IF::empty() {
    return false;
}

bool ID::empty() {
    return ActionQueue.empty();
}

bool EX::empty() {
    return ActionQueue.empty();
}

bool MEM::empty() {
    return ActionQueue.empty();
}

bool WB::empty() {
    return ActionQueue.empty();
}