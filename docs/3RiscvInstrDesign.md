##  RISCV32I汇编格式

### 汇编指示符
*   .file  "demo"                               汇编文件名
*   .option  {rvc,norvc,pic,nopic,push,pop}     RISCV选项
*   .text                                       代码段
*   .global  main                               声明全局符号"main"      
*   .align  2                                   后续代码按字节对齐
*   .section .rodata                            进入只读数据段
*   .type symbol, @function
*   .size symbol, symbol                
*   .word  expression                               
*   .comm symbol_name, size, align              
*   .ident "string"




