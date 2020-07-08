# RISC-V 指令集架构介绍 - RV32I

> This content comes from [Jim’s Dev Blog](https://tclin914.github.io/16df19b4/), and the original is written in traditional Chinese.

RV32I为 32-bit基本整数指令集，有 32个 32-bit暂存器(x0-x31)，总共有 47道指令，以下介绍各个指令的用途与格式。

### 整数运算指令 (Integer Computational Instructions)

#### 整数暂存器与常数指令 (Integer Register-Immediate Instructions)

指令为暂存器与常数之间的运算

* **ADDI**
  *addi rd, rs1, simm12*
  常数部分为 sign-extended 12-bit，会将 12-bit做 sign-extension成 32-bit后，再与 rs1暂存器做加法运算，将结果写入 rd暂存器，*addi rd, rs1, 0* 可被使用来当做 mov指令。

* **SLTI**
  *slti rd, rs1, simm12*
  常数部分为 sign-extended 12-bit，会将 12-bit做 sign-extension成 32-bit后，再与 rs1暂存器当做 signed number做比较，若 rs暂存器1小于常数，则将数值 1写入 rd暂存器，反之则写入数值 0。

* **SLTIU**
  *sltiu rd, rs1, simm12*
  常数部分为 sign-extended 12-bit，会将 12-bit做 sign-extension成 32-bit后，再与 rs1暂存器当作 unsigned number做比较·若 rs1暂存器小于常数，则将数值 1写入 rd暂存器，反之则写入数值 0。

* **ANDI/ORI/XORI**
  *andi/ori/xori rd, rs1, simm12*
  常数部分为 sign-extended 12-bit，会将 12-bit做 sign-extension成 32-bit后，再与 rs1暂存器做 AND/OR/XOR运算，将结果写入 rd暂存器。

* **SLLI/SRLI/SRAI**
  *slli/srli/srai rd, rs1, uimm5*
  常数部分为 unsigned 5-bit，范围为 0~31，为 shift amount，将 rs1暂存器做 shift运算，结果写入 rd暂存器，SLLI为 logical左移，会补 0到最低位元，SRLI为 logical右移，会补 0到最高位元，SRAI为 arithmetic右移，会将原本的 sign bit複製到最高位元。

* **LUI (Load upper immediate)**
  *lui rd, uimm20*
  将 unsigned 20-bit放到 rd暂存器的最高 20-bit，并将剩馀的 12-bit补 0，此指令可与 ADDI搭配，一起组合出完整 32-bit的数值。

* **AUIPC(add upper immediate to pc)**
  *auipc rd, uimm20*
  unsigned 20-bit放到最高 20位元，剩馀 12位元补0，将此数值与 pc相加写入 rd暂存器。

#### 整数暂存器与暂存器指令 (Integer Register-Register Insructions)

指令为暂存器与暂存器之间的运算

* **ADD/SUB**
  *add/sub rd, rs1, rs2*
  将 rs1暂存器与 rs2暂存器做加法/减法运算，将结果写入 rd暂存器。

* **SLT/SLTU**
  *slt/sltu rd, rs1, rs2*
  将 rs1暂存器与 rs2暂存器当做 singed/unsigned number做比较，若 rs1暂存器小于 rs2暂存器，则将数值 1写入 rd暂存器，反之则写入数值 0。

* **AND/OR/XOR**
  *and/or/xor rd, rs1, rs2*
  将 rs1暂存器与 rs2暂存器做 AND/OR/XOR运算，将结果写入 rd暂存器。

* **SLL/SRL/SRA**
  *sll/srl/sra rd, rs1,, rs2*
  将 rs1暂存器做 shift运算，结果写入 rd暂存器，rs2暂存器的最低 5-bit为 shift amount。

#### NOP 指令

NOP 指令即为不改变任何暂存器状态，除了 pc以外。NOP 指令会被编码成 *addi x0, x0, 0* 替代。

### 控制转移指令 (Control Transfer Instructions)

分别有两种控制转移指令，无条件跳跃(Unconditional jumps)与条件分支(Conditional branches)

#### 无条件跳跃 (Unconditional Jumps)

* **JAL (jump and link)**
  *jal rd, simm21*
  常数部分为 sign-extended 21-bit，要注意的是此常数必须为 2的倍数，即最低位元为 0，因为此道指令编码的常数位元数只有 20位元，所以只会将 signed 21-bit的最高 20位元放入指令编码中，跳跃范围为 -+1MiB，同时也会将下一道指令的位址 pc+4写入 rd暂存器中，在标准的 calling convention中，rd暂存器会使用 x1。如果只是单纯的 jump，并非是呼叫函示需要储存其返回位址 pc+4，可用 *jal x0, simm21* 取代。

* **JALR (jump and link register)**
  *jalr rd, rs1, simm12*
  常数部分为 sign-extended 12-bit，跳跃的位址为 rs暂存器加上 sign-extended 12-bit，并把下一道指令的位址 pc+4写入 rd暂存器中。

#### 条件跳跃 (Conditional Branches)

* **BEQ/BNE/BLT/BLTU/BGE/BGEU**
  *beq/bne/blt/bltu/bge/bgeu rs1, rs2, simm13*
  常数部分为 sign-extended 13-bit，要注意的是此常数必须为 2的倍数，即最低位元为 0，因为此道指令编码的常数位元数只有 12位元，所以只会将 signed 13-bit的最高 12位元放入指令编码中，跳跃范围为 -+4Kib，BEQ/BNE将 rs1暂存器与 rs2暂存器做相同与不同的比较，若成立则跳跃，BLT/BLTU将 rs1暂存器与 rs2暂存器分别做 signed/unsigned小于比较，若成立则跳跃，BGE/BGEU将 rs1暂存器与 rs2暂存器分别做 signed/unsigned大于等于比较，若成立则跳跃，跳跃的位址则为 pc加上 sign-extended 13-bit。

### 载入与储存指令 (Load and Store Instructions)

RV32I 必须使用载入与储存指令去存取记忆体，前面的运算指令只能够对暂存器做操作。

* **LW/LH/LHU/LB/LBU**
  *lw/lh/lhu/lb/lbu rd, rs1, simm12*
  常数部分为 sign-extended 12-bit，载入位址则为 rs1暂存器加上 sign-extended 12-bit，LW为载入 32-bit资料写入 rd暂存器，LH/LHU为载入 16-bit资料分别做 unsigned/signed extension成 32-bit后写入 rd暂存器，LB/LBU为载入 8-bit资料分别做 unsigned/signed extension成 32-bit后写入 rd暂存器。

* **SW/SH/SB**
  *sw/sh/sb rs2, rs1, simm12*
  常数部分为 sign-extended 12-bit，储存位址则为 rs1暂存器加上 sign-extended 12-bit，SW为将 rs2暂存器完整 32-bit资料写入记忆体，SH为将 rs2暂存器最低 16-bit资料写入记忆体，SB为将 rs2暂存器最低 8-bit资料写入记忆体。

### Memory model

定义了一组 FENCE指令，用来做不同 thread之间，记忆体的同步。

### 控制与状态暂存器指令 (Control and Status Register Instructions)

#### CSR Instructions

* **CSRRW/CSRRS/CSRRC/CSRRWI/CSRRSI/CSRRCI**
  定义了一组 CSR指令，可用来读取写入 CSR。

#### Timers and Counters

* **RDCYCLE[H]**
  rdcycle用来读取最低 31-bit cycle CSR，rdcycleh用来读取最高 31-bit cycle数。

* **RDTIME[H]**
  用来读取 time CSR。

* **RDINSTRET**
  用来读取 instret CSR。

### Environment Call and Breakpoints

* **ECALL**
  使用来呼叫 system call。

* **EBREAK**
  Debugger 用来切换进 Debugging 环境。

#### Reference

[1] [The RISC-V Instruction Set Manual](https://riscv.org/specifications/)