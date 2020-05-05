#pragma  once
#include <iostream>
#include "../include/parser.h"

class riscvGenerator
{
    public:
        riscvGenerator (std::string str) {filename = str;}
        void printAsmCode(Parser &p);  //输出汇编代码
    private:
        std::string filename;
};
