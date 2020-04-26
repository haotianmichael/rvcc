	.file	"S-SDTTestBench.c"
	.option nopic
	.text
	.globl	cosa
	.section	.srodata,"a"
	.align	2
	.type	cosa, @object
	.size	cosa, 4
cosa:
	.word	10
	.globl	cosb
	.align	2
	.type	cosb, @object
	.size	cosb, 4
cosb:
	.word	15
	.comm	array,40,4
	.text
	.align	1
	.globl	foo
	.type	foo, @function
foo:
	addi	sp,sp,-48
	sw	s0,44(sp)
	addi	s0,sp,48
	sw	a0,-36(s0)
	sw	a1,-40(s0)
	lw	a4,-36(s0)
	lw	a5,-40(s0)
	add	a4,a4,a5
	li	a5,10
	ble	a4,a5,.L2
	lw	a4,-36(s0)
	lw	a5,-40(s0)
	add	a5,a4,a5
	sw	a5,-20(s0)
	lw	a5,-20(s0)
	j	.L3
.L2:
	lw	a4,-36(s0)
	lw	a5,-40(s0)
	sub	a4,a4,a5
	lui	a5,%hi(array)
	addi	a5,a5,%lo(array)
	sw	a4,12(a5)
	lui	a5,%hi(array)
	addi	a5,a5,%lo(array)
	lw	a5,12(a5)
.L3:
	mv	a0,a5
	lw	s0,44(sp)
	addi	sp,sp,48
	jr	ra
	.size	foo, .-foo
	.align	1
	.globl	partion
	.type	partion, @function
partion:
	addi	sp,sp,-48
	sw	s0,44(sp)
	addi	s0,sp,48
	sw	a0,-36(s0)
	sw	a1,-40(s0)
	sw	a2,-44(s0)
	lw	a5,-40(s0)
	sw	a5,-28(s0)
	lw	a5,-44(s0)
	sw	a5,-20(s0)
	lw	a5,-40(s0)
	sw	a5,-24(s0)
	j	.L5
.L7:
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)
.L6:
	lw	a5,-24(s0)
	slli	a5,a5,2
	lw	a4,-36(s0)
	add	a5,a4,a5
	lw	a4,0(a5)
	lw	a5,-28(s0)
	slli	a5,a5,2
	lw	a3,-36(s0)
	add	a5,a3,a5
	lw	a5,0(a5)
	blt	a4,a5,.L7
	j	.L8
.L9:
	lw	a5,-20(s0)
	addi	a5,a5,-1
	sw	a5,-20(s0)
.L8:
	lw	a5,-20(s0)
	slli	a5,a5,2
	lw	a4,-36(s0)
	add	a5,a4,a5
	lw	a4,0(a5)
	lw	a5,-28(s0)
	slli	a5,a5,2
	lw	a3,-36(s0)
	add	a5,a3,a5
	lw	a5,0(a5)
	bgt	a4,a5,.L9
	lw	a4,-24(s0)
	lw	a5,-20(s0)
	bge	a4,a5,.L5
	lw	a5,-20(s0)
	slli	a5,a5,2
	lw	a4,-36(s0)
	add	a5,a4,a5
	lw	a5,0(a5)
	sw	a5,-32(s0)
	lw	a5,-24(s0)
	slli	a5,a5,2
	lw	a4,-36(s0)
	add	a4,a4,a5
	lw	a5,-20(s0)
	slli	a5,a5,2
	lw	a3,-36(s0)
	add	a5,a3,a5
	lw	a4,0(a4)
	sw	a4,0(a5)
	lw	a5,-24(s0)
	slli	a5,a5,2
	lw	a4,-36(s0)
	add	a5,a4,a5
	lw	a4,-32(s0)
	sw	a4,0(a5)
.L5:
	lw	a4,-24(s0)
	lw	a5,-20(s0)
	blt	a4,a5,.L6
	lw	a5,-28(s0)
	slli	a5,a5,2
	lw	a4,-36(s0)
	add	a4,a4,a5
	lw	a5,-24(s0)
	slli	a5,a5,2
	lw	a3,-36(s0)
	add	a5,a3,a5
	lw	a4,0(a4)
	sw	a4,0(a5)
	lw	a5,-24(s0)
	mv	a0,a5
	lw	s0,44(sp)
	addi	sp,sp,48
	jr	ra
	.size	partion, .-partion
	.align	1
	.globl	quickSort
	.type	quickSort, @function
quickSort:
	addi	sp,sp,-48
	sw	ra,44(sp)
	sw	s0,40(sp)
	addi	s0,sp,48
	sw	a0,-36(s0)
	sw	a1,-40(s0)
	sw	a2,-44(s0)
	lw	a4,-40(s0)
	lw	a5,-44(s0)
	bge	a4,a5,.L14
	lw	a2,-44(s0)
	lw	a1,-40(s0)
	lw	a0,-36(s0)
	call	partion
	sw	a0,-20(s0)
	lw	a2,-20(s0)
	lw	a1,-40(s0)
	lw	a0,-36(s0)
	call	quickSort
	lw	a5,-20(s0)
	addi	a5,a5,1
	lw	a2,-44(s0)
	mv	a1,a5
	lw	a0,-36(s0)
	call	quickSort
.L14:
	nop
	lw	ra,44(sp)
	lw	s0,40(sp)
	addi	sp,sp,48
	jr	ra
	.size	quickSort, .-quickSort
	.section	.rodata
	.align	2
.LC0:
	.string	"%d "
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-32
	sw	ra,28(sp)
	sw	s0,24(sp)
	addi	s0,sp,32
	sw	zero,-20(s0)
	j	.L16
.L17:
	li	a4,5
	lw	a5,-20(s0)
	sub	a4,a4,a5
	lui	a5,%hi(array)
	addi	a3,a5,%lo(array)
	lw	a5,-20(s0)
	slli	a5,a5,2
	add	a5,a3,a5
	sw	a4,0(a5)
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L16:
	lw	a4,-20(s0)
	li	a5,4
	ble	a4,a5,.L17
	li	a2,4
	li	a1,0
	lui	a5,%hi(array)
	addi	a0,a5,%lo(array)
	call	quickSort
	sw	zero,-20(s0)
	j	.L18
.L19:
	lui	a5,%hi(array)
	addi	a4,a5,%lo(array)
	lw	a5,-20(s0)
	slli	a5,a5,2
	add	a5,a4,a5
	lw	a5,0(a5)
	mv	a1,a5
	lui	a5,%hi(.LC0)
	addi	a0,a5,%lo(.LC0)
	call	printf
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L18:
	lw	a4,-20(s0)
	li	a5,4
	ble	a4,a5,.L19
	li	a5,0
	mv	a0,a5
	lw	ra,28(sp)
	lw	s0,24(sp)
	addi	sp,sp,32
	jr	ra
	.size	main, .-main
	.ident	"GCC: (GNU) 8.3.0"
