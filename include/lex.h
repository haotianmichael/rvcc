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

    

    /*
     *字面值 : 整数  字符串 字符
     *
     */
    CONST_INT,    //32位整形
    CONST_CHAR,    //8位字符型
    CONST_STRING    //字符串常量
};



//表达式返回值
struct ExpressionRetValue{

    std::string name;   //表达式返回的名字---> 统一规定。表达式都需需要一个反汇变量名
    bool isSurable;   //是否确定
//    ValueType type;  //值类型
    int number;
    char character;
    bool isEmpty;   //是否是空的
};


//情况子语句返回
struct SubCaseRet{
    int constValue;
    bool recognize;
    std::string label;
};


//标识符类型
enum ItemType{
    Constant,  //常量
    Variable,  //变量
    Function,  //函数
    Parament   //参数
};

//数据类型
enum ValueType{
    IntType,
    CharType,
    StringType
};

//函数类型
enum ReturnFucType{
    VoidType,
    ReturnIntType,
    ReturnCharType 
};






