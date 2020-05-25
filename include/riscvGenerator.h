#pragma  once
#include <iostream>
#include "../include/parser.h"
#include <fstream>

class riscvGenerator
{
    public:
        riscvGenerator (std::string str);

        /*输出汇编代码*/
        void printAsmCode(Parser &p);  

        /*汇编代码生成函数*/
        void fun_asmCodeGen(std::ofstream &out);
        void nofun_asmCodeGen(std::ofstream &out, Parser &p, int sp);
        enum TYPE{intermediaT, variableArrayT, variableNoArrayT};
        TYPE assCheckType(std::string name, Parser &p); 
        int getsp() {return riscvsp;}
        void setsp(int sp) { riscvsp = sp;}
        
        
        /*寄存器，内存分配*/
        int arrayAlloc(std::string name, int length, Parser &p);
        bool registerAlloc(int num);  //返回值  true分配到寄存器   false寄存器组已满
        void stackAlloc(int num, int &tmpsp);  //运行栈地址分配
        void  registerFree();
        void stackFree(); 
        std::pair<bool, int > addressAlloc(int num, int &tmpsp);  //用户分配函数  bool 表示是否为寄存器组  int为序号
        void addressFree();
         



        /*赋值语句汇编生成*/
        void intermedia_TargetCodeGen(FourYuanInstr &fy, std::ofstream &out, int &sp);
        void nointermedia_globalTargetCodeGen(FourYuanInstr &fy, std::ofstream &out, int &sp);
        void nointermedia_localTargetCodeGen(FourYuanInstr &fy, std::ofstream &out, int &sp);

        /*中间变量-寄存器映射表*/
        std::map<int, int> intermediaToReg;        //<中间变量序号, 寄存器序号>
        /*中间变量-栈空间地址映射表*/
        std::map<int, int> intermediaToStack;      //<中间变量序号, 运行栈偏移地址>

    private:
        std::string filename;
        std::vector<bool> Reg;  //临时寄存器  false 表示可使用  true  表示不可使用
        int riscvsp = -1;   //临时运行栈顶
        int riscvs0 = -1;   //临时运行栈底

}; 
