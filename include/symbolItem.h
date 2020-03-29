#pragma  once
#include "lex.h"



/*
   分程序结构 符号表项设计
   子类继承方式   root根符号表对多类型符号表用vector管理
   @根符号表
   @函数符号表
   @过程符号表 (作用域)
   @局部变量符号表
   @数组符号表 
 */
class symbolItem
{
    public:
        symbolItem () {}
        virtual ~symbolItem () {}
};

//函数
class funcItem : public symbolItem
{
    public:
        funcItem ();
        virtual ~funcItem ();

    private:
        funcReturnType frt;    //函数返回类型

};

//过程
class procItem : public symbolItem 
{
    public:
        procItem();
        virtual ~procItem();

    private:

};

//常量，变量
class localItem : public symbolItem {

    public:
        localItem();
        virtual ~localItem();

    private:
        localMold lm;   //常量&变量
        itemType it;  //具体数据类型
        int _iValue;   //整形值
        char _cValue;   //字符值
};


//数组-一维
class arrayItem : public symbolItem
{
    public:
        arrayItem ();
        virtual ~arrayItem ();

    private:
        int length;   //数组长度
};

