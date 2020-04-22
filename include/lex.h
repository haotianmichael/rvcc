#pragma  once
#include <iostream>
#include <string>

//词素
enum P_Token{

    /*
     * 词法分析器返回的单词类别:
     * 标识符
     * 关键字
     * 整数，字符字面值(常数)
     * 内置符号
     * */
    INVALD = 0,   //<invalid>
    TK_EOF,  // eof

    /*
     * 标识符  用户定义·
     * 用字符串常量直接表示
     */
    TK_IDENT,  

    /*
     * 预处理器  文件名
     * */
    TK_FILENAME,

    /*
     * 内置符号 
     * 支持的运算比较少： 加减乘除  赋值   关系比较符  特殊符号
     *  ""和''表示字面值 本身没有什么价值  不用表示
     * */
    SY_PLUS,   // +
    SY_MINUS,  // -
    SY_TIMES,  // *
    SY_DEV,   // /
    SY_ASSIGN, //=
    SY_EQ,   // ==
    SY_NE,   // !=
    SY_GT,   // >
    SY_GE,   //  >=
    SY_LT,    //  <
    SY_LE,    //  <=
    SY_AND,    // &&
    SY_OR,  // ||


    SY_COMMA,  // , 
    SY_SEMICOLON,   // ;
    SY_ADDRESS,  //&
    SY_PERCENT,  //%
    SY_LPAREN,   //  (
    SY_RPAREN,    // ) 
    SY_LBRACE,   // {  
    SY_RBRACE,  //  } 
    SY_LBRACKET,   // [  
    SY_RBRACKET,  // ] 

    //关键字
    KW_INT,   //int
    KW_CHAR, //char
    KW_CONST, //const
    KW_MAIN, //main
    KW_VOID, // void
    KW_IF,    //if
    KW_ELSE,  //else
    KW_TRUE,   //true
    KW_FALSE,   //false
    KW_WHILE,  //while
    KW_NULL,   //null
    KW_RETURN,   //return
    KW_SCANF,
    KW_PRINTF,


    /*
     *字面值 : 整数  字符串 字符
     *
     */
    CONST_INT,    //32位整形
    CONST_CHAR,    //8位字符型
    CONST_STRING    //字符串常量
};


//符号表类型
enum symbolType{
    st_funcType,
    st_localType    
};


//funcitem_函数返回值类型
enum funcReturnType{
    frt_voidType,
    frt_charType,
    frt_intType
};


/*localItem 
  大体上分为3种:常量  变量  参数
  每一种类又分为  整形(数组)  字符(数组)   
  */
//localItem 种类  数据类型直接通过函数重载实现
enum localMold{
    lm_constant,    //常量
    lm_variable,    //变量
    lm_parameter     //参数 
};

//localItem 类型检查
enum itemType{
    it_arrayType,  //数组类型
    it_charType,  //字符类型
    it_intType,  //整形
    it_stringType   //字符串
};


//语法分析 表达式返回值
struct exprRet {
    std::string name;  //临时变量
    itemType it;
    int value;
    char cvalue;
    bool isEmpty;
    bool isconstant;
};
