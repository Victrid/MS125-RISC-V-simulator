# RISC-V-simulator

This is the RISC-V Simulator implementation homework of MS125: Principle and Practice of Computer Algorithms.

Requirements: [PPCA_2020: RISC-V simulator](https://acm.sjtu.edu.cn/wiki/PPCA_2020#RISCV.E5.A4.A7.E4.BD.9C.E4.B8.9A). Please refer to [documentation](docs) for details.

[Other documentations](aux-docs) are files for reference, the repository owner claims no right to these files. Use them at your own risk.

To build binary executables, run commands:

| Implementation           | commands            |
| ------------------------ | ------------------- |
| Sequential               | `make code_seq`     |
| Pipeline                 | `make code_pip`     |
| Pipeline with prediction | `make` or `make code` |

and execute with `./code`. 

Testcases are located in [dataset](dataset). The plain files comes from Repo. [riscv-testcases](https://github.com/sjtuliuck/riscv-testcases) and files in subdirectories are created under this project.

See detailed description in [structures.md](docs/structures.md).

