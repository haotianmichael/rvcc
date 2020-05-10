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
/*    ADD,    //res = src1 + src2*/
    //SUB,    //res = src1 - src2
    //MUL,    //res = src1 * src2
    /*DIV,    //res = src1 / src2*/
    //NEG,    //res = -src1
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


//后缀表达式
struct PostfixExpression {
    itemType it;
    int value;  
    char cvalue;
    std::string str;  //变量名称
    /*
        if(!isconstant) 
        then  str
    */
    bool isconstant;   //是常量还是变量
    bool isOpcode;   //是否为运算符
};


//中间代码四元式
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
        inline bool getistargetArr() { return __targetArr;}
        inline bool getissrcArr() { return __srcArr;}
        inline bool getparaArr() { return __paraArr;}
        inline char getop() { return __op;}
        inline char getcvalue() {return __cvalue;}
        inline int getvalue() { return __value;}
        /*set*/
        inline void setleft(std::string left) { __left = left;}
        inline void setright(std::string right) { __right = right;}
        inline void setopcode(fourYuanOpcode fyo) { __opcode = fyo;}
        inline void settarget(std::string target) { __target = target;}
        inline void setparat(itemType it) { __paraT = it;}
        inline void setfunct(funcReturnType frt) { __frt = frt;}
        inline void settargetArr(bool is) { __targetArr = is;}
        inline void setsrcArr(bool is) { __srcArr = is;}
        inline void setparaArr(bool is) { __paraArr = is;}
        inline void setop(char op) { __op = op;}
        inline void setcvalue(char cvalue) { __cvalue = cvalue;}
        inline void setvalue(int value) { __value = value;}
    private:
        /*element*/
        fourYuanOpcode __opcode;   //中间代码操作符
        std::string __target = "";  //结果
        std::string __left = "";  //左操作数
        std::string __right = "";  //右操作数
        itemType __paraT;  //参数类型
        funcReturnType __frt;  //函数返回类型
        int __value;  //值
        char __cvalue; //值

        bool __targetArr;  //赋值  结果是否为数组
        bool __srcArr;  //赋值  操作数是否为数组
        bool __paraArr;  //参数是否为数组
        char __op;    //运算符
};
