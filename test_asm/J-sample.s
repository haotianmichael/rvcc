    /*无内容int main() {}*/
    .text
    .align  1
    .global main
    .type   main @function
main:
    addi sp, sp, -16
    sw s0, 12(sp)
    addi  s0, sp, 16
    li a5, 0
    mv a0, a5
    lw s0, 12(sp)
    addi sp, sp, 16
    jr ra

 
    /*无函数调用版本 J-constTB.c*/
    addi sp, sp, -16
    sw s0, 12(sp)    
    addi s0,sp,16
    li  a4,2
    li  a5,3
    add a4, a4, a5
    lui a5,%hi(sum)
    sw  a4,%lo(sum)(a5)
    li  a5,0
    mv  a0,a5
    lw  s0,12(sp)
    addi sp,sp,16
    jr  ra
    
   
    /*有函数版本  J-varTB.c
        printf("");
    */
    addi sp, sp, -16
    sw ra,12(sp)
    sw s0,8(sp)
    addi s0,sp,16
    lui a5,%hi(.LC0)
    addi a0,a5,%lo(.LC0)
    call puts
    li a5,0
    mv a0,a5
    lw ra,12(sp)
    lw s0,8(sp)
    addi sp,sp,16
    jr ra


    /*有函数调用版本
        int a, b, sum; int sum(int a, int b) {sum = a + b; return (sum);}
        int main() {a = 10; b = 10; sum = fun(a, b);}
    */

    .align 1
    .globl fun
    .type fun, @function
fun:
    addi sp, sp, -48
    sw s0, 44(sp)
    addi s0, sp, 48
    sw  a0, -36(s0)
    sw  a1, -40(s0)
    lw  a4, -36(s0) 
    lw  a5, -40(s0)
    add a5,a4,a5
    sw a5,-20(s0)
    lw a5,-20(s0)
    mv a0,a5
    lw s0,44(sp) 
    addi sp,sp,48   
    jr ra
    .size fun, .-fun

    .align 1
    .globl main
    .type main, @function
main:
    addi sp, sp, -16
    sw ra,12(sp)
    sw s0,8(sp)
    addi s0,sp,16
    lui a5,%hi(a)
    li a4,10
    sw a4,%lo(a)(a5)
    lui a5,%hi(b)
    li a4,10
    sw a4,%lo(b)(a5)
    lui a5,%hi(a)
    lw a4,%lo(a)(a5)
    lui a5,%hi(b)
    lw a5,%lo(b)(a5)
    mv a1,a5
    mv a2,a4
    call fun
    mv a4,a0
    lui a5,%hi(sum)
    sw a4,%lo(sum)(s5)
    li a5,0
    mv a0,a5
    lw ra,12(sp)
    lw s0,8(sp)
    addi sp, sp, 16
    jr ra
    .size main, .-main


