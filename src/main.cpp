#include <iostream>
#include <algorithm>
#include <string>
#include "../include/parser.h"
#include "../include/riscvGenerator.h"

using namespace std;

int 
main(int argc, char *argv[])
{
    if(argc < 2) cout << "ERROR" << endl;
    Parser p(argv[1]);   //前端-语法分析器
    riscvGenerator rv(argv[1]);   //后端-代码生成器

    rv.printAsmCode(p);  //打印
    return 0;
}
