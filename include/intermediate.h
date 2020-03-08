#pragma  once
#include "lex.h"

/*
 
四元式结构总体设计
1.值参传入push x, push y
2.调用函数call add
3.赋值语句 i = ret  i = t1 + 1
4.条件判断 x == y x <= y
5.纯标签Label1:
6.跳转函数 goto label1  bne label
7.函数返回 ret x  ret
8.函数声明 int x
9.参数表 param x
10.print "xxx"  print 'c'  print 23  print a
11.read int a, read char c
 */
enum TmpCodeType{
    ValueParamDeliver,
    FunctionCall,
    AssignStmt,
    Label,
    FunctionDef,
    ParamDef,
    Jump,
    BEZ,
    BNZ,
    BLZ,
    BLEZ,
    BGZ,
    BGEZ,
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
    OverProcedure

};

//四元式结构体
struct FourYuanItem{
    TmpCodeType type;
    ValueType valueType;   //参数   print语句表达式输出
    ReturnFucType FuncType;
    std::string target;
    std::string index1;
    bool isTargetArr;
    bool isLeftArr;
    bool isNotPrintCharId;   //打印的是不是char类型的id
    std::string left;
    std::string index2;
    std::string right;
    char op;
};




//中缀表达式转逆波兰表达式栈的项结构体
struct PostfixItem{
    ValueType type;
    std::string str;
    int number;
    bool isNorCharVar;   //是否是char型变量或者说是char型数组某个元素
    bool isNotOperator;  //如果是char型常量， 是不是运算符
};
