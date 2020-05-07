#pragma  once
#include <iostream>
#include "../include/parser.h"

class riscvGenerator
{
    public:
        riscvGenerator (std::string str) {filename = str;}

        /*输出汇编代码*/
        void printAsmCode(Parser &p);  

        /*汇编代码生成函数*/
        void fun_asmCodeGen();
        void nofun_asmCodeGen();
    private:
        std::string filename;
};
