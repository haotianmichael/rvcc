#pragma  once
#include <tuple>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "lex.h"


class Parser
{
    public:
        explicit Parser (const std::string &fileName);
        virtual ~Parser () {}
        void printLex();   //词法分析测试函数
        void printParser();  //语法分析器测试函数  
    private:
        //语法分析器    
        void parse();
        //当前Token
        std::tuple<P_Token, std::string> currentToken;
        //全局解析函数/变量/数组名称(解析main之前的函数定义&&全局变量定义)
        struct { //在全局解析中一定会被填
            std::string Name;
            P_Token type;
        }overallSymbol;
        //得到当前Token的Token
        inline P_Token getCurrentToken() const {
            return std::get<0>(currentToken); 
        }
        //得到当前Token的词素
        inline std::string getCurrentLexeme() {
            return std::get<1>(currentToken); 
        }

    public:
        /*递归下降子程序定义的分析过程 
          语法分析，直接生成中间代码——四元式 
         */
        //<程序> ::= [<常量说明>][<变量说明>][<函数定义部分>]<主函数>
        bool Parse_procedure();
        //<常量说明> ::= const<常量定义>;{const<常量定义>;}
        bool Parse_constDeclaration();
        /*<常量定义> ::= int<标识符>=<整数>{,<标识符>=<整数>}
          | char<标识符>=<整数>{,<标识符>=<整数>}*/
        bool Parse_constDefinition();
        //<变量说明> ::= <变量定义>;{<变量定义>;}
        bool Parse_varDeclaration(bool isGlobal);
        /*<变量定义> ::= <类型标识符>(<标识符>|<标识符>'['<无符号整数>']')
            {,(<标识符> | <标识符> '['<无符号整数>']')}*/
        bool Parse_varDefinition();
        //<函数定义> ::= {<无返回值定义> | <有返回值函数>}
        bool Parse_functionDefinition();
        //<有返回值函数定义> ::= <声明头部>'('<参数表>')''{'<复合语句>'}'
        bool Parse_haveReturnFuncDefinition();
        //<无返回值函数定义> ::= void'('<参数表>')''{'<复合语句>'}'
        bool Parse_noReturnFuncDefinition();
        //<参数表> ::= <类型标识符><标识符>{,<类型标识符><标识符>}
        bool Parse_paraList();
        //<声明头部> ::= int <标识符> | char <标识符>
        bool Parse_FunctionDeclarHead();


        //<复合语句> ::= [<常量说明>][<变量说明>]{<语句>}
        bool Parse_compoundStmt();
       /*<语句> ::= <条件语句> | <循环语句> | <写语句>; | <标识符>['('<值参数表>')'];
            | <赋值语句>; | <读语句>; | <写语句>; | <空>; |  <返回语句>*/
        bool Parse_Stmt();
        //<赋值语句> ::= <标识符> = <表达式> | <标识符>'['<表达式>']'=<表达式>
        bool Parse_assignStmt();
        //<条件语句> ::= if'('<条件>')'<语句>else<语句>
        bool Parse_conditionStmt();
        //<条件> ::= <表达式><关系运算符><表达式> | <表达式>
        bool Parse_condition();
        //<循环语句> ::= while'('<条件>')'<语句>
        bool Parse_loopStmt();
        //<值参数表> ::= <表达式>{, <表达式>}
        bool Parse_valueParamList();

        //<表达式> ::= [+ | -]<项>{<加法运算符><项>}
        bool Parse_expression();
        //<项> ::= <因子>{<乘法运算符><因子>}
        bool Parse_item(); 
        //<因子> ::= <标识符>['('<值参数表>')']|<标识符>'['<表达式>']'|'('<表达式>')'|<整数>|<字符>
        bool Parse_factor();

        //<读语句> ::= scanf‘(’<标识符>{,<标识符>}')'
        bool Parse_scanf();
        //<写语句> ::= printf'('<字符串>,<表达式>')' | printf'('<字符串>')' | prinntf'('<表达式>')'
        bool Parse_printf();
        //<返回语句> ::= return['('<表达式>')']
        bool Parse_returnStmt();
        //<整数> ::= [ + | -]<无符号整数> | 0
        bool Parse_integer();

    private:
        //词法分析器  输出<词素, 值>
        std::tuple<P_Token, std::string> next(); 
        //预处理器
        std::string preprocessors();
        //哈希表存储关键字  
        const std::unordered_map<std::string, P_Token> keywords; 

        //文件输入流
        std::fstream fs;
        //当前第一行第零列
        int column = 1, line = 1;
        //读入下一个字符 并消耗
        inline char getNextChar() { column++; return static_cast<char>(fs.get());}
        //读入下一个字符 不消耗
        inline char peekNextChar() {return static_cast<char>(fs.peek());}
};
