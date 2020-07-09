	.file	"array_test1.c"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0_c2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.globl	judgeResult
	.section	.sbss,"aw",@nobits
	.align	2
	.type	judgeResult, @object
	.size	judgeResult, 4
judgeResult:
	.zero	4
	.globl	Mod
	.section	.srodata,"a"
	.align	2
	.type	Mod, @object
	.size	Mod, 4
Mod:
	.word	253
	.text
	.align	1
	.globl	printInt
	.type	printInt, @function
printInt:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	lui	a5,%hi(judgeResult)
	lw	a4,%lo(judgeResult)(a5)
	lw	a5,-20(s0)
	xor	a4,a4,a5
	lui	a5,%hi(judgeResult)
	sw	a4,%lo(judgeResult)(a5)
	lui	a5,%hi(judgeResult)
	lw	a5,%lo(judgeResult)(a5)
	addi	a4,a5,173
	lui	a5,%hi(judgeResult)
	sw	a4,%lo(judgeResult)(a5)
	nop
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	printInt, .-printInt
	.align	1
	.globl	printStr
	.type	printStr, @function
printStr:
	addi	sp,sp,-48
	sw	s0,44(sp)
	addi	s0,sp,48
	sw	a0,-36(s0)
	lw	a5,-36(s0)
	sw	a5,-20(s0)
	j	.L3
.L4:
	lw	a5,-20(s0)
	lbu	a5,0(a5)
	mv	a4,a5
	lui	a5,%hi(judgeResult)
	lw	a5,%lo(judgeResult)(a5)
	xor	a4,a4,a5
	lui	a5,%hi(judgeResult)
	sw	a4,%lo(judgeResult)(a5)
	lui	a5,%hi(judgeResult)
	lw	a5,%lo(judgeResult)(a5)
	addi	a4,a5,521
	lui	a5,%hi(judgeResult)
	sw	a4,%lo(judgeResult)(a5)
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L3:
	lw	a5,-20(s0)
	lbu	a5,0(a5)
	bne	a5,zero,.L4
	nop
	nop
	lw	s0,44(sp)
	addi	sp,sp,48
	jr	ra
	.size	printStr, .-printStr
	.globl	a
	.bss
	.align	2
	.type	a, @object
	.size	a, 16
a:
	.zero	16
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-48
	sw	ra,44(sp)
	sw	s0,40(sp)
	addi	s0,sp,48
	sw	zero,-20(s0)
	j	.L6
.L7:
	lui	a5,%hi(a)
	addi	a4,a5,%lo(a)
	lw	a5,-20(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	sw	zero,0(a5)
	lw	a5,-20(s0)
	addi	a4,a5,1
	lw	a5,-20(s0)
	slli	a5,a5,2
	addi	a3,s0,-16
	add	a5,a3,a5
	sw	a4,-24(a5)
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L6:
	lw	a4,-20(s0)
	li	a5,3
	ble	a4,a5,.L7
	sw	zero,-20(s0)
	j	.L8
.L9:
	lui	a5,%hi(a)
	addi	a4,a5,%lo(a)
	lw	a5,-20(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a5,0(a5)
	mv	a0,a5
	call	printInt
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L8:
	lw	a4,-20(s0)
	li	a5,3
	ble	a4,a5,.L9
	addi	a5,s0,-40
	sw	a5,-24(s0)
	sw	zero,-20(s0)
	j	.L10
.L11:
	lw	a5,-20(s0)
	slli	a5,a5,2
	lw	a4,-24(s0)
	add	a5,a4,a5
	lw	a5,0(a5)
	mv	a0,a5
	call	printInt
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L10:
	lw	a4,-20(s0)
	li	a5,3
	ble	a4,a5,.L11
	lui	a5,%hi(judgeResult)
	lw	a4,%lo(judgeResult)(a5)
	li	a5,253
	rem	a5,a4,a5
	mv	a0,a5
	lw	ra,44(sp)
	lw	s0,40(sp)
	addi	sp,sp,48
	jr	ra
	.size	main, .-main
	.ident	"GCC: (GNU) 10.1.0"
