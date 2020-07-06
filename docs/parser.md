# Parser

The parser, including `parser.cpp` and `parser.hpp`, convert hexadecimal instructions to processable orders.

## Basic Knowledge to RISC-V instructions

### Registers 

Cf. [riscv-asm.md](riscv-asm.md)

### Basic Instruction Formats

Here are the instruction on the RISC-V manual.

![image-20200706155102937](image-20200706155102937.png)

Except for the 5-bit immediates used in CSR instructions, immediates are always sign-extended, and are generally packed towards the leftmost available bits in the instruction and have been allocated to reduce hardware complexity. In particular, the sign bit for all immediates is always in bit 31 of the instruction to speed sign-extension circuitry.

立即数的符号拓展

### Immediate Encoding Variants

![image-20200706155537090](image-20200706155537090.png)

B-type instructions have even immediate number, making so imm[0]is omitted.

![image-20200706155551485](image-20200706155551485.png)

The location of instruction bits in the U and J format immediates is chosen to maximize overlap with the other formats and with each other.

### Integer Computational Instructions