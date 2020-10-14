# rvcc

`rvcc` is my first compiler. And I decided not to maintain it.

It supports a [subsets](docs/0GrammarDesign.md) of `C98`. I choose 3-Address Code as IR for optimization. `rvcc` produces `tmpCodeFile.txt`  to show IR, however,  optimization doesn't perform well in  `rvcc`,  which will be the focus of my next Compiler.

For better readability,  `rvcc` produces `symbolTableFile.txt` to show the symbolTable consists  of `var`, `const `and `function name` of source code.

Finally, `riscvCodeFile.s` as the target assembly file will be produced. **BUT**, `rvcc`  achieves few RISCV-Instructions, for Its origin purpose was to learn compiler technology. 

TestSuite is [here](https://github.com/haotianmichael/rvcc/blob/master/test_src/README.md) . **All of tests can be compiled to IR level**. But only [J-constTB.c](https://github.com/haotianmichael/rvcc/blob/master/test_src/J-constTB.c) and [J-varTB.c](https://github.com/haotianmichael/rvcc/blob/master/test_src/J-varTB.c) is **fully compiled to run on the qemu**. [Here](https://haotianmichael.github.io/2020/02/16/Gcc-RISCV%E4%BA%A4%E5%8F%89%E7%BC%96%E8%AF%91%E5%B7%A5%E5%85%B7%E9%93%BE/) shows you  how to build RISC-V GNU Compiler Toolchain.



# RISCV

A breif introduction of my rv-implementation is [here](docs/3RiscvInstrDesign.md); 

* `basic Memory layout`
* `scanf`   
* `printf`



# Build

* `cd rvcc && mkdir build && cd build`

* `cmake ../`
* `make`  
* `./release   ../test_src/**TB.c`



# Output

* `riscvCodeFile.s`          
* `symbolTableFile.txt `     
* `tmpCodeFile.txt`         



