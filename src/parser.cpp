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
    if(!fs.is_open()) {
        panic("ParserError: can't open files"); 
    }else if(!formatC(fileName)) {
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

    std::cout << "SyntaxAnalysis Start..." << std::endl << std::endl;

    /*初始化符号表*/
    this->__symbolTable = new symbolTable();


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
        std::cout << "###########################Start " << std::endl << std::endl << std::endl;
        //解析
        if(Parse_procedure())  {
            std::cout << std::endl<< std::endl << "SyntaxAnalysis succeeded!..." << std::endl << std::endl;
            return;
        }
    }
}

//<程序> ::= [<常量说明>][<变量说明>][<函数定义部分>]<主函数>
bool Parser::Parse_procedure() {

    /*说明语句----------填符号表*/
    //全局常量声明
    Parse_constDeclaration();
    //全局变量说明
    Parse_varDeclaration(true);
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

    /*执行语句----------查表，生成中间代码*/
    //复合语句
    Parse_compoundStmt();


    //  } 
    if(getCurrentToken() != SY_RBRACE)
        panic("SyntaxError: main lack } at line %d, column %d", line, column);

    return true;  //分析正常结束
}

//<常量说明> ::= const<常量定义>;{const<常量定义>;}
bool Parser::Parse_constDeclaration() {

    std::cout << "Parse_constDeclaration Start..." << std::endl;
    //const
    if(getCurrentToken() != KW_CONST)  return false;
    currentToken = next();
    //解析<常量定义>
    Parse_constDefinition();

    // ;
    if(getCurrentToken() != SY_SEMICOLON)
        panic("SyntaxError: constDeclaration lack semicolon at line %d, column %d", line, column);

    while(true) {   //  处理右递归

        currentToken = next();
        if(getCurrentToken() != KW_CONST) break;  //正常break

        currentToken = next();
        Parse_constDefinition();

        //  ;
        if(getCurrentToken() != SY_SEMICOLON)
            panic("SyntaxError: constDeclaration lack semicolon at line %d, column %d", line, column);

    }
    std::cout  << "Parse_constDeclaration Over..." << std::endl << std::endl;
    return true;
}


/*<常量定义> ::= int<标识符>=<整数>{,<标识符>=<整数>}
  | char<标识符>=<字符{,<标识符>=<字符>}>*/
bool Parser::Parse_constDefinition() {
    std::string id;
    if(getCurrentToken() == KW_INT){  //int
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
            currentToken = next();
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
            panic("SyntaxError: const definition not complete at line %d, column %d", line, column); 
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
            currentToken = next();
            if(getCurrentToken() != SY_COMMA)  break;   //正常退出

            currentToken = next();
            if(getCurrentToken() != TK_IDENT)   //标识符
                panic("SyntaxError: const definition not complete at line %d, column %d", line, column); 
            id = getCurrentLexeme();

            currentToken = next();
            if(getCurrentToken() != SY_ASSIGN) // = 
                panic("SyntaxError: const definition not complete at line %d, column %d", line, column); 

            currentToken = next();
            if(getCurrentToken() != CONST_CHAR)  //字符
                panic("SyntaxError: const definition not complete at line %d, column %d", line, column); 

            /*填充符号表*/
        }

    }else{
        panic("SyntaxError: const definition not complete at line %d, column %d", line, column); 
        return false;
    }
    return true;
}


//<变量说明> ::= <变量定义>;{<变量定义>;}
bool Parser::Parse_varDeclaration(bool isGlobal) {

    std::cout <<  "Parse_varDeclaration Start..." << std::endl;
    //在main之前全局定义   函数 &&  全局变量 && 全局数组
    if(isGlobal) {
        if(getCurrentToken() == KW_INT || getCurrentToken() == KW_CHAR || getCurrentToken() == KW_VOID) {
            overallSymbol.type = getCurrentToken(); 
            currentToken = next();
            if(getCurrentToken() == KW_MAIN) return false;  //解析主函数
            if(getCurrentToken() == TK_IDENT) {   //全局变量 && 全局数组
                overallSymbol.Name  = getCurrentLexeme();   
                currentToken = next();
                if(getCurrentToken() == SY_LPAREN) {
                    std::cout << "Parse_varDeclaration Over..." << std::endl << std::endl;
                    return false;                //解析函数
                }
            }
        }else if(getCurrentToken() == TK_EOF) return false;    //当没有全局声明的时候直接返回   意味着后面没有东西了， 因为全局声明可以检测到main函数的
    }else {   //局部变量  数组
        if(getCurrentToken() == KW_INT || getCurrentToken() == KW_CHAR) {
            overallSymbol.type =  getCurrentToken();    
            currentToken = next();
            if(getCurrentToken() == KW_MAIN) {   //局部不可能出现main
                panic("SyntaxError: duplicate definition of mainn at line %d, column %d", line, column); 
            }else if(getCurrentToken() == TK_IDENT) {   //局部变量  数组
                overallSymbol.Name = getCurrentLexeme(); 
                currentToken = next();
                if(getCurrentToken() == SY_LPAREN) {  //局部不可能出现函数声明
                    panic("SyntaxError: function declaration shows at local area at line %d, column %d", line, column); 
                } 
            }
        }else return false;
    }

    //解析全局变量   全局数组  [    ,    ;
    if(!Parse_varDefinition()) return false;

    if(getCurrentToken() != SY_SEMICOLON)  //  ;
    panic("SyntaxError: varDeclration not complete at line %d, column %d", line, column);

    //处理右递归
    while(true){
        currentToken = next();
        if(isGlobal) {
            if(getCurrentToken() == KW_INT || getCurrentToken() == KW_CHAR || getCurrentToken() == KW_VOID) {
                overallSymbol.type = getCurrentToken();
                currentToken = next();
                if(getCurrentToken() == KW_MAIN)  return false;         //解析主函数
                if(getCurrentToken() == TK_IDENT) {  //全局变量 && 全局数组
                    overallSymbol.Name = getCurrentLexeme();
                    currentToken = next();
                    if(getCurrentToken() == SY_LPAREN){
                        std::cout << "Parse_varDeclaration Over..." << std::endl << std::endl;
                        return false;    //解析函数
                    }
                }
            }else break;
        }else {  //局部变量右递归
            if(getCurrentToken() == KW_INT || getCurrentToken() == KW_CHAR) {
                overallSymbol.type = getCurrentToken(); 
                currentToken = next();
                if(getCurrentToken() == KW_MAIN) {
                    panic("SyntaxError: duplicate definition of main at line %d, column %d", line, column); 
                }else if(getCurrentToken() == TK_IDENT) {
                    overallSymbol.Name = getCurrentLexeme();    
                    currentToken = next();
                    if(getCurrentToken() == SY_LPAREN) {
                        panic("SyntaxError: function declaration shows at local area at line %d, column %d", line, column); 
                    } 
                } 
            }else break; 
        } 
        if(!Parse_varDefinition())  return false;

        if(getCurrentToken() != SY_SEMICOLON) //  ;
        panic("SyntaxError: varDeclration not complete at line %d, column %d", line, column);
    }

    std::cout << "Parse_varDeclaration Over..." << std::endl << std::endl;
    return true;
}


/*<变量定义> ::= <类型标识符>(<标识符> | <标识符>'['<无符号整数>']')
  {, (<标识符> | <标识符> '['<无符号整数>']')} */
bool Parser::Parse_varDefinition() {

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
        currentToken = next();
        if(getCurrentToken() != SY_RBRACKET)    //   ] 
            panic("SyntaxError: varDefinition not complete at line %d, column %d", line, column); 
        currentToken = next();
    }else if(getCurrentToken() == SY_SEMICOLON) {    // ;
        return true; 
    }

    while(true) {

        // ,
        if(getCurrentToken() != SY_COMMA) break;

        //标识符
        currentToken = next();
        if(getCurrentToken() != TK_IDENT) {
            panic("SyntaxError: varDefiniton lack ident at line %d, columne %d", line, column); 
            break;
        }
        id = getCurrentLexeme();

        currentToken = next();  // [
        if(getCurrentToken() == SY_LBRACKET) {
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
            currentToken = next();
            if(getCurrentToken() != SY_RBRACKET) 
                panic("SyntaxError: varDefinition not complete at line %d, column %d", line, column); 
            currentToken = next(); 
        }else if(getCurrentToken() == SY_SEMICOLON) break;
    }
    return true;
}


/*目前静态区域只允许定义一个函数*/
//<函数定义部分> ::= {<有返回值函数定义> | <无返回值函数定义>}
bool Parser::Parse_functionDefinition() {

    std::cout <<  "Parse_functionDefinition Start..." << std::endl;

    if(getCurrentToken() == TK_EOF){
        std::cout <<  "Parse_functionDefinition Over..." << std::endl << std::endl;
        return false; 
    } 
    if(overallSymbol.type == KW_INT || overallSymbol.type == KW_CHAR) {  //因为全局变量解析一定会被执行，所以overallSymbol一定会被赋值
        Parse_haveReturnFuncDefinition();  
    }else if(overallSymbol.type == KW_VOID) {
        Parse_noReturnFuncDefinition(); 
    }else {
        return false; 
    }

    while(true) {
        currentToken = next();    
        if(getCurrentToken() == KW_INT || getCurrentToken() == KW_CHAR || getCurrentToken() == KW_VOID) {
            overallSymbol.type = getCurrentToken();  
            currentToken = next();
            if(getCurrentToken() == KW_MAIN) return false;
            else if(getCurrentToken() != TK_IDENT) {
                panic("SyntaxError: functionDefinition needs name at line %d, column %d", line, column); 
            } 
            overallSymbol.Name = getCurrentLexeme();
            currentToken = next();
            if(getCurrentToken() == SY_LPAREN) {  //解析函数
                if(overallSymbol.type == KW_INT || overallSymbol.type == KW_CHAR) 
                    Parse_haveReturnFuncDefinition();
                else if(overallSymbol.type == KW_VOID)
                    Parse_noReturnFuncDefinition();
            }
        }else break;
    }
    std::cout <<  "Parse_functionDefinition Over..." << std::endl << std::endl;
    return true;
}


//<有返回值函数定义> ::= <声明头部>'('<参数表>')''{'<复合语句>'}'
bool Parser::Parse_haveReturnFuncDefinition() {

    //声明头部
    Parse_FunctionDeclarHead();

    //参数
    currentToken = next();
    if(getCurrentToken() == SY_RPAREN) {  //无参数
        ;
    }else {    //参数列表
        Parse_paraList(); 
    }

    // 参数列表出来检测  ) 
    if(getCurrentToken() != SY_RPAREN) {
        panic("SyntaxError: FunctionDefiniton expects ) at line %d, column %d", line, column);
        return false;
    }

    //  {
    currentToken = next();
    if(getCurrentToken() != SY_LBRACE){
        panic("SyntaxError: lack compound in func at line %d, column %d", line, column);    
    }

    //复合语句
    Parse_compoundStmt();

    // } 
    if(getCurrentToken() != SY_RBRACE) {
        panic("SyntaxError: funcDefiniton expects } at line %d, column %d", line, column); 
    }
    return true;
}


//<无返回值函数定义> ::= void<标识符>'('<参数表>')''{'<复合语句>'}'
bool Parser::Parse_noReturnFuncDefinition() {

    //参数
    currentToken = next();
    if(getCurrentToken() == SY_RPAREN) {   //无参数
        ;
    }else {   //参数列表
        Parse_paraList();
    }

    // 有参数之后再次判断)
    if(getCurrentToken() != SY_RPAREN) {
        panic("SyntaxError: FunctionDefiniton expects ) at line %d, column %d", line, column);
        return false;
    }

    // {
    currentToken = next();
    if(getCurrentToken() != SY_LBRACE) {
        panic("SyntaxError: lack compound in func at line %d, column %d", line, column);    
    }

    //复合语句
    Parse_compoundStmt();


    // }
    if(getCurrentToken() != SY_RBRACE) {
        panic("SyntaxError: funcDefinition expects } at line %d, column %d", line, column); 
    }
    return true;
}

//<声明头部> ::= int<标识符> | char<标识符>
bool Parser::Parse_FunctionDeclarHead() {

    //函数返回值
    if(overallSymbol.type != KW_INT && overallSymbol.type != KW_CHAR)
        panic("SyntaxError: funcDefinition's returnValue error at line %d, column %d", line, column);

    //函数名称
    std::string funcName = overallSymbol.Name;
    return true;
}



//<参数表> ::= <类型标识符><标识符>{,<类型标识符><标识符>}
bool Parser::Parse_paraList() {

    //类型标识符  int | char
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
        if(getCurrentToken() == SY_RPAREN)  break;  // ) 参数结束 
        else if(getCurrentToken() == SY_COMMA) {   //  , 继续解析

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
        }else if(getCurrentToken() == SY_LBRACKET) {    //   [ 参数类型为数组
            /*参数数组可以没有数字  int func(int a[]);*/
            currentToken = next();
            if(getCurrentToken() == SY_RBRACKET){   //无数字
                ; 
            }else if(getCurrentToken() == CONST_INT) {  //有数字
                std::string strnum = getCurrentLexeme();
                int num = 0;
                for(unsigned int i = 0; i < strnum.length(); i ++) {
                    num += strnum[i] - '0'; 
                    num *= 10; 
                } 
                num /= 10;
                if(num <= 0) panic("SyntaxError: elements of array must be positive at line %d, column %d", line, column);

                currentToken = next();
                if(getCurrentToken() != SY_RBRACKET)
                    panic("SyntaxError: paralist lack ]  at line %d, column %d", line, column);

            }else  panic("SyntaxError: paralist error at line %d, column %d", line, column); 
        }else{
            panic("SyntaxError: paralist error at line %d, column %d", line, column);
            return false;
        }
    }

    return true;
}


//<复合语句> ::= [<常量说明>][<变量说明>]{<语句>}
bool Parser::Parse_compoundStmt() {

    currentToken = next();
    Parse_constDeclaration();
    Parse_varDeclaration(false);
    while(true) {
        if(!Parse_Stmt())    //初始值为1
            break;     
    }
    return true;
}



/*<语句> ::= <条件语句> | <循环语句> |  <标识符>['('<值参数表>')'];
  | <赋值语句>; | <读语句>; | <写语句>; | ; | <返回语句>;  */
bool Parser::Parse_Stmt() {


    //紧接着constDeclaration 和 varDeclaration  最后一步是!next
    switch (getCurrentToken()) {
        case KW_IF:   //<条件语句>
            Parse_conditionStmt(); 
            break;
        case KW_WHILE:    //<循环语句>
            Parse_loopStmt();
            break;
        case KW_SCANF:   //<读语句>
            Parse_scanf();
            if(getCurrentToken() != SY_SEMICOLON)
                panic("SyntaxError: Statement lack ; at line %d, column %d", line, column);
            break;
        case KW_PRINTF:    //<写语句>
            Parse_printf();
            if(getCurrentToken() != SY_SEMICOLON)
                panic("SyntaxError: Statement lack ; at line %d, column %d", line, column);
            break;
        case SY_SEMICOLON:   //<空语句>
            break;
        case KW_RETURN:     //<返回语句>
            Parse_returnStmt();
            currentToken = next();
            if(getCurrentToken() != SY_SEMICOLON) 
                panic("SyntaxError: Statement lack ; at line %d, column %d", line, column);
            break;
        case TK_IDENT:   //调用语句   赋值语句

            break;
        default:
            return false;
            break;
    }
    currentToken = next();    //开始检测下一个语句  或者   } 
    return true;
}




//<条件语句> ::= if'('<条件>')'<语句>else<语句>
bool Parser::Parse_conditionStmt() {

    std::cout <<  "Parse_conditionStmt Start..." << std::endl;
    if(getCurrentToken() != KW_IF)  {
        return false; 
    }

    //识别 (
    currentToken = next();
    if(getCurrentToken() != SY_LPAREN) {
        panic("SytaxErro: lack ( at line %d, column %d", line, column); 
        return false; 
    }

    //识别<条件>
    Parse_condition();

    //代码生成

    // )
    if(getCurrentToken() != SY_RPAREN) {
        panic("SytaxErro: lack )  at line %d, column %d", line, column); 
        return false; 
    } 

    currentToken = next();   //   {
    if(getCurrentToken() != SY_LBRACE) {
        panic("SytaxErro: lack  {  at line %d, column %d", line, column); 
        return false;
    }

    //分析语句
    Parse_Stmt();

    currentToken = next();
    if(getCurrentToken() != SY_RBRACE) {   // } 
        panic("SytaxError: lack  } at line %d, column %d", line, column); 
        return false; 
    }

currentToken = next();
if(getCurrentToken() != KW_ELSE) {
    return true;    //没有else 条件语句直接退出
}else {
    currentToken = next(); 
    if(getCurrentToken() != SY_LBRACE) {
        panic("SytaxError: lack  {  at line %d, column %d", line, column); 
        return false; 
    } 

    Parse_Stmt();

    currentToken = next();
    if(getCurrentToken() != SY_RBRACE) {
        panic("SytaxError: lack  } at line %d, column %d", line, column); 
        return false; 
    }
}

std::cout <<  "Parse_conditionStmt Over..." << std::endl;
return true;
}


//<条件> ::= <表达式><关系运算符><表达式> | <表达式>
//<关系运算符> ::= < | <= | > | >= | != | ==
bool Parser::Parse_condition() {

    Parse_expression();

    if(getCurrentToken() == SY_RPAREN) {   //表达式
        return true;
    }else {
        if(getCurrentToken() != SY_GE && getCurrentToken() != SY_GT 
                && getCurrentToken() != SY_LE && getCurrentToken() != SY_LT
                && getCurrentToken() != SY_EQ && getCurrentToken() != SY_NE) {
            panic("SyntaxError: wrong operator at line %d, column %d", line, column); 
        }else {
            Parse_expression(); 

            if(getCurrentToken() != SY_RPAREN) 
                panic("SyntaxError: lack )  at line %d, column %d", line, column);
        }
    }

    return true;
}



//<表达式> ::= [ + | -]<项>{<加法运算符><项>}
bool Parser::Parse_expression() {


    //[+ | -]
    currentToken = next();
    if(getCurrentToken() == SY_PLUS || getCurrentToken() == SY_MINUS){
        //生成代码相关

    }else {
        Parse_item();
        //std::cout << getCurrentLexeme() << std::endl;
        while(true) {
            if(getCurrentToken() == SY_PLUS || getCurrentToken() == SY_MINUS) {
                //生成代码相关
            }else break; 
            Parse_item();
        }
    }

    //表达式计算
    return true;
}


//<项> ::= <因子>{<乘法运算符><因子>}
bool Parser::Parse_item() {

    Parse_factor();

    while(true) {
        if(getCurrentToken() == SY_TIMES || getCurrentToken() == SY_DEV) {


        }else break;
        //因子
        Parse_factor();
    }

    return true;
}


//<因子> ::= <标识符>['('<值参数表>')'] | <标识符> '['<表达式>']' | <整数> | <字符>
bool Parser::Parse_factor() {

    switch (getCurrentToken()) {
        case TK_IDENT:    //标识符   ————>  变量   函数   数组
            currentToken = next();
            if(getCurrentToken() == SY_LPAREN) {
            
            }else if(getCurrentToken() == SY_LBRACKET) {
            
            }else {
            
            
            }
            break;
        case CONST_INT:

            break;
        case CONST_STRING:

            break;
        default:
            return false;
    }

    return true;
}


//<循环语句> ::= while'('<条件>')'<语句>
bool Parser::Parse_loopStmt() {

    std::cout << "Parse_loopStmt Start..." << std::endl;
    if(getCurrentToken() != KW_WHILE) {
        return false;
    }

    currentToken = next();    //   (
    if(getCurrentToken() != SY_LPAREN) {
        panic("SyntaxError: lack  ( at line %d, column %d", line, column); 
    }

    Parse_condition();

    if(getCurrentToken() != SY_RPAREN) {
        panic("SyntaxError: lack  ) at line %d, column %d", line, column); 
    }

    currentToken = next();   //  {
    if(getCurrentToken() != SY_LBRACE) {
        panic("SyntaxError: lack  { at line %d, column %d", line, column); 
    }

    Parse_Stmt();

    currentToken = next();   //   } 
    if(getCurrentToken() != SY_RBRACE) {
        panic("SyntaxError: lack }  at line %d, column %d", line, column); 
    }

    std::cout << "Parse_loopStmt Over..." << std::endl;
    return true;
}




//<值参数表> ::= <表达式>{,<表达式>}
bool  Parser::Parse_valueParamList() {




    return true;
}


//<读语句> ::= scanf'('<标识符>{,<标识符>}')'
bool Parser::Parse_scanf() {

    if(getCurrentToken() != KW_SCANF) {
        return false; 
    }

    currentToken = next();   //   (
    if(getCurrentToken() != SY_LPAREN) {
        panic("SyntaxError: lack  ( at line %d, column %d", line, column); 
    }

    currentToken = next();
    if(getCurrentToken() != TK_IDENT){
        panic("SyntaxError: scanf needs para at line %d, column %d", line, column); 
    }

    while(true) {
        currentToken = next(); 
        if(getCurrentToken() != SY_COMMA){    //   ,
            break; 
        }else{
            currentToken = next();
            if(getCurrentToken() != TK_IDENT) {
                panic("SyntaxError: scanf needs para at line %d, column %d", line, column); 
            }
        }
    }

    //   ) 
    if(getCurrentToken() != SY_RPAREN) {
        panic("SyntaxError: lack  ( at line %d, column %d", line, column); 
    }
    currentToken = next();
    if(getCurrentToken() != SY_SEMICOLON) {
        panic("SyntaxError: lack  ; at line %d, column %d", line, column); 
    }
    return true;
}


//<写语句> ::= printf'('<字符串>')' | printf '('<表达式>')'
bool Parser::Parse_printf() {

    if(getCurrentToken() != KW_PRINTF) {
        return false; 
    }

    currentToken = next();    //    (
    if(getCurrentToken() != SY_LPAREN) {
        panic("SyntaxError: lack  ( at line %d, column %d", line, column); 
    }

    currentToken = next();
    if(getCurrentToken() == CONST_STRING) {   //字符串常量
        currentToken = next();    //    )
    }else {
        Parse_expression();    //  表达式
    }

    if(getCurrentToken() != SY_RPAREN) {
        panic("SyntaxError: lack  )  at line %d, column %d", line, column); 
    } 

    currentToken = next();   //   ;
    if(getCurrentToken() != SY_SEMICOLON) {
        panic("SyntaxError: lack  ; at line %d, column %d", line, column); 
    }
    return true;
}




//<返回语句> ::= return ['('<表达式>')']
bool Parser::Parse_returnStmt() {

    if(getCurrentToken() != KW_RETURN) {
        return false; 
    }

    currentToken = next();
    if(getCurrentToken() != SY_LPAREN) {
        panic("SyntaxError: lack  ( at line %d, column %d", line, column); 
    }

    Parse_expression();

    currentToken = next();
    if(getCurrentToken() != SY_RPAREN) {
        panic("SyntaxError: lack  )  at line %d, column %d", line, column); 
    }

    currentToken = next();
    if(getCurrentToken() != SY_SEMICOLON) {
        panic("SyntaxError: lack ; at line %d, colunm, %d", line, column); 
    }
    return true;
}


//<赋值语句> ::= <标识符>=<表达式> | <标识符>'['<表达式>']'=<表达式>
//实际分析的是  = <表达式> | '['<表达式>']'=<表达式>
bool Parser::Parse_assignStmt() {

        

    return true;
}



//<整数> ::= [+ | -]<无符号整数> | 0
//注意。0前面不能有任何正负号
bool Parser::Parse_integer() {

    //符号表内容  判断数字


    return true;
}



//语法分析器测试函数
void Parser::printParser() {

    parse();
    return;
}


