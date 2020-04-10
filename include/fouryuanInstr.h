#pragma once
#include "lex.h"
#include <vector>

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
        FourYuanInstr() {}
        virtual ~FourYuanInstr (){}
        void printFourYuan();

        /*get*/
        inline std::string getleft() { return __left;};
        inline std::string getright() { return __right;}
        inline fourYuanOpcode getopcode() { return __opcode;}
        inline std::string gettarget() { return __target;}
        inline itemType getparat() { return __paraT;}
        inline funcReturnType getfunct() { return __frt;}
        inline int gettargetindex() { return __targetindex;}
        inline int getsrcindex() { return __srcindex;}
        inline bool getistargetArr() { return __targetArr;}
        inline bool getissrcArr() { return __srcArr;}

        /*set*/
        inline void setleft(std::string left) { __left = left;}
        inline void setright(std::string right) { __right = right;}
        inline void setopcode(fourYuanOpcode fyo) { __opcode = fyo;}
        inline void settarget(std::string target) { __target = target;}
        inline void setparat(itemType it) { __paraT = it;}
        inline void setfunct(funcReturnType frt) { __frt = frt;}
        inline void settargetindex(int index) { __targetindex = index;}
        inline void setsrcindex(int index) { __srcindex = index;}
        inline void settargetArr(bool is) { __targetArr = is;}
        inline void setsrcArr(bool is) { __srcArr = is;}

    private:
        /*element*/
        fourYuanOpcode __opcode;
        std::string __target = "";
        std::string __left = "";
        std::string __right = "";
        itemType __paraT;
        funcReturnType __frt;
        bool __targetArr;
        int __targetindex;
        bool __srcArr;
        int __srcindex; 
};
