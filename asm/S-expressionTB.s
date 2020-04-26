	.file	"S-expressionTB.c"
	.option nopic
	.text
	.globl	N
	.section	.srodata,"a"
	.align	2
	.type	N, @object
	.size	N, 4
N:
	.word	1001
	.text
	.align	1
	.globl	fun
	.type	fun, @function
fun:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	sw	a1,-24(s0)
	mv	a5,a2
	sw	a3,-32(s0)
	sb	a5,-25(s0)
	nop
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	fun, .-fun
	.align	1
	.globl	foo
	.type	foo, @function
foo:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	nop
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	foo, .-foo
	.align	1
	.globl	bee
	.type	bee, @function
bee:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	nop
	mv	a0,a5
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	bee, .-bee
	.align	1
	.globl	duu
	.type	duu, @function
duu:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	sw	a1,-24(s0)
	sw	a2,-28(s0)
	nop
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	duu, .-duu
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-96
	sw	ra,92(sp)
	sw	s0,88(sp)
	sw	s1,84(sp)
	sw	s2,80(sp)
	addi	s0,sp,96
	lw	a4,-20(s0)
	lw	a5,-24(s0)
	mul	a5,a4,a5
	addi	a5,a5,-10
	mv	a0,a5
	call	foo
	lw	a4,-28(s0)
	li	a5,-390
	div	a4,a4,a5
	lw	a5,-24(s0)
	neg	a3,a5
	lw	a5,-20(s0)
	mul	a5,a3,a5
	slli	a5,a5,2
	add	a5,a4,a5
	mv	a0,a5
	call	foo
	lbu	a5,-86(s0)
	mv	a4,a5
	lw	a5,-28(s0)
	sub	a5,a4,a5
	mv	a0,a5
	call	foo
	lbu	a5,-86(s0)
	mv	a4,a5
	li	a5,-390
	div	a4,a4,a5
	lbu	a5,-85(s0)
	mv	a3,a5
	lw	a5,-24(s0)
	neg	a5,a5
	mul	a5,a3,a5
	slli	a5,a5,2
	add	a4,a4,a5
	lw	a5,-64(s0)
	add	a5,a4,a5
	mv	a0,a5
	call	foo
	li	a0,5
	call	bee
	mv	a3,a0
	lbu	a5,-85(s0)
	neg	a4,a5
	lw	a5,-24(s0)
	mul	a5,a4,a5
	add	a4,a3,a5
	lw	a5,-64(s0)
	sub	a5,a4,a5
	mv	a0,a5
	call	foo
	lbu	a5,-86(s0)
	addi	a5,a5,-90
	mv	a0,a5
	call	bee
	mv	a4,a0
	lbu	a5,-84(s0)
	neg	a5,a5
	mul	a4,a4,a5
	lw	a3,-28(s0)
	lw	a5,-24(s0)
	sub	a5,a3,a5
	div	a5,a4,a5
	lbu	a4,-85(s0)
	add	s1,a5,a4
	lbu	a5,-86(s0)
	mv	a4,a5
	lw	a5,-28(s0)
	sub	s2,a4,a5
	li	a0,5
	call	bee
	mv	a3,a0
	lbu	a5,-85(s0)
	neg	a4,a5
	lw	a5,-24(s0)
	mul	a5,a4,a5
	add	a4,a3,a5
	lw	a5,-64(s0)
	neg	a5,a5
	lbu	a3,-85(s0)
	div	a5,a5,a3
	add	a5,a4,a5
	mv	a2,a5
	mv	a1,s2
	mv	a0,s1
	call	duu
	li	a5,0
	mv	a0,a5
	lw	ra,92(sp)
	lw	s0,88(sp)
	lw	s1,84(sp)
	lw	s2,80(sp)
	addi	sp,sp,96
	jr	ra
	.size	main, .-main
	.ident	"GCC: (GNU) 8.3.0"
