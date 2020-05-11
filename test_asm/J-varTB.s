	.file	"J-varTB.c"
	.option nopic
	.text
	.globl	n
	.section	.srodata,"a"
	.align	2
	.type	n, @object
	.size	n, 4
n:
	.word	-110
	.globl	m
	.align	2
	.type	m, @object
	.size	m, 4
m:
	.word	2
	.globl	p
	.type	p, @object
	.size	p, 1
p:
	.byte	112
	.globl	q
	.type	q, @object
	.size	q, 1
q:
	.byte	113
	.globl	r
	.type	r, @object
	.size	r, 1
r:
	.byte	114
	.comm	a,8,4
	.comm	b,12,4
	.comm	d,8,4
	.comm	g,4,4
	.comm	x,4,4
	.comm	y,4,4
	.comm	array,4,4
	.comm	str,40,4
	.comm	ctr,10,4
	.comm	s,1,1
	.comm	o,1,1
	.section	.rodata
	.align	2
.LC0:
	.string	"%d"
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-32
	sw	ra,28(sp)
	sw	s0,24(sp)
	addi	s0,sp,32
	lui	a5,%hi(g)
	addi	a1,a5,%lo(g)
	lui	a5,%hi(.LC0)
	addi	a0,a5,%lo(.LC0)
	call	scanf
	addi	a5,s0,-20
	mv	a1,a5
	lui	a5,%hi(.LC0)
	addi	a0,a5,%lo(.LC0)
	call	scanf
	li	a0,10
	call	putchar
	lw	a5,-20(s0)
	mv	a1,a5
	lui	a5,%hi(.LC0)
	addi	a0,a5,%lo(.LC0)
	call	printf
	li	a0,10
	call	putchar
	lui	a5,%hi(g)
	lw	a5,%lo(g)(a5)
	mv	a1,a5
	lui	a5,%hi(.LC0)
	addi	a0,a5,%lo(.LC0)
	call	printf
	li	a5,0
	mv	a0,a5
	lw	ra,28(sp)
	lw	s0,24(sp)
	addi	sp,sp,32
	jr	ra
	.size	main, .-main
	.ident	"GCC: (GNU) 8.3.0"
