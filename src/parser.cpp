#include "../include/parser.h"
#include "../include/utils.h"



/*
   构造函数  列表初始化成员关键字 
 */
Parser::Parser(const std::string &fileName) : keywords({
        {"main", KW_MAIN},
        {"void", KW_VOID},
        {"const", KW_CONST},
        {"if", KW_IF}, 
        {"while", KW_WHILE}, 
        {"else", KW_ELSE},
        {"true", KW_TRUE},
        {"false", KW_FALSE},
        {"return", KW_RETURN},
        {"int", KW_INT},
        {"char", KW_CHAR},
        {"null", KW_NULL,},
        {"scanf", KW_SCANF},
        {"printf", KW_PRINTF} 
        }){
    fs.open(fileName);
    if(!fs.is_open() && formatC(fileName)) {    //输入以.c结尾的文件
        panic("ParserError: Please open files with .c\n");
    } 
}

/*
   预处理器   格式：  #include <file.h>
 */
std::string Parser::preprocessors() {
    std::string filename = "";
    std::string tmp = "";
    char ctr = peekNextChar();
    while(ctr != 'i' && ctr != '\n') {
        getNextChar(); 
        ctr = peekNextChar();
    }
    if(ctr == '\n') panic("SynaxError: wrong filequote 2 at line %d, column %d", line, column);

    //include
    for(int i = 0; i < 7; i ++) {
        ctr = getNextChar(); 
        tmp += ctr; 
    }

    if(tmp != "include") panic("SynaxError: wrong filequote 1 at line %d, column %d", line, column);
    else {
        ctr = peekNextChar(); 
        while(ctr != '<' && ctr != '\n') {
            getNextChar();
            ctr = peekNextChar(); 
        } 
        if(ctr == '\n') panic("SynaxError: wrong filequote 2 at line %d, column %d", line, column);
        getNextChar();
        ctr = getNextChar();
        while(ctr != '>') {
            filename +=  ctr;
            ctr = getNextChar();
        }    
    }
    return filename;
}



/*
   词法分析器
 */
std::tuple<P_Token, std::string>  Parser::next(){


    char ctr = getNextChar();
    if(ctr == EOF) return std::make_tuple(TK_EOF, "eof");

    //消除空格和换行
    while(ctr == ' ' || ctr == '\n' || ctr == '\t' || ctr == '\r') {
        if(ctr == '\n' || ctr == '\r') {
            line++;
            column = 0; 
        }
        ctr = getNextChar(); 
        if(ctr == EOF) return std::make_tuple(TK_EOF, "eof");   //eof+换行
    }


    /*字面值*/

    //整形
    if(ctr >= '0' && ctr <= '9') {
        char ctrn = peekNextChar(); 
        std::string num = "";
        num += ctr; 
        while(ctrn >= '0' && ctrn <= '9') {
            ctr = getNextChar(); 
            num += ctr; 
            ctrn = peekNextChar();
        }
        return std::make_tuple(CONST_INT, num);
    }
    //字符串型
    if(ctr == '\"') {
        char ctrn = peekNextChar(); 
        std::string num = ""; 
        while(ctrn != '\"') {
            ctr = getNextChar(); 
            num += ctr; 
            ctrn = peekNextChar(); 
        }
        getNextChar();   //去掉 “
        return std::make_tuple(CONST_STRING, num); 
    }
    //字符型
    if(ctr == '\'') {
        ctr = getNextChar();
        std::string num = "";
        num += ctr;
        if(getNextChar() != '\'') panic("SynaxError: wrong char format at line %d, column %d", this->line, this->column);
        return std::make_tuple(CONST_CHAR, num);
    }


    /*标识符 关键字*/
    if((ctr >= 'a' && ctr <= 'z') || (ctr >= 'A' && ctr <= 'Z')) {
        std::string num = "";
        num += ctr;
        char ctrn = peekNextChar();    
        while((ctrn >= 'a' && ctrn <= 'z') || (ctrn >= 'A' && ctrn <= 'Z')
                || (ctr >= '0' && ctr <= '9')) {
            ctr = getNextChar();        
            num += ctr; 
            ctrn = peekNextChar(); 
        } 
        auto result = keywords.find(num);
        return (result != keywords.end()) ? std::make_tuple(result->second, num)  : std::make_tuple(TK_IDENT, num);
    }


    /*内置符号*/


    //预处理器   格式：  #include <file.h>
    if(ctr == '#') {
        std::string name = preprocessors(); 
        return std::make_tuple(TK_FILENAME, name); 
    }


    //普通内置符号
    if(ctr == '+') return std::make_tuple(SY_PLUS, "+");
    if(ctr == '-') return std::make_tuple(SY_MINUS, "-");
    if(ctr == '*') return std::make_tuple(SY_TIMES, "*");
    if(ctr == '/') return std::make_tuple(SY_DEV, "/");
    if(ctr == ',') return std::make_tuple(SY_COMMA, ",");
    if(ctr == ';') return std::make_tuple(SY_SEMICOLON, ";");
    if(ctr == '(') return std::make_tuple(SY_LPAREN, "(");
    if(ctr == ')') return std::make_tuple(SY_RPAREN, ")");
    if(ctr == '[') return std::make_tuple(SY_LBRACKET, "[");
    if(ctr == ']') return std::make_tuple(SY_RBRACKET, "]");
    if(ctr == '{') return std::make_tuple(SY_LBRACE, "{");
    if(ctr == '}') return std::make_tuple(SY_RBRACE, "}");
    if(ctr == '%') return std::make_tuple(SY_PERCENT, "%");
    if(ctr == '&') return std::make_tuple(SY_ADDRESS, "&");

    if(ctr == '=') {
        char ctrn = peekNextChar();
        if(ctrn == '=')  {
            ctr = getNextChar(); 
            return std::make_tuple(SY_EQ, "==");
        }else {
            return std::make_tuple(SY_ASSIGN, "="); 
        }
    }

    if(ctr == '>') {
        char ctrn = peekNextChar();  
        if(ctrn == '=') {
            ctr = getNextChar();
            return std::make_tuple(SY_GE, ">="); 
        }else {
            return std::make_tuple(SY_GT, ">"); 
        }
    }

    if(ctr == '<') {
        char ctrn = peekNextChar(); 
        if(ctrn == '=') {
            ctr = getNextChar();
            return std::make_tuple(SY_LE, "<="); 
        } else {
            return std::make_tuple(SY_LT, "<"); 
        }
    }

    if(ctr == '!') {
        char ctrn = peekNextChar();
        if(ctrn == '=') {
            ctr = getNextChar(); 
            return std::make_tuple(SY_NE, "!=");
        }else {
            panic("SynaxError: wrong NE format at line %d, column %d", line, column);
        }
    }


    panic("SynaxError: unknow token: %c", ctr);
    return std::make_tuple(INVALD, "invalid");
}




void Parser::printLex() {

    std::tuple<P_Token, std::string> s;
    do{
        s = next(); 
        std::cout << "[" << std::get<0>(s) << ", " <<  std::get<1>(s) << "]"  << std::endl;
    }while(std::get<0>(s) != TK_EOF);
}


/*语法分析器*/
//入口函数
void Parser::parse() {

    std::cout << "SyntaxAnalysis start..." << std::endl << std::endl;
    currentToken = next();
    //currentLexeme = TK_FILENAME;   预处理直接忽略
    if(getCurrentToken() == TK_EOF) {
        std::cout << "Nothing to parse!" << std::endl;
        return ;
    }else {
        //预处理器
        while(getCurrentToken() == TK_FILENAME) {
            std::string filename = getCurrentLexeme();    
            std::cout << "Preprocessors: #include <" << filename << ">" << std::endl; 
            currentToken = next();
        }
        //解析
        if(Parse_procedure())  return;   
    }
}

//<程序> ::= [<常量说明>][<变量说明>][<函数定义部分>]<主函数>
bool Parser::Parse_procedure() {

    //全局常量声明
    Parse_constDeclaration("Global");
    //全局变量说明
    Parse_varDeclaration(true,"Global");
    //检查函数定义
    Parse_functionDefinition();

    //在函数定义部分一定会识别到int main再返回 因此currentToken就是main
    /*解析主函数main*/
    if(getCurrentToken() != KW_MAIN) {
        panic("SyntaxError: symbol _main can't be found at line %d, column %d", line, column);
    }
    //接着分析
    currentToken = next();  // (
    if(getCurrentToken() != SY_LPAREN)
        panic("SyntaxError: main lack ( at line %d, column %d", line, column);
    currentToken = next();    //  )  
    if(getCurrentToken() != SY_RPAREN)  
        panic("SyntaxError: main lack ) at  line %d, column %d", line, column);
    currentToken = next();    // {
    if(getCurrentToken() != SY_LBRACE) 
        panic("SyntaxError: main lack { at line %d, column %d", line, column);


    //复合语句
    FourYuanItem four;
    four.type = FunctionDef;
    four.FuncType = VoidType;
    four.target = "main";
    Parse_compoundStmt("main");


    // } 
    if(getCurrentToken() != SY_RBRACE)
        panic("SyntaxError: main lack } at line %d, column %d", line, column);

    return true;  //分析正常结束
}

//<常量说明> ::= const<常量定义>;{const<常量定义>;}
bool Parser::Parse_constDeclaration(std::string funcName) {

    //const
    if(getCurrentToken() != KW_CONST)  return false;
    currentToken = next();
    //解析<常量定义>
    Parse_constDefinition(funcName);

    //  ;
    if(getCurrentToken() != SY_SEMICOLON)
        panic("SyntaxError: constDeclaration lack semicolon at line %d, column %d", line, column);

    while(true) {   //  处理右递归

        currentToken = next();
        if(getCurrentToken() != KW_CONST) break;  //正常break

        currentToken = next();
        Parse_constDefinition(funcName);

        //  ;
        if(getCurrentToken() != SY_SEMICOLON)
            panic("SyntaxError: constDeclaration lack semicolon at line %d, column %d", line, column);

    }
    return true;
}


/*<常量定义> ::= int<标识符>=<整数>{,<标识符>=<整数>}
  | char<标识符>=<字符{,<标识符>=<字符>}>*/
bool Parser::Parse_constDefinition(std::string funcName) {
    std::string id;
    id = funcName;  //忽略
    if(getCurrentToken() == KW_INT) {  //int
        currentToken = next();
        if(getCurrentToken() != TK_IDENT) {
            panic("SyntaxError: const definition not complete at line %d, column %d", line, column);
            return false;
        }
        id = getCurrentLexeme();

        currentToken = next();
        if(getCurrentToken() != SY_ASSIGN) {   // = 
            panic("syntaxError: const definition not complete at line %d, columnn %d", line, column); 
            return false; 
        }

        //整数
        currentToken = next();
        Parse_integer();
        /*填充符号表*/

        //解析右递归
        while(true) {
            if(getCurrentToken() != SY_COMMA) break;  //正常退出

            currentToken = next();
            if(getCurrentToken() != TK_IDENT)   //标识符
                panic("SyntaxError: const definition not complete at line %d, column %d", line, column);
            id = getCurrentLexeme();

            currentToken = next();
            if(getCurrentToken() != SY_ASSIGN)  // = 
                panic("syntaxError: const definition not complete at line %d, columnn %d", line, column); 

            currentToken  = next();
            Parse_integer();  //整数
            /*填充符号表*/ 
        }

    }else if(getCurrentToken() == KW_CHAR){   //char
        currentToken = next();
        if(getCurrentToken() != TK_IDENT)  {
            panic("SyntaxError: const definition nont complete at line %d, column %d", line, column); 
            return false;
        }       
        id = getCurrentLexeme();

        currentToken = next();
        if(getCurrentToken() != SY_ASSIGN) {  // = 
            panic("SyntaxError: const definition not complete at line %d, column %d", line, column); 
            return false;
        }

        currentToken = next();
        if(getCurrentToken() != CONST_CHAR)   // 字符
            panic("SyntaxError: const definition not complete at line %d, column %d", line, column); 

        /*填充符号表*/

        //解析右递归
        while(true) {
            if(getCurrentToken() != SY_COMMA)  break;   //正常退出

            currentToken = next();
            if(getCurrentToken() != TK_IDENT)   //标识符
                panic("SyntaxError: const definition not complete at line %d, column %d", line, column); 
            id = getCurrentLexeme();

            currentToken = next();
            if(getCurrentToken() != SY_ASSIGN) // = 
                panic("SyntaxError: const definition not complete at line %d, column %d", line, column); 

            currentToken = next();
            if(getCurrentToken() == CONST_CHAR)  //字符
                panic("SyntaxError: const definition not complete at line %d, column %d", line, column); 

            /*填充符号表*/
        }

    }else {
        panic("SyntaxError: const definition not complete at line %d, column %d", line, column); 
        return false;
    }
    return true;
}


//<变量说明> ::= <变量定义>;{<变量定义>;}
bool Parser::Parse_varDeclaration(bool isGlobal, std::string funcName) {

    //在main之前定义   函数 &&  全局变量 && 全局数组
    if(isGlobal) {
        currentToken = next();
        if(getCurrentToken() == KW_INT || getCurrentToken() == KW_CHAR || getCurrentToken() == KW_VOID) {
            overallSymbol.type = getCurrentToken(); 
            currentToken = next();
            if(getCurrentToken() == KW_MAIN) return false;  //解析主函数
            if(getCurrentToken() == TK_IDENT) {   //全局变量 && 全局数组
                overallSymbol.Name  = getCurrentLexeme();   
                currentToken = next();
                if(getCurrentToken() == SY_LPAREN) {
                    return false;                //解析函数
                }
            }
        }    
    }

    //解析全局变量   全局数组
    if(!Parse_varDefinition(funcName)) return false;

    currentToken = next();
    if(getCurrentToken() != SY_SEMICOLON)  //  ;
    panic("SyntaxError: varDeclration not complete at line %d, column %d", line, column);

    //处理右递归
    while(true){
        if(isGlobal) {
            currentToken = next();
            if(getCurrentToken() == KW_INT || getCurrentToken() == KW_CHAR || getCurrentToken() == KW_VOID) {
                overallSymbol.type = getCurrentToken();
                currentToken = next();
                if(getCurrentToken() == KW_MAIN)  return false;         //解析主函数
                if(getCurrentToken() == TK_IDENT) {  //全局变量 && 全局数组
                    overallSymbol.Name = getCurrentLexeme();
                    currentToken = next();
                    if(getCurrentToken() == SY_LPAREN){
                        return false;    //解析函数
                    }
                }
            }
        } 
        if(!Parse_varDefinition(funcName))  return false;

        currentToken = next();
        if(getCurrentToken() != SY_SEMICOLON) //  ;
        panic("SyntaxError: varDeclration not complete at line %d, column %d", line, column);
    }

    return true;
}


/*<变量定义> ::= <类型标识符>(<标识符> | <标识符>'['<无符号整数>']')
  {, (<标识符> | <标识符> '['<无符号整数>']')} */
bool Parser::Parse_varDefinition(std::string funcName) {

    std::string id;
    int length, num = 0;
    if(getCurrentToken() == SY_LBRACKET) { //  [  全局数组

        currentToken = next(); 
        if(getCurrentToken() != CONST_INT) {
            panic("SyntaxError: varDefinition not complete at line %d, column %d", line, column); 
            return false; 
        }else if(getCurrentToken() == CONST_INT) {
            std::string strnum = getCurrentLexeme(); 
            for(unsigned int i = 0; i < strnum.length(); i ++) {
                num += strnum[i] - '0';  
                num *= 10;  
            }         
            num /= 10;
            if(num == 0)  
                panic("SyntaxError: elements of array must be positive at line %d, column %d", line, column);
            length = num;   //数组长度
        }
    }

    while(true) {

        // ,
        currentToken = next(); 
        if(getCurrentToken() != SY_COMMA) break;

        //标识符
        currentToken = next();
        if(getCurrentToken() != TK_IDENT) {
            panic("SyntaxError: varDefiniton lack ident at line %d, columne %d", line, column); 
            break;
        }
        id = getCurrentLexeme();

        currentToken = next();  // [
        if(getCurrentToken() != SY_LBRACKET) {
            currentToken = next();
            if(getCurrentToken() != CONST_INT) {
                panic("SyntaxError: varDefinition not complete at line %d, column %d", line, column); 
                return false; 
            }else if(getCurrentToken() == CONST_INT) {
                std::string strnum = getCurrentLexeme(); 
                for(unsigned int i = 0; i < strnum.length(); i ++) {
                    num += strnum[i] - '0'; 
                    num *= 10; 
                } 
                num /= 10;
                if(num == 0) 
                    panic("SyntaxError: elements of array must be positive at line %d, column %d", line, column);
                length = num;   //数组长度
            }
        }
    }
    return true;
}


/*目前静态区域只允许定义一个函数*/
//<函数定义部分> ::= {<有返回值函数定义> | <无返回值函数定义>}
bool Parser::Parse_functionDefinition() {

    if(overallSymbol.type == KW_INT || overallSymbol.type == KW_CHAR) {  //因为全局变量解析一定会被执行，所以overallSymbol一定会被赋值
        Parse_haveReturnFuncDefinition();  
    }else if(overallSymbol.type == KW_VOID) {
        Parse_noReturnFuncDefinition(); 
    }else {
        return false; 
    }

    return true;
}


//<有返回值函数定义> ::= <声明头部>'('<参数表>')''{'<复合语句>'}'
bool Parser::Parse_haveReturnFuncDefinition() {

    //声明头部
    Parse_FunctionDeclarHead();

    //参数
    currentToken = next();
    if(getCurrentToken() != SY_RPAREN) {
        Parse_paraList(overallSymbol.Name); 
    }

    // ) 
    currentToken = next();
    if(getCurrentToken() != SY_RPAREN) {
        panic("SyntaxError: expects ) at line %d, column %d", line, column);
        return false;
    }

    //  {
    currentToken = next();
    if(getCurrentToken() != SY_LBRACE){
        panic("SyntaxError: lack compound in func at line %d, column %d", line, column);    
    }

    //复合语句
    Parse_compoundStmt(overallSymbol.Name);

    // } 
    currentToken = next();
    if(getCurrentToken() != SY_RBRACE) {
        panic("SyntaxError: funcDefiniton expects } at line %d, column %d", line, column); 
    }
    return true;
}


//<无返回值函数定义> ::= void<标识符>'('<参数表>')''{'<复合语句>'}'
bool Parser::Parse_noReturnFuncDefinition() {

    //参数
    currentToken = next();
    if(getCurrentToken() != SY_RPAREN) {
        panic("SyntaxError: void not need paralist at line %d, column %d", line, column); 
        return false; 
    }

    // )
    currentToken  = next();
    if(getCurrentToken() != SY_RPAREN) {
        panic("SyntaxError: expects ) at line %d, column %d", line, column);
        return false;
    }


    // {
    currentToken = next();
    if(getCurrentToken() == SY_LBRACE) {
        panic("SyntaxError: lack compound in func at line %d, column %d", line, column);    
    }

    //复合语句
    Parse_compoundStmt(overallSymbol.Name);


    // }
    currentToken = next(); 
    if(getCurrentToken() != SY_RBRACE) {
        panic("SyntaxError: funcDefiniton expects } at line %d, column %d", line, column); 
    }
    return true;
}


//<参数表> ::= <类型标识符><标识符>{,<类型标识符><标识符>}
bool Parser::Parse_paraList(std::string funcName) {

    //类型标识符  int | char
    currentToken = next();
    if(getCurrentToken() != KW_INT && getCurrentToken() != KW_CHAR) {
        panic("SyntaxError: paralist error at line %d, column %d", line, column);
        return false; 
    }

    //标识符
    currentToken = next();
    if(getCurrentToken() != TK_IDENT) {
        panic("SyntaxError: paralist error at line %d, column %d", line, column);
        return false;
    }


    while(true) {
        currentToken = next();
        if(getCurrentToken() != SY_COMMA)  break;

        currentToken = next();
        if(getCurrentToken() != KW_INT && getCurrentToken() != KW_CHAR) {
            panic("SyntaxError: paralist error at line %d, column %d", line, column);
            return false;
        }


        currentToken = next();
        if(getCurrentToken() != TK_IDENT){
            panic("SyntaxError: paralist error at line %d, column %d", line, column);
            return false; 
        }
    }

    return true;
}


//<复合语句> ::= [<常量说明>][<变量说明>]{<语句>}
bool Parser::Parse_compoundStmt(std::string funcName) {

    Parse_constDeclaration(funcName);
    Parse_varDeclaration(false, funcName);
    std::vector<FourYuanItem> noUseCache;
    while(true) {
        if(!Parse_Stmt(funcName, false, noUseCache, 1))    //初始值为1
               break;     
    }
    return true;
}



//<表达式> ::= [ + | -]<项>{<加法运算符><项>}
ExpressionRetValue Parser::Parse_expression(std::string funcName, bool isCache, std::vector<FourYuanItem> & cache, int weight) {

    bool isSure = false;
    int expResult= 0;
    ValueType type;
    std::vector<PostfixItem> tar, obj;
    ExpressionRetValue returnValue;


    //[+ | -]
    currentToken = next();
    if(getCurrentToken() == SY_PLUS || getCurrentToken() == SY_MINUS)  {
        //生成代码相关 
    
    }

    Parse_item(tar, funcName, isCache, cache, weight);
    while(true) {
        currentToken = next(); 
        if(getCurrentToken() == SY_PLUS || getCurrentToken() == SY_MINUS) {
            //生成代码相关 
        } 
    
        Parse_item(tar, funcName, isCache, cache, weight); 
    }

    //表达式计算

    return returnValue;
}


//<项> ::= <因子>{<乘法运算符><因子>}
bool Parser::Parse_item(std::vector<PostfixItem> &obj, std::string funcName, bool isCache, std::vector<FourYuanItem> &cache, int weight) {

    Parse_factor(obj, funcName, isCache, cache, weight);

    while(true) {
        currentToken = next(); 
        if(getCurrentToken() == SY_TIMES || getCurrentToken() == SY_DEV) {
        
        
        } 
        //因子
        Parse_factor(obj, funcName, isCache, cache, weight);
    }

    return true;
}


//<因子> ::= <标识符>['('<值参数表>')'] | <标识符> '['<表达式>']' | '('<表达式>')' | <整数> | <字母>
bool Parser::Parse_factor(std::vector<PostfixItem> &obj, std::string funcName, bool isCache, std::vector<FourYuanItem> &cache, int weight) {
    PostfixItem item;
    ExpressionRetValue term2;
    FourYuanItem term3;
    std::string id;

    currentToken = next();
    switch (getCurrentToken()) {
        case TK_IDENT:
            



            break;
        case SY_PLUS:
        case SY_MINUS:
            break;

        default:
            
    }

}



/*<语句> ::= <条件语句> | <循环语句> | ’{‘{<语句>}'}' | <标识符>['('<值参数表>')'];
        | <赋值语句>; | <读语句>; | <写语句>; | ; | <返回语句>;  */
bool Parser::Parse_Stmt(std::string funcName, bool isCache, std::vector<FourYuanItem> &cache, int weight) {

    FourYuanItem four;
    four.type = FunctionCall;

    currentToken = next();
    switch (getCurrentToken()) {
        case KW_IF:
            Parse_conditionStmt(funcName, isCache, cache, weight); 
            break;
        case KW_WHILE:
            Parse_loopStmt(funcName, isCache, cache, weight);
            break;
        case SY_LBRACE:
            while(true) {
                if(!Parse_Stmt(funcName, isCache, cache, weight)) 
                    break;
            } 
            currentToken = next();
            if(getCurrentToken() != SY_RBRACE) 
                panic("SyntaxError: Statement lack } at line %d, column %d", line, column);
            break;
        case TK_IDENT:
            
            break;
            case 
        default:
            
    }


    return true;
}





//<赋值语句> ::= <标识符>=<表达式> | <标识符>'['<表达式>']'=<表达式>
//实际分析的是  = <表达式> | '['<表达式>']'=<表达式>
bool Parser::Parse_assignStmt(std::string funcName, std::string id, bool isCache, std::vector<FourYuanItem>& cache, int weight) {



}




//<条件语句> ::= if'('<条件>')'<语句>else <语句>
bool Parser::Parse_condition(std::string funcName, bool isCache, std::vector<FourYuanItem> &cache, int weigth) {




}


//<条件> ::= <表达式><关系运算符><表达式> | <表达式>
//<关系运算符> ::= < | <= | > | >= | != | ==
std::string Parser::Parse_condition(std::string funcName, bool isCache, std::vector<FourYuanItem> &cache, int weight) {






}



//<循环语句> ::= while'('<条件>')'<语句>
bool Parser::Parse_loopStmt(std::string funcName, bool isCache, std::vector<FourYuanItem>& cache, int weight) {



}




//<值参数表> ::= <表达式>{,<表达式>}
std::vector<ValueType> Parser::Parse_valueParamList(std::string funcName, bool isCache, std::vector<FourYuanItem>& cache, int weight) {




}


//<读语句> ::= scanf'('<标识符>{,<标识符>}')'
bool Parser::Parse_scanf(std::string funcName, bool isCache, std::vector<FourYuanItem>&cache, int weight) {




}


//<写语句> ::= printf'('<字符串>,<表达式>')' | printf'('<字符串>')' | printf '('<表达式>')'
bool Parser::Parse_printf(std::string funcName, bool isCache, std::vector<FourYuanItem> &cache, int weight) {




}




//<返回语句> ::= return ['('<表达式>')']
bool Parser::Parse_returnStmt(std::string funcName, bool isCache, std::vector<FourYuanItem> &cache, int weight) {




}



//<整数> ::= [+ | -]<无符号整数> | 0
//注意。0前面不能有任何正负号
bool Parser::Parse_integer() {



}


//<声明头部> ::= int<标识符> | char <标识符>
bool Parser::Parse_FunctionDeclarHead() {



}





