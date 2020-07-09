riscv32-unknown-elf-as $1 -o t.o
riscv32-unknown-elf-objdump -M numeric -M no-aliases -d t.o
