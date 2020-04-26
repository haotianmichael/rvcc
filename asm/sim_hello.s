    .text
    .align  2
    .global     main
main:
    addi sp, sp, -16
    sw  ra, 12(sp)
    lui a0, %hi(string1)
    addi a0, a0, %lo(string1) 
    lui a1, %hi(string2)
    addi a1, a1, %lo(string2)
    call printf
    lw ra, 12(sp)
    addi sp, sp, 16
    li a0, 0
    ret
    .section    .rodata
    .balign     4
string1:
    .string "Hello, %s\n"
string1:
    .string "haotianmichael"

