# RISC-V simulator

This is the RISC-V Simulator implementation homework of MS125: Principle and Practice of Computer Algorithms.

This version is used for [BASIC RISC-V compiler](https://github.com/Victrid/BASIC-RISC-V-compiler). The complete version is located in the `master` branch.

It is based on the sequential version. Additional features are listed below.

## Additional feature specific to the compiler project

- Added **RV32M Standard Extension** for extra multiplication and division instructions.

- **Input** instruction:

  - **Assembly**: `input`

  - **machine code**: `0xFF00FF00`

  - **actions**: read an integer from input and store it into `a0`(`x10`) register. 

    (For unexpected behavior like inputing an overflowing integer or some chars, this action is implemented by `cin` and should behave like that.)

- **Return values** are widened to 32-digit unsigned integer.

## Build and run

To build binary executables, run `make ` or `make code`.

In this version, `code` can have arguments like this:

|            command             | program code | input contents |
| :----------------------------: | :----------: | :------------: |
|            `./code`            |   `stdin`    |    `stdin`     |
|      `./code some.riscv`       | `some.riscv` |    `stdin`     |
| `./code some.riscv some.input` | `some.riscv` |  `some.input`  |

**Note:** when both program code and input contents are from `stdin`, program code and input should be separated by `00 00 00 00`. Actually, in this simulator series  `00 00 00 00` code is used to indicate the end of memory loading, if not reaching the end of file.

Cf. `master` branch’s page for detailed information.