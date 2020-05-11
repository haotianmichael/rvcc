	.file	"J-constTB.c"
	.option nopic
	.text
	.globl	i
	.section	.srodata,"a"
	.align	2
	.type	i, @object
	.size	i, 4
i:
	.word	-11023
	.globl	a
	.align	2
	.type	a, @object
	.size	a, 4
a:
	.word	2
	.globl	b
	.align	2
	.type	b, @object
	.size	b, 4
b:
	.word	3
	.globl	c
	.align	2
	.type	c, @object
	.size	c, 4
c:
	.word	5
	.globl	u
	.type	u, @object
	.size	u, 1
u:
	.byte	117
	.globl	v
	.type	v, @object
	.size	v, 1
v:
	.byte	118
	.globl	w
	.type	w, @object
	.size	w, 1
w:
	.byte	119
	.globl	g
	.type	g, @object
	.size	g, 1
g:
	.byte	97
	.section	.rodata
	.align	2
.LC0:
	.string	"hello world"
	.align	2
.LC1:
	.string	"hello buaa"
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-16
	sw	ra,12(sp)
	sw	s0,8(sp)
	addi	s0,sp,16
	lui	a5,%hi(.LC0)
	addi	a0,a5,%lo(.LC0)
	call	puts
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	li	a5,0
	mv	a0,a5
	lw	ra,12(sp)
	lw	s0,8(sp)
	addi	sp,sp,16
	jr	ra
	.size	main, .-main
	.ident	"GCC: (GNU) 8.3.0"
