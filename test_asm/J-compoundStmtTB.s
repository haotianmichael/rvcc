	.file	"J-compoundStmtTB.c"
	.option nopic
	.text
	.globl	N
	.section	.srodata,"a"
	.align	2
	.type	N, @object
	.size	N, 4
N:
	.word	100010
	.globl	M
	.align	2
	.type	M, @object
	.size	M, 4
M:
	.word	110
	.comm	a,4,4
	.comm	b,4,4
	.comm	d,4,4
	.section	.rodata
	.align	2
.LC0:
	.string	"haotian is a good man"
	.align	2
.LC1:
	.string	"he have bad life"
	.text
	.align	1
	.globl	fun
	.type	fun, @function
fun:
	addi	sp,sp,-48
	sw	ra,44(sp)
	sw	s0,40(sp)
	addi	s0,sp,48
	sw	a0,-36(s0)
	sw	a1,-40(s0)
	sw	a2,-44(s0)
	lw	a5,-44(s0)
	lw	a5,0(a5)
	lw	a4,-36(s0)
	add	a5,a4,a5
	sw	a5,-20(s0)
	lw	a5,-40(s0)
	sw	a5,-24(s0)
	lw	a4,-20(s0)
	lw	a5,-24(s0)
	ble	a4,a5,.L2
	lui	a5,%hi(.LC0)
	addi	a0,a5,%lo(.LC0)
	call	puts
	j	.L3
.L2:
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
.L3:
	lw	a5,-44(s0)
	lw	a5,0(a5)
	lw	a4,-20(s0)
	sub	a5,a4,a5
	mv	a0,a5
	lw	ra,44(sp)
	lw	s0,40(sp)
	addi	sp,sp,48
	jr	ra
	.size	fun, .-fun
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-64
	sw	ra,60(sp)
	sw	s0,56(sp)
	addi	s0,sp,64
	li	a5,10
	sw	a5,-20(s0)
	li	a5,80
	sw	a5,-24(s0)
	li	a5,10
	sw	a5,-64(s0)
	j	.L6
.L7:
	addi	a5,s0,-64
	mv	a2,a5
	lw	a1,-24(s0)
	lw	a0,-20(s0)
	call	fun
	mv	a4,a0
	lui	a5,%hi(a)
	sw	a4,%lo(a)(a5)
	lw	a4,-64(s0)
	lui	a5,%hi(a)
	lw	a5,%lo(a)(a5)
	add	a5,a4,a5
	sw	a5,-20(s0)
.L6:
	li	a5,110
	lw	a4,-20(s0)
	blt	a4,a5,.L7
	li	a5,0
	mv	a0,a5
	lw	ra,60(sp)
	lw	s0,56(sp)
	addi	sp,sp,64
	jr	ra
	.size	main, .-main
	.ident	"GCC: (GNU) 8.3.0"
