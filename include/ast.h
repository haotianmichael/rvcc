#pragma  once


enum TOKEN{

    /*
     * 词法分析器返回的单词类别:
     * 标识符
     * 关键字
     * 内置符号
     * */
    INVALD = 0,   //<invalid>


    /*
     * 标识符
     * 用字符串常量直接表示
     */
    TK_IDENT,  

    /*
     * 内置符号 
     * 支持的运算比较少： 加减乘除  赋值   关系比较符  特殊符号
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
    SY_LPAREN,   //  (
    SY_RPAREN,    // ) 
    SY_LBRACE,   // {  
    SY_RBRACE,  //  } 
    SY_LBRACKET,   // [  
    SY_RBRACKET,  // ] 
    SY_EOF,    //eof

    //关键字
    KW_INT,   //int
    KW_CHAR, //char
    KW_IF,    //if
    KW_ELSE,  //else
    KW_TRUE,   //true
    KW_FALSE,   //false
    KW_WHILE,  //while
    KW_NULL,   //null
    KW_RETURN,   //return
};
