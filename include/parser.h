#pragma  once
#include <tuple>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "lex.h"
#include "intermediateGenerator.h"
#include "symbolTable.h"

const int ERROR_CODE  = -2147483648;  //错误码

class Parser
{
    public:
        explicit Parser(const std::string &fileName);
        virtual ~Parser(){}
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
            funcReturnType  type;
        }overallSymbol;
        //得到当前Token的Token
        inline P_Token getCurrentToken() const {
            return std::get<0>(currentToken); 
        }
        //得到当前Token的词素
        inline std::string getCurrentLexeme() {
            return std::get<1>(currentToken); 
        }

    private:
        /*符号表*/
        SymbolTable *__symbolTable;

    public:
        SymbolTable* getSymbolTable() {return this->__symbolTable;}

    public:
        /*递归下降子程序定义的分析过程 
          语法分析，直接生成中间代码——四元式 
          */
        //<程序> ::= [<常量说明>][<变量说明>][<函数定义部分>]<主函数>
        bool Parse_procedure();
        //<常量说明> ::= const<常量定义>;{const<常量定义>;}
        bool Parse_constDeclaration(std::string scope);
        /*<常量定义> ::= int<标识符>=<整数>{,<标识符>=<整数>}
          | char<标识符>=<整数>{,<标识符>=<整数>}*/
        bool Parse_constDefinition(std::string scope);
        //<变量说明> ::= <变量定义>;{<变量定义>;}
        bool Parse_varDeclaration(bool isGlobal, std::string scope);
        /*<变量定义> ::= <类型标识符>(<标识符>|<标识符>'['<无符号整数>']')
          {,(<标识符> | <标识符> '['<无符号整数>']')}*/
        bool Parse_varDefinition(std::string scope);
        //<函数定义> ::= {<无返回值定义> | <有返回值函数>}
        bool Parse_functionDefinition();
        //<有返回值函数定义> ::= <声明头部>'('<参数表>')''{'<复合语句>'}'
        bool Parse_haveReturnFuncDefinition();
        //<无返回值函数定义> ::= void'('<参数表>')''{'<复合语句>'}'
        bool Parse_noReturnFuncDefinition();
        //<参数表> ::= <类型标识符><标识符>{,<类型标识符><标识符>}
        bool Parse_paraList(std::string scope);
        //<声明头部> ::= int <标识符> | char <标识符>
        bool Parse_functionDeclarHead();


        //<复合语句> ::= [<常量说明>][<变量说明>]{<语句>}
        bool Parse_compoundStmt(std::string scope);
        /*<语句> ::= <条件语句> | <循环语句> | <写语句>; | <标识符>['('<值参数表>')'];
          | <赋值语句>; | <读语句>; | <写语句>; | <空>; |  <返回语句>*/
        bool Parse_Stmt(std::string scope);
        //<赋值语句> ::= <标识符> = <表达式> | <标识符>'['<表达式>']'=<表达式>
        bool Parse_assignStmt(std::string scope, std::string name);
        //<条件语句> ::= if'('<条件>')'<语句>else<语句>
        bool Parse_conditionStmt(std::string scope);
        //<条件> ::= <表达式><关系运算符><表达式> | <表达式>
        std::string Parse_condition(std::string scope);
        //<循环语句> ::= while'('<条件>')'<语句>
        bool Parse_loopStmt(std::string scope);
        //<值参数表> ::= <表达式>{, <表达式>}
        std::vector<itemType> Parse_valueParamList(std::string scope);

        //<表达式> ::= [+ | -]<项>{<加法运算符><项>}
        exprRet Parse_expression(std::string scope);
        //<项> ::= <因子>{<乘法运算符><因子>}
        bool Parse_item(std::string scope, std::vector<PostfixExpression> &pfeList); 
        //<因子> ::= <标识符>['('<值参数表>')']|<标识符>'['<表达式>']'|'('<表达式>')'|<整数>|<字符>
        bool Parse_factor(std::string scope, std::vector<PostfixExpression> &preList);
        //<读语句> ::= scanf‘(’<标识符>{,<标识符>}')'
        bool Parse_scanf(std::string scope);
        //<写语句> ::= printf'('<字符串>,<表达式>')' | printf'('<字符串>')' | prinntf'('<表达式>')'
        bool Parse_printf(std::string scope);
        //<返回语句> ::= return['('<表达式>')']
        bool Parse_returnStmt(std::string scope);
        //<整数> ::= [ + | -]<无符号整数> | 0
        int  Parse_integer(std::string value);

    public:
        /*表达式算法*/
        std::vector<PostfixExpression> postfixReverse(std::vector<PostfixExpression> &pfeListBefore);   //中缀表达式转后缀表达式
        exprRet postfixExprTotmpCode(std::vector<PostfixExpression> &);  //表达式中间代码生成
        std::string varGenerator();
        std::string labelGenetar();
        std::string stringGenetar();
        void factor_symbol(int isPre, std::vector<PostfixExpression> &preList);


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
