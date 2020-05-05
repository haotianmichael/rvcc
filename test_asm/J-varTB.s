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
	.ident	"GCC: (GNU) 8.3.0"
