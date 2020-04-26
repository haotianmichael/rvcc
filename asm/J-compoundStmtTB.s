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
	.word	10010
	.comm	a,4,4
	.comm	b,4,4
	.comm	c,4,4
	.comm	d,4,4
	.text
	.align	1
	.globl	funa
	.type	funa, @function
funa:
	addi	sp,sp,-16
	sw	s0,12(sp)
	addi	s0,sp,16
	nop
	lw	s0,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	funa, .-funa
	.align	1
	.globl	funb
	.type	funb, @function
funb:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	sw	a1,-24(s0)
	nop
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	funb, .-funb
	.section	.rodata
	.align	2
.LC0:
	.string	"haotian is a good man"
	.align	2
.LC1:
	.string	"he have bad life"
	.text
	.align	1
	.globl	func
	.type	func, @function
func:
	addi	sp,sp,-64
	sw	ra,60(sp)
	sw	s0,56(sp)
	addi	s0,sp,64
	sw	a0,-52(s0)
	sw	a1,-56(s0)
	sw	a2,-60(s0)
	li	a5,10
	sw	a5,-20(s0)
	li	a5,110
	sw	a5,-24(s0)
	li	a5,98304
	addi	a5,a5,1706
	sw	a5,-28(s0)
	nop
.L4:
	lw	a4,-32(s0)
	lw	a5,-36(s0)
	add	a5,a4,a5
	lw	a4,-40(s0)
	bne	a4,a5,.L4
	lui	a5,%hi(.LC0)
	addi	a0,a5,%lo(.LC0)
	call	printf
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	printf
	nop
	mv	a0,a5
	lw	ra,60(sp)
	lw	s0,56(sp)
	addi	sp,sp,64
	jr	ra
	.size	func, .-func
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-48
	sw	ra,44(sp)
	sw	s0,40(sp)
	addi	s0,sp,48
	li	a5,10
	sw	a5,-20(s0)
	li	a5,110
	sw	a5,-24(s0)
	li	a5,8192
	addi	a5,a5,1818
	sw	a5,-28(s0)
	lui	a5,%hi(.LC0)
	addi	a0,a5,%lo(.LC0)
	call	printf
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	printf
.L6:
	lw	a4,-32(s0)
	lw	a5,-40(s0)
	bgt	a4,a5,.L6
	li	a5,0
	mv	a0,a5
	lw	ra,44(sp)
	lw	s0,40(sp)
	addi	sp,sp,48
	jr	ra
	.size	main, .-main
	.ident	"GCC: (GNU) 8.3.0"
