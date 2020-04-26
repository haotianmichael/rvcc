    .file   "ori_hello.c"
    .option nopic
    .text
    .section    .rodata
    .align  2
.LC0:
    .string "haotianmichael"
    .align  2
.LC1:
    .string "Hello %s\n"
    .text
    .align 1
    .global     main
    .type   main, @function
main:
    addi sp, sp, -16
    sw ra, 12(sp)
    sw s0, 8(sp)
    addi s0, sp, 16
    lui a5, %hi(.LC0)
    addi a1, a5, %lo(.LC0)
    lui a1, %hi(.LC1)
    addi a0, a5, %lo(.LC1)
    call printf
    li a5, 0
    mv a0, a5
    lw ra, 12(sp)
    lw s0, 8(sp)
    addi sp, sp, 16
    jr ra
    .size   main, .-main
    .ident  "GCC:(GNU)8.3.0"
    
