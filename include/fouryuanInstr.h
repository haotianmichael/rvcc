#pragma once
#include "lex.h"

/*
   中间代码设计
   四元式
   QuaterOpcode src1  src2  res 
   [操作码] [源操作数1]  [源操作数2]  [结果]
 */

//四元式操作码
enum fourYuanOpcode{

    /*计算指令*/
    ADD,    //res = src1 + src2
    SUB,    //res = src1 - src2
    MUL,    //res = src1 * src2
    DIV,    //res = src1 / src2
    NEG,    //res = -src1
    ASS,    //res = src1


    /*跳转指令*/
    JMP,    //无条件跳转到标签res
    LABEL,  //生成标签res
    JT,     //if(src1 == true) 跳转到 标签res
    JNT,    //if(src1 == false) 跳转到 标签res
    GT,     //src1 > src2
    GE,     //src1 >= src2
    LT,     //src1 < src2
    LE,     //src1 <= src2
    ENQ,    //src1 == src2
    BNE,    //src1 != src2


    /*函数*/
    PUSH,  //参数
    FUNCALL,   //调用
    FUNDEC,    //声明
    RET,   //返回
    PARAM,  //参数表
    BEGIN,  //函数入口
    END,  //函数出口

    /*命令*/
    ReadChar,
    ReadInt,

    PrintStr,
    PrintChar,
    PrintInt,
    PrintId,
    
    ReturnChar,
    ReturnInt,
    ReturnId,
    ReturnEmpty, 
};



class FourYuanInstr
{
    public:
        FourYuanInstr (fourYuanOpcode opcode, std::string left, std::string right, std::string res);
        virtual ~FourYuanInstr (){}
        void printFourYuan();

        /*get*/
        inline std::string getleft() { return __left;};
        inline std::string getright() { return __right;}
        inline fourYuanOpcode getopcode() { return __opcode;}

    private:
        fourYuanOpcode __opcode;
        std::string __left;
        std::string __right;
        std::string __res;
};


