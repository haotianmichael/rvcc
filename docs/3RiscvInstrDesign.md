##  RISCV32I体系结构

### 1. 汇编Directives
| 汇编指示符                                    | 说明                                                         |
| --------------------------------------------- | ------------------------------------------------------------ |
| `.file "demo"`                                | **汇编文件名**                                               |
| `.option {rvc, norvc, pic, nopic, push, pop}` | **RISCV选项{压缩指令，不压缩指令，与位置有关代码段，与位置无关代码段，将option设置存入栈，从栈中弹出option设置}** |
| `.section {.rodata, .text, .data, .bss}`      | **声明的section{只读字段，代码段(机器语言代码)，数据段(全局变量)，bss段(初始化为0的全局变量)}** |
| `.global main`                                | **声明全局符号"main"**                                       |
| `.align n`                                    | **按照2n字节对齐**                                           |
| `.string "str"`                               | **在内存中存入字符串str，空字符结尾**                        |
| ` .word`                                      | **在内存中连续存储n个32位的量**                              |
| `.comm  __symbole`                            | **将符号存储到.bss段中**                                     |
| `.type`                                       | **accepted for source compatibility**                        |
| `.size`                                       | **accepted for source compatibility**                        |
| `.ident`                                      | **accepted for source compatibility**                        |

### 2. 汇编ABI



