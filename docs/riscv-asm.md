# RISC-V 汇编程序员指南

# 命令行参数

与其在这里重复，我觉得可能去翻binutils的文档可能更好。

# 寄存器

寄存器对所有处理器来讲都是最重要的。RISC-V 定义了很多种寄存器，有哪些具体要取决于包括了哪些拓展。主寄存器（还有程序计数器 `PC` ），控制寄存器，浮点寄存器（ F 拓展）以及向量寄存器（ V 拓展）。

## 主寄存器

基础整数指令集RV32I包括32个寄存器，从 `x0` 到 `x31`。与其他处理器（比如ARM-32）形成对比的是，程序计数器 `PC` 是和这些寄存器单列的。第一个寄存器 `x0` 有特殊的用途。读这个寄存器一定会返回0, 写这个寄存器则会被忽略。我们很快就会看到，这个特性可以带来一些技巧和简化。

在实践上，程序员通常不会用这种寄存器的表示方法。传统上特定的寄存器会有特殊的用途，尽管在处理器的角度看来 `x1` 到 `x31` 都是等价的通用寄存器。另外，那些你通常在汇编代码里读到的名字，是汇编器给他们取的符合RISC-V**应用二进制接口**（ ABI ）标准的名字。如果你真的想要看到他们的原始名称，你可以在 objdump 中加 `-M` 参数。

寄存器  | ABI名字     | 传统用法                   | 保留？ 
:-------- | :---------- | :---------------                      | ------
x0        | zero        | 接地的寄存器，忽视写入 | _/_ 
x1        | ra          | 返回的地址指针      | 否  
x2        | sp          | 栈指针                       | 是 
x3        | gp          | 全局指针                      | _/_ 
x4        | tp          | 线程指针                      | */* 
x5        | t0          | 临时寄存器 0          | 否 
x6        | t1          | 临时寄存器 1          | 否 
x7        | t2          | 临时寄存器 2          | 否 
x8        | s0 _or_ fp  | 保留寄存器 0 或者 帧指针 | 是 
x9        | s1          | 保留寄存器 1             | 是 
x10       | a0          | 返回值 或者 程序参数 0 | 否 
x11       | a1          | 返回值 或者 程序参数 1 | 否 
x12       | a2          | 程序参数 2            | 否 
x13       | a3          | 程序参数 3            | 否 
x14       | a4          | 程序参数 4            | 否 
x15       | a5          | 程序参数 5            | 否 
x16       | a6          | 程序参数 6            | 否 
x17       | a7          | 程序参数 7            | 否 
x18       | s2          | 保留寄存器 2            | 是 
x19       | s3          | 保留寄存器 3              | 是 
x20       | s4          | 保留寄存器 4            | 是 
x21       | s5          | 保留寄存器 5            | 是 
x22       | s6          | 保留寄存器 6            | 是 
x23       | s7          | 保留寄存器 7            | 是 
x24       | s8          | 保留寄存器 8            | 是 
x25       | s9          | 保留寄存器 9            | 是 
x26       | s10         | 保留寄存器 10           | 是 
x27       | s11         | 保留寄存器 11           | 是 
x28       | t3          | 临时寄存器 3          | 否 
x29       | t4          | 临时寄存器 4          | 否 
x30       | t5          | 临时寄存器 5          | 否 
x31       | t6          | 临时寄存器 6          | 否 
pc        | _(没有)_    | 程序计数器                  | _/_ 

*表： RV32I 的寄存器。基于 RISC-V 文档与帕特森，沃特曼著《 RISC-V 读本》（2017）*

依照通常的规定，**保留寄存器** `s0` 到 `s11` 在函数调用的过程中会被保留，而**函数参数寄存器** `a0` to `a7` 和**临时寄存器** `t0` to `t6` 则不会。传统的特殊寄存器（比如 `sp` ）的用法会在后文详细讨论。

*控制寄存器、浮点寄存器 （ RV32F ）、向量寄存器（ RV32V ）的内容缺失。*


# 寻址

寻址的格式是 %pcrel_lo()。可以直接翻看RISC-V PS ABI的文档，来了解地址跳转具体是怎样工作的。

# 指令集

官方参数页面：https://riscv.org/specifications/

最新草案的存储库链接：https://github.com/riscv/riscv-isa-manual

[RISC-V 用户层 ISA 指令](https://riscv.org/specifications/)  [RISC-V 特权 ISA 指令](https://riscv.org/specifications/privileged-isa/)

## 指令集的别名

opcodes/riscv-opc.c 中的别名。

当你的程序流流进了奇怪的地方的时候，你可能想要作出一些陷阱指令来探测与诊断。对此，你可以使用 `UNIMP` 伪指令。这个几乎在所有的系统里都可用。*事实上*这个指令的标准实现有：

- `C.UNIMP`: `0000` 全零是一个非法指令。任何捕获非法指令的系统都会在此捕获到 `UNIMP` 指令。尽管这不是一个合法指令，但是它符合 16 位（压缩指令集）的格式，因此 `0000 0000` 会被转义成两个16位的 `UNIMP` 指令。
- `UNIMP` : `C0001073` 这是 `CSRRW x0, cycle, x0` 的一个别名。`cycle` 是一个只读的控制寄存器，所以任何向 `cycle` 写数据的操作都会抛出非法指令异常。如果系统没有C拓展，或者使用了`.option norvc` 指令，就会抛出这个32位形式的 `UNIMP`。

## 伪指令

这里的伪指令包括了RISC-V特定的，和GNU那种用句点开头的伪指令。

这个表是汇编器命令。

命令    | 参数                    | 描述 
:----------- | :-------------                 | :---------------
.align       | 整数                        | 对齐2的指数幂（是 .p2align 的别名） 
.file        | "文件名"                     | 传递符号表中FILE LOCAL 
.globl       | 符号名                    | 向符号表标记全局的符号名 
.local       | 符号名                 | 向符号表标记局部的符号名 
.comm        | 符号名,大小,对齐   | 向.bss段标记一个通常的对象 
.common      | 符号名,大小,对齐 | 向.bss段标记一个通常的对象 
.ident       | "字符串"                       | 描述源代码兼容性 
.section     | [{.text,.data,.rodata,.bss}]   | 开始一个段（默认是.text） 
.size        | 符号, 符号               | 描述源代码兼容性 
.text        |                                | 开始一个.text段 
.data        |                                | 开始一个.data段 
.rodata      |                                | 开始一个.rodata段 
.bss         |                                | 开始一个.rodata段 
.string      | "字符串"                    | 开始一个字符串 
.asciz       | "字符串"                    | 开始一个字符串（是 .string 的别名） 
.equ         | 名称, 值                   | 定义一个常量 
.macro       | 名称 参数1 [, 参数n]           | 开始一个宏，参数名会被替换 
.endm        |                                | 结束一个宏
.type        | 符号, @函数              | 描述源代码兼容性 
.option      | {rvc,norvc,pic,nopic,push,pop} | RISC-V 选项 
.byte        | 表达式 [, 表达式]*  | 8位逗号分隔的字组 
.2byte       | 表达式 [, 表达式]* | 16位逗号分隔的字组 
.half        | 表达式 [, 表达式]* | 16位逗号分隔的字组 
.short       | 表达式 [, 表达式]* | 16位逗号分隔的字组 
.4byte       | 表达式 [, 表达式]* | 32位逗号分隔的字组 
.word        | 表达式 [, 表达式]* | 32位逗号分隔的字组 
.long        | 表达式 [, 表达式]* | 32位逗号分隔的字组 
.8byte       | 表达式 [, 表达式]* | 64位逗号分隔的字组 
.dword       | 表达式 [, 表达式]* | 64位逗号分隔的字组 
.quad        | 表达式 [, 表达式]* | 64位逗号分隔的字组 
.dtprelword  | 表达式 [, 表达式]*             | 64位线程局部字 
.dtpreldword | 表达式 [, 表达式]* | 64位线程局部字 
.sleb128     | expression                     | 有符号小端128位DWARF 
.uleb128     | expression                     | 无符号小端128位DWARF 
.p2align     | p2,[pad_val=0],max             | 对齐2的指数幂 
.balign      | b,[pad_val=0]                  | 字节对齐 
.zero        | integer                        | 零字节 

## 汇编器重定位选项

接下来的表格列出了汇编器重定向拓展。

汇编器记号          | 描述                    | 指令 / 宏 
:----------------------     | :---------------               | :-------------------
%hi(symbol)                 | Absolute (HI20)                | lui
%lo(symbol)                 | Absolute (LO12)                | load, store, add
%pcrel_hi(symbol)           | PC-relative (HI20)             | auipc
%pcrel_lo(label)            | PC-relative (LO12)             | load, store, add
%tprel_hi(symbol)           | TLS LE "Local Exec"            | lui
%tprel_lo(symbol)           | TLS LE "Local Exec"            | load, store, add
%tprel_add(symbol)          | TLS LE "Local Exec"            | add
%tls_ie_pcrel_hi(symbol) \* | TLS IE "Initial Exec" (HI20)   | auipc
%tls_gd_pcrel_hi(symbol) \* | TLS GD "Global Dynamic" (HI20) | auipc
%got_pcrel_hi(symbol) \*    | GOT PC-relative (HI20)         | auipc

\* 这些复用了 %pcrel_lo(label) 来取低位。

标签
------------

文字标签通常被用作分支、无条件跳转和符号定义。

文字标签会添加到编译好的模组中的符号表里。

```
loop:
        j loop
```

数字标签被用作局部索引。带 `f` 后缀是正向索引，带 `b` 后缀是反向索引。

```
1:
        j 1b
```

绝对寻址
------------------------

以下例子展示了如何载入一个绝对地址。

```
.section .text
.globl _start
_start:
	    lui a0,       %hi(msg)       # load msg(hi)
	    addi a0, a0,  %lo(msg)       # load msg(lo)
	    jal ra, puts
2:	    j 2b

.section .rodata
msg:
	    .string "Hello World\n"
```

这会被汇编器重定位，正如下面 objdump 展示的那样：

```
0000000000000000 <_start>:
   0:	000005b7          	lui	a1,0x0
			0: R_RISCV_HI20	msg
   4:	00858593          	addi	a1,a1,8 # 8 <.L21>
			4: R_RISCV_LO12_I	msg
```

相对寻址
------------------------

以下例子展示了如何载入一个相对于程序计数器的地址。

```
.section .text
.globl _start
_start:
1:	    auipc a0,     %pcrel_hi(msg) # load msg(hi)
	    addi  a0, a0, %pcrel_lo(1b)  # load msg(lo)
	    jal ra, puts
2:	    j 2b

.section .rodata
msg:
	    .string "Hello World\n"
```

这会被汇编器重定位，正如下面 objdump 展示的那样：

```
0000000000000000 <_start>:
   0:	00000517          	auipc	a0,0x0
			0: R_RISCV_PCREL_HI20	msg
   4:	00050513          	addi	a0,a0,16 # 10 <.L21>
			4: R_RISCV_PCREL_LO12_I	.L11
```

GOT间接寻址
------------------------

以下例子展示了如何从 GOT 中载入一个地址。

```
.section .text
.globl _start
_start:
1:	    auipc a0, %got_pcrel_hi(msg) # load msg(hi)
	    ld    a0, %pcrel_lo(1b)(a0)  # load msg(lo)
	    jal ra, puts
2:	    j 2b

.section .rodata
msg:
	    .string "Hello World\n"
```

这会被汇编器重定位，正如下面 objdump 展示的那样：

```
0000000000000000 <_start>:
   0:	00000517          	auipc	a0,0x0
			0: R_RISCV_GOT_HI20	msg
   4:	00053503          	ld	a0,0(a0) # 0 <_start>
			4: R_RISCV_PCREL_LO12_I	.L11
```

立即数
-------------------

接下来的例子展示了加载立即数的 `li` 伪命令：

```
.section .text
.globl _start
_start:

.equ CONSTANT, 0xdeadbeef

        li a0, CONSTANT
```

对于 RV32I，这会被汇编器处理，正如下面 objdump 展示的那样：

```
   0:   deadc537                lui     a0,0xdeadc
   4:   eef50513                addi    a0,a0,-273
```

加载地址
-----------------

接下来的例子展示了加载地址的 `la` 伪命令：

```
.section .text
.globl _start
_start:

        la a0, msg

.section .rodata
msg:
	    .string "Hello World\n"
```

对于非 PIC，这会被汇编器处理，正如下面 objdump 展示的那样：

```
0000000000000000 <_start>:
   0:	00000517          	auipc	a0,0x0
			0: R_RISCV_PCREL_HI20	msg
   4:	00850513          	addi	a0,a0,8 # 8 <_start+0x8>
			4: R_RISCV_PCREL_LO12_I	.L11
```

对于 PIC，这会被汇编器处理，正如下面 objdump 展示的那样：

```
0000000000000000 <_start>:
   0:	00000517          	auipc	a0,0x0
			0: R_RISCV_GOT_HI20	msg
   4:	00053503          	ld	a0,0(a0) # 0 <_start>
			4: R_RISCV_PCREL_LO12_I	.L0
```

--------------

接下来展示了用%hi 和 %lo 这两个汇编器函数来加载常数。

```
.equ UART_BASE, 0x40003000

        lui a0,      %hi(UART_BASE)
        addi a0, a0, %lo(UART_BASE)
```

这个例子使用 `li` 伪命令来加载一个常数，并使用 polled IO 向 UART 中写一个字符串：

```
.equ UART_BASE, 0x40003000
.equ REG_RBR, 0
.equ REG_TBR, 0
.equ REG_IIR, 2
.equ IIR_TX_RDY, 2
.equ IIR_RX_RDY, 4

.section .text
.globl _start
_start:
1:      auipc a0, %pcrel_hi(msg)    # load msg(hi)
        addi a0, a0, %pcrel_lo(1b)  # load msg(lo)
2:      jal ra, puts
3:      j 3b

puts:
        li a2, UART_BASE
1:      lbu a1, (a0)
        beqz a1, 3f
2:      lbu a3, REG_IIR(a2)
        andi a3, a3, IIR_TX_RDY
        beqz a3, 2b
        sb a1, REG_TBR(a2)
        addi a0, a0, 1
        j 1b
3:      ret

.section .rodata
msg:
	    .string "Hello World\n"
```

浮点数规约模式
-----------------------------

对于一个有规约模式的浮点操作，规约方式可以通过添加一个操作数来制定。比如说 `fcvt.w.s` 四舍五入可以写成  `fcvt.w.s a0, fa0, rtz`。如果没有指定，会使用默认的动态 `dyn` 规约规则。 

支持的规约方式（必须小写）:
* `rne`: 四舍六入五取偶
* `rtz`: 四舍五入
* `rdn`: 向下规约
* `rup`: 向上规约
* `rmm`: round to nearest, ties to max magnitude
* `dyn`: 动态规约模式（使用 `frm` 或者 `fcsr` 寄存器规定的规约模式）

控制/状态寄存器
---------------------------------

接下来的代码展示了如何开启计时器中断，并设置为等待中断发生。

```
.equ RTC_BASE,      0x40000000
.equ TIMER_BASE,    0x40004000

# setup machine trap vector
1:      auipc   t0, %pcrel_hi(mtvec)        # load mtvec(hi)
        addi    t0, t0, %pcrel_lo(1b)       # load mtvec(lo)
        csrrw   zero, mtvec, t0

# set mstatus.MIE=1 (enable M mode interrupt)
        li      t0, 8
        csrrs   zero, mstatus, t0

# set mie.MTIE=1 (enable M mode timer interrupts)
        li      t0, 128
        csrrs   zero, mie, t0

# read from mtime
        li      a0, RTC_BASE
        ld      a1, 0(a0)

# write to mtimecmp
        li      a0, TIMER_BASE
        li      t0, 1000000000
        add     a1, a1, t0
        sd      a1, 0(a0)

# loop
loop:
        wfi
        j loop

# break on interrupt
mtvec:
        csrrc  t0, mcause, zero
        bgez t0, fail       # interrupt causes are less than zero
        slli t0, t0, 1      # shift off high bit
        srli t0, t0, 1
        li t1, 7            # check this is an m_timer interrupt
        bne t0, t1, fail
        j pass

pass:
        la a0, pass_msg
        jal puts
        j shutdown

fail:
        la a0, fail_msg
        jal puts
        j shutdown

.section .rodata

pass_msg:
        .string "PASS\n"

fail_msg:
        .string "FAIL\n"
```

# 版权与许可证信息 

**Copyright and License Information**

Original Work: 原始信息：

RISC-V 汇编程序员指南由以下人员编著：

The RISC-V Assembly Programmer's Manual is

  &copy; 2017 Palmer Dabbelt <palmer@dabbelt.com>
  &copy; 2017 Michael Clark <michaeljclark@mac.com>
  &copy; 2017 Alex Bradbury <asb@lowrisc.org>

原作品采用<a rel="license" href="http://creativecommons.org/licenses/by/4.0/">知识共享署名 4.0 国际许可协议</a>进行许可。

It is licensed under the Creative Commons Attribution 4.0 International License (CC-BY 4.0). The full license text is available at https://creativecommons.org/licenses/by/4.0/.

翻译人员信息：

&copy; 2020 [Victrid](https://victrid.dev) 

本作品采用<a rel="license" href="http://creativecommons.org/licenses/by/4.0/">知识共享署名 4.0 国际许可协议</a>进行许可。

This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by/4.0/">Creative Commons Attribution 4.0 International License</a>.