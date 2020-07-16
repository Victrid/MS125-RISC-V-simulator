#include "core_tomasulo.hpp"

using namespace std;

taddr& rentab::operator[](const shiftreg& branch) {
    return tab[branch.get()];
}

void resstation::CDBcall(taddr regname, taddr value) {
    if (ready)
        return;
    if (waiting_rs1 && regname == exc.rs1) {
        exc.rs1     = value;
        waiting_rs1 = false;
    }
    if (waiting_rs2 && regname == exc.rs2) {
        exc.rs2     = value;
        waiting_rs2 = false;
    }
    ready = (waiting_rs1 || waiting_rs2);
}

resstation::resstation(core_session* c) : core(c){};

void resstation::load(excute command, shiftreg bs) {
    branchselect = bs;
    switch (command.instruction) {
    case instr::T:
    case instr::J:
    case instr::U:
    case instr::Ua:
        waiting_rs1 = waiting_rs2 = false;
        break;
    case instr::B:
    case instr::R:
    case instr::S:
        if (core->rt[command.rs2][bs] == command.rs2) {
            command.rs2 = core->reg[command.rs2];
            waiting_rs2 = false;
        } else {
            command.rs2 = core->rt[command.rs2][bs];
            waiting_rs2 = true;
        }
    case instr::Il:
    case instr::I:
    case instr::Ij:
        if (core->rt[command.rs1][bs] == command.rs1) {
            command.rs1 = core->reg[command.rs1];
            waiting_rs1 = false;
        } else {
            command.rs1 = core->rt[command.rs1][bs];
            waiting_rs1 = true;
        }
        break;
    }
    core->rt[command.rd][bs] = number & 0x80000000;
    exc                      = command;
}

void resstation::issue(int alu) {
    core->A[alu].load(exc, number, branchselect);
    busy = true;
}

void resstation::release() {
    busy  = false;
    empty = true;
    ready = false;
}

int ALU::tick() {
    if (!empty) {
        switch (Action.instruction) {
        case instr::T:
            core->term();
        case instr::B:
            //     switch (Action.funct3) {
            //     case 0b000: //BEQ
            //         if (Action.rs1 != Action.rs2)
            //             return 0;
            //         break;
            //     case 0b001: //BNE
            //         if (Action.rs1 == Action.rs2)
            //             return 0;
            //         break;
            //     case 0b100: //BLT
            //         if (P.fint(Action.rs1) >= P.fint(Action.rs2))
            //             return 0;
            //         break;
            //     case 0b101: //BGE
            //         if (P.fint(Action.rs1) < P.fint(Action.rs2))
            //             return 0;
            //         break;
            //     case 0b110: //BLTU
            //         if (Action.rs1 >= Action.rs2)
            //             return 0;
            //         break;
            //     case 0b111: //BGEU
            //         if (Action.rs1 < Action.rs2)
            //             return 0;
            //         break;
            //     }
            //     core->jumpstall();
            //     core->pcmod(Action.addr + P.fint(Action.imm));
            break;
        case instr::J: //JAL
                       //     core->jumpstall();
                       //     core->pcmod(Action.addr + P.fint(Action.imm));
                       //     man.rdproc = true;
                       //     man.rdwr   = mempair{Action.rd, Action.addr + 4};
                       //     core->cMEM.enqueue(man);
            break;
        case instr::U: //LUI
            core->bus.publish(Action.rd, Action.imm, branchselect);
            break;
        case instr::Ua: //AUIPC
            core->bus.publish(Action.rd, Action.imm + Action.addr, branchselect);
            break;
        case instr::S:
            // man.rdproc  = false;
            // man.read    = false;
            // man.address = Action.rs1 + P.fint(Action.imm);
            // man.content = Action.rs2;
            // man.bits    = Loadbits[Action.funct3];
            // core->memqueue.enqueue(man);
            break;
        case instr::Il:
            // P.padimm(Action.imm, 12);
            // man.rdproc  = false;
            // man.read    = true;
            // man.address = Action.rs1 + P.fint(Action.imm);
            // man.rd      = Action.rd;
            // man.bits    = Loadbits[Action.funct3];
            // man.sign    = Loadsign[Action.funct3];
            // core->memqueue.enqueue(man);
            break;
        case instr::I: {
            P.padimm(Action.imm, 12);
            taddr tas;
            switch (Action.funct3) {
            case 0b000: //ADDI
                tas = P.ftaddr(P.fint(Action.rs1) + P.fint(Action.imm));
                break;
            case 0b010: //SLTI
                tas = (P.fint(Action.rs1) < P.fint(Action.imm));
                break;
            case 0b011: //SLTIU
                tas = (Action.rs1 < Action.imm);
                break;
            case 0b100: //XORI
                tas = (Action.rs1 ^ Action.imm);
                break;
            case 0b110: //ORI
                tas = (Action.rs1 | Action.imm);
                break;
            case 0b111: //ANDI
                tas = (Action.rs1 & Action.imm);
                break;
            case 0b001: //SLLI
                tas = (Action.rs1 << Action.imm);
                break;
            case 0b101:
                if (Action.funct7 == 0) //SRLI
                    tas = (Action.rs1 >> Action.imm);
                else //SRAI
                    tas = P.ftaddr(P.fint(Action.rs1) >> Action.imm);
                break;
            }
            core->bus.publish(Action.rd, tas, branchselect);
        } break;
        case instr::Ij: //JALR
            // core->jumpstall();
            // core->pcmod(Action.rs1 + P.fint(Action.imm));
            // man.rdproc = true;
            // man.rdwr   = mempair{Action.rd, Action.addr + 4};
            // core->cMEM.enqueue(man);
            break;
        case instr::R:
            taddr tas;
            switch (Action.funct3) {
            case 0b000:
                if (Action.funct7 == 0) //ADD
                    tas = P.ftaddr(P.fint(Action.rs1) + P.fint(Action.rs2));
                else //SUB
                    tas = P.ftaddr(P.fint(Action.rs1) - P.fint(Action.rs2));
                break;
            case 0b001: //SLL
                tas = Action.rs1 << (Action.rs2 & 0b11111);
                break;
            case 0b010: //SLT
                tas = (P.fint(Action.rs1) < P.fint(Action.rs2));
                break;
            case 0b011: //SLTU
                tas = (Action.rs1 < Action.rs2);
                break;
            case 0b100: //XOR
                tas = (Action.rs1 ^ Action.rs2);
                break;
            case 0b101:
                if (Action.funct7 == 0) //SRL
                    tas = (Action.rs1 >> (Action.rs2 & 0b11111));
                else //SRA
                    tas = P.ftaddr(P.fint(Action.rs1) >> (Action.rs2 & 0b11111));
                break;
            case 0b110: //OR
                tas = Action.rs1 | Action.rs2;
                break;
            case 0b111: //AND
                tas = Action.rs1 & Action.rs2;
                break;
            }
            core->bus.publish(Action.rd, tas, branchselect);
            break;
        }
    }
    return 0;
}