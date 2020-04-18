#include "../include/parser.h"
#include "../include/utils.h"
#include "../include/intermediateGenerator.h"
#include "../include/riscvGenerator.h"

extern IntermediateGenerator itgenerator;   //四元式产生表
int varCount = 0;  //临时变量计数器
static int labelCount = 0;   //标签计数器
static int stringCount = 0;   //字符串计数器
static int isPre = 0;  //表达式计算  是否有前驱符号 0为没有  1为+   2为-


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
    }else if(ctr == '-') {  //负数   注意算数表达式部分要注意和直接声明时候的区别
        char ctrn = peekNextChar();
        if((ctrn >= '0' && ctrn <= '9')) {
            std::string num = "";
            num += ctr;
            while(ctrn >= '0' && ctrn <= '9') {
                ctr = getNextChar(); 
                num += ctr; 
                ctrn = peekNextChar(); 
            } 
            return std::make_tuple(CONST_INT, num);
        }
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

    if(ctr == '&') {
        char ctrn = peekNextChar();
        if(ctrn == '&') {
            ctr = getNextChar();
            return std::make_tuple(SY_AND,"&&");        
        }else {
            return std::make_tuple(SY_ADDRESS, "&");
        }
    }

    if(ctr == '|') {
        char ctrn = peekNextChar();
        if(ctrn == '|') {
            ctr = getNextChar();
            return std::make_tuple(SY_OR,"||");        
        }else {
            panic("SynaxError: unknow token: %c", ctr);
        }
    }

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

    std::cout << "Opening Complier..." << std::endl << std::endl;

    /*初始化符号表*/
    this->__symbolTable = new SymbolTable();


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
        std::cout << "###########################SyntaxAnalysis Starting..." << std::endl << std::endl << std::endl;
        //解析
        if(Parse_procedure())  {
            std::cout << std::endl<< std::endl << "###########################SyntaxAnalysis Succeeded!..." << std::endl;
            std::cout  << "Starting To Print SymbolTableFile..." << std::endl;
            __symbolTable->printTable();   //打印符号表
            std::cout << "Starting To Print tmpCodeFile..." << std::endl;
            itgenerator.printTmpItem();   //打印中间代码
            //std::cout << "Starting To Print RiscvCodeFile..." << std::endl;
            std::cout << "Print Succeeded!" << std::endl << std::endl << "Closing Complier..." << std::endl;
            return;
        }
    }
}


/*声明部分*/
//<程序> ::= [<常量说明>][<变量说明>][<函数定义部分>]<主函数>
bool Parser::Parse_procedure() {

    /*说明语句----------填符号表*/
    //全局常量声明
    Parse_constDeclaration("Global");
    //全局变量说明
    Parse_varDeclaration(true, "Global");
    //return true;
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
    __symbolTable->pushSymbolItem("Global", "main", frt_voidType);

    /*中间代码*/
    FourYuanInstr tmp;
    tmp.setopcode(FUNDEC);
    tmp.setfunct(frt_voidType); 
    tmp.settarget("main");
    itgenerator.pushIntermediateItem(tmp);

    //复合语句
    Parse_compoundStmt("main");

    //  } 
    if(getCurrentToken() != SY_RBRACE)
        panic("SyntaxError: main lack } at line %d, column %d", line, column);

    return true;  //分析正常结束
}

//<常量说明> ::= const<常量定义>;{const<常量定义>;}
bool Parser::Parse_constDeclaration(std::string scope) {

    std::cout << "Parse_constDeclaration Start..." << std::endl;
    //const
    if(getCurrentToken() != KW_CONST)  return false;
    currentToken = next();
    //解析<常量定义>
    Parse_constDefinition(scope);

    // ;
    if(getCurrentToken() != SY_SEMICOLON)
        panic("SyntaxError: constDeclaration lack semicolon at line %d, column %d", line, column);

    while(true) {   //  处理右递归

        currentToken = next();
        if(getCurrentToken() != KW_CONST) break;  //正常break

        currentToken = next();
        Parse_constDefinition(scope);

        //  ;
        if(getCurrentToken() != SY_SEMICOLON)
            panic("SyntaxError: constDeclaration lack semicolon at line %d, column %d", line, column);

    }
    std::cout  << "Parse_constDeclaration Over..." << std::endl << std::endl;
    return true;
}


/*<常量定义> ::= int<标识符>=<整数>{,<标识符>=<整数>}
  | char<标识符>=<字符{,<标识符>=<字符>}>*/
bool Parser::Parse_constDefinition(std::string scope) {
    std::string constname = "";
    int value = ERROR_CODE;
    char cvalue = ' ';
    if(getCurrentToken() == KW_INT){  //int
        currentToken = next();
        if(getCurrentToken() != TK_IDENT) {
            panic("SyntaxError: const definition not complete at line %d, column %d", line, column);
            return false;
        }
        constname = getCurrentLexeme();

        currentToken = next();
        if(getCurrentToken() != SY_ASSIGN) {   // = 
            panic("syntaxError: const definition not complete at line %d, columnn %d", line, column); 
            return false; 
        }

        //整数
        currentToken = next();
        value = Parse_integer(getCurrentLexeme());
        /*填充符号表*/
        __symbolTable->pushSymbolItem(scope, constname, lm_constant, it_intType, value);
        //解析右递归
        while(true) {
            currentToken = next();
            if(getCurrentToken() != SY_COMMA) break;  //正常退出

            currentToken = next();
            if(getCurrentToken() != TK_IDENT)   //标识符
                panic("SyntaxError: const definition not complete at line %d, column %d", line, column);
            constname = getCurrentLexeme();

            currentToken = next();
            if(getCurrentToken() != SY_ASSIGN)  // = 
                panic("syntaxError: const definition not complete at line %d, columnn %d", line, column); 

            currentToken  = next();
            value = Parse_integer(getCurrentLexeme());  //整数
            /*填充符号表*/ 
            __symbolTable->pushSymbolItem(scope, constname, lm_constant, it_intType, value);
        }
        //char
    }else if(getCurrentToken() == KW_CHAR){   
        currentToken = next();
        if(getCurrentToken() != TK_IDENT)  {
            panic("SyntaxError: const definition not complete at line %d, column %d", line, column); 
            return false;
        }       
        constname  = getCurrentLexeme();

        currentToken = next();
        if(getCurrentToken() != SY_ASSIGN) {  // = 
            panic("SyntaxError: const definition not complete at line %d, column %d", line, column); 
            return false;
        }

        currentToken = next();
        if(getCurrentToken() != CONST_CHAR)   // 字符
            panic("SyntaxError: const definition not complete at line %d, column %d", line, column); 

        /*填充符号表*/
        std::string str  = getCurrentLexeme();
        cvalue = str[0];
        __symbolTable->pushSymbolItem(scope, constname, lm_constant, it_charType, cvalue);
        //解析右递归
        while(true) {
            currentToken = next();
            if(getCurrentToken() != SY_COMMA)  break;   //正常退出

            currentToken = next();
            if(getCurrentToken() != TK_IDENT)   //标识符
                panic("SyntaxError: const definition not complete at line %d, column %d", line, column); 
            constname  = getCurrentLexeme();

            currentToken = next();
            if(getCurrentToken() != SY_ASSIGN) // = 
                panic("SyntaxError: const definition not complete at line %d, column %d", line, column); 

            currentToken = next();
            if(getCurrentToken() != CONST_CHAR)  //字符
                panic("SyntaxError: const definition not complete at line %d, column %d", line, column); 

            /*填充符号表*/
            std::string str = getCurrentLexeme();
            cvalue = str[0];
            __symbolTable->pushSymbolItem(scope, constname, lm_constant, it_charType, cvalue);
        }

    }else{
        panic("SyntaxError: const definition not complete at line %d, column %d", line, column); 
        return false;
    }
    return true;
}


//<变量说明> ::= <变量定义>;{<变量定义>;}
bool Parser::Parse_varDeclaration(bool isGlobal, std::string scope) {
    std::cout <<  "Parse_varDeclaration Start..." << std::endl;
    //在main之前全局定义   函数 &&  全局变量 && 全局数组
    if(isGlobal) {
        if(getCurrentToken() == KW_INT || getCurrentToken() == KW_CHAR || getCurrentToken() == KW_VOID) {
            if(getCurrentToken() == KW_INT) {
                overallSymbol.type = frt_intType;
            }else if(getCurrentToken() == KW_CHAR) {
                overallSymbol.type = frt_charType; 
            }else if(getCurrentToken() == KW_VOID){
                overallSymbol.type = frt_voidType;
            }else {
                panic("SyntaxError:  unknown dataType at line %d, column %d", line, column);
            }
            currentToken = next();
            overallSymbol.Name  = getCurrentLexeme();   
            if(getCurrentToken() == KW_MAIN) {
                overallSymbol.Name  = getCurrentLexeme();   
                return false;  //解析主函数
            }
            if(getCurrentToken() == TK_IDENT) {   //全局变量 && 全局数组
                currentToken = next();
                if(getCurrentToken() == SY_LPAREN) {
                    std::cout << "Parse_varDeclaration Over..." << std::endl << std::endl;
                    return false;                //解析函数
                }
            }
        }else if(getCurrentToken() == TK_EOF) return false;    //当没有全局声明的时候直接返回   意味着后面没有东西了， 因为全局声明可以检测到main函数的
    }else {   //局部变量  数组
        if(getCurrentToken() == KW_INT || getCurrentToken() == KW_CHAR) {
            if(getCurrentToken() == KW_INT) {
                overallSymbol.type = frt_intType; 
            }else if(getCurrentToken() == KW_CHAR) {
                overallSymbol.type = frt_charType; 
            }else if(getCurrentToken() == KW_VOID){
                overallSymbol.type = frt_voidType;
            }else {
                panic("SyntaxError:  unknown dataType at line %d, column %d", line, column);
            }
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
    if(!Parse_varDefinition(scope)) return false;

    if(getCurrentToken() != SY_SEMICOLON)  //  ;
    panic("SyntaxError: varDeclration not complete at line %d, column %d", line, column);

    //处理右递归
    while(true){
        currentToken = next();
        if(isGlobal) {
            if(getCurrentToken() == KW_INT || getCurrentToken() == KW_CHAR || getCurrentToken() == KW_VOID) {
                if(getCurrentToken() == KW_INT) {
                    overallSymbol.type = frt_intType; 
                }else if(getCurrentToken() == KW_CHAR) {
                    overallSymbol.type = frt_charType; 
                }else if(getCurrentToken() == KW_VOID){
                    overallSymbol.type = frt_voidType;
                }else {
                    panic("SyntaxError:  unknown dataType at line %d, column %d", line, column);
                }
                currentToken = next();
                if(getCurrentToken() == KW_MAIN)  {
                    overallSymbol.Name = getCurrentLexeme();
                    return false;         //解析主函数
                }
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
                if(getCurrentToken() == KW_INT) {
                    overallSymbol.type = frt_intType; 
                }else if(getCurrentToken() == KW_CHAR) {
                    overallSymbol.type = frt_charType; 
                }else if(getCurrentToken() == KW_VOID){
                    overallSymbol.type = frt_voidType;
                }else {
                    panic("SyntaxError:  unknown dataType at line %d, column %d", line, column);
                }
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
        if(!Parse_varDefinition(scope))  return false;

        if(getCurrentToken() != SY_SEMICOLON) //  ;
        panic("SyntaxError: varDeclration not complete at line %d, column %d", line, column);
    }

    std::cout << "Parse_varDeclaration Over..." << std::endl << std::endl;
    return true;
}


/*<变量定义> ::= <类型标识符>(<标识符> | <标识符>'['<无符号整数>']')
  {, (<标识符> | <标识符> '['<无符号整数>']')} */
bool Parser::Parse_varDefinition(std::string scope) {

    int length, num = 0;
    if(getCurrentToken() == SY_LBRACKET) { //  [  全局数组

        currentToken = next(); 
        if(getCurrentToken() != CONST_INT) {
            panic("SyntaxError: varDefinition not complete at line %d, column %d", line, column); 
            return false; 
        }else{
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
        /*填充符号表*/
        if(overallSymbol.type == frt_intType) {
            __symbolTable->pushSymbolItem(scope, overallSymbol.Name, lm_variable, it_arrayType, it_intType, length);
        }else if(overallSymbol.type == frt_charType) {
            __symbolTable->pushSymbolItem(scope, overallSymbol.Name, lm_variable, it_arrayType, it_charType, length);
        }
        currentToken = next();
    }else {
        /*填充符号表*/
        //变量不能在声明的时候直接赋值   所以value就是ERROR_CODE
        if(overallSymbol.type == frt_charType) {
            __symbolTable->pushSymbolItem(scope, overallSymbol.Name, lm_variable, it_charType, '-');
        }else if(overallSymbol.type == frt_intType) {
            __symbolTable->pushSymbolItem(scope, overallSymbol.Name, lm_variable, it_intType, ERROR_CODE);
        } 
    }

    if(getCurrentToken() == SY_SEMICOLON) {    // ;
        return true; 
    } 

    while(true) {
        // ,
        if(getCurrentToken() != SY_COMMA) break;

        //标识符
        std::string identname;
        currentToken = next();
        if(getCurrentToken() != TK_IDENT) {
            panic("SyntaxError: varDefiniton lack ident at line %d, columne %d", line, column); 
            break;
        }
        identname = getCurrentLexeme();

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

            /*填充符号表*/
            if(overallSymbol.type == frt_intType) {
                __symbolTable->pushSymbolItem(scope, identname, lm_variable, it_arrayType, it_intType, length);
            }else if(overallSymbol.type == frt_charType) {
                __symbolTable->pushSymbolItem(scope, identname, lm_variable, it_arrayType, it_charType, length);
            }
            currentToken = next(); 
        }else {
            /*填充符号表*/
            if(overallSymbol.type == frt_charType) {
                __symbolTable->pushSymbolItem(scope, identname, lm_variable, it_charType, '-');
            }else if(overallSymbol.type == frt_intType) {
                __symbolTable->pushSymbolItem(scope, identname, lm_variable, it_intType, ERROR_CODE);

            } 
        }

        if(getCurrentToken() == SY_SEMICOLON) {
            break; 
        }
    }
    return true;
}


//<函数定义部分> ::= {<有返回值函数定义> | <无返回值函数定义>}
bool Parser::Parse_functionDefinition() {

    std::cout <<  "Parse_functionDefinition Start..." << std::endl;

    if(getCurrentToken() == TK_EOF){
        std::cout <<  "Parse_functionDefinition Over..." << std::endl << std::endl;
        return false; 
    } 
    if(overallSymbol.Name == "main" && overallSymbol.type == frt_intType) {
        return false; 
    }else if(overallSymbol.Name == "main" && overallSymbol.type!= frt_voidType) {
        panic("SyntaxError:  Main return wrong value! at line %d, colunm %d", line, column); 
    }
    if(overallSymbol.type == frt_intType || overallSymbol.type == frt_charType) {  //因为全局变量解析一定会被执行，所以overallSymbol一定会被赋值
        //std::cout << getCurrentLexeme() << std::endl;
        Parse_haveReturnFuncDefinition();  
    }else if(overallSymbol.type == frt_voidType) {
        Parse_noReturnFuncDefinition(); 
    }else {
        return false; 
    }

    while(true) {
        currentToken = next();    
        if(getCurrentToken() == KW_INT || getCurrentToken() == KW_CHAR || getCurrentToken() == KW_VOID) {
            if(getCurrentToken() == KW_INT) {
                overallSymbol.type = frt_intType; 
            }else if(getCurrentToken() == KW_CHAR) {
                overallSymbol.type = frt_charType; 
            }else if(getCurrentToken() == KW_VOID){
                overallSymbol.type = frt_voidType; 
            }else {
                panic("SyntaxError:  unknown dataType at line %d, column %d", line, column);
            }
            currentToken = next();
            if(getCurrentToken() == KW_MAIN) return false;
            else if(getCurrentToken() != TK_IDENT) {
                panic("SyntaxError: functionDefinition needs name at line %d, column %d", line, column); 
            } 
            overallSymbol.Name = getCurrentLexeme();
            currentToken = next();
            if(getCurrentToken() == SY_LPAREN) {  //解析函数
                if(overallSymbol.type == frt_intType || overallSymbol.type == frt_charType) 
                    Parse_haveReturnFuncDefinition();
                else if(overallSymbol.type == frt_voidType)
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
    Parse_functionDeclarHead();

    //参数
    currentToken = next();
    if(getCurrentToken() == SY_RPAREN) {  //无参数
        ;
    }else {    //参数列表
        Parse_paraList(overallSymbol.Name); 
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
    Parse_compoundStmt(overallSymbol.Name);
    // } 
    if(getCurrentToken() != SY_RBRACE) {
        //std::cout << getCurrentLexeme() << std::endl;
        panic("qSyntaxError: funcDefiniton expects } at line %d, column %d", line, column); 
    }
    return true;
}


//<无返回值函数定义> ::= void<标识符>'('<参数表>')''{'<复合语句>'}'
bool Parser::Parse_noReturnFuncDefinition() {

    std::string funName = overallSymbol.Name; 
    if(overallSymbol.type != frt_voidType) {
        panic("SyntaxError: funcDefinition's returnValue error at line %d, column %d", line, column);
        return false; 
    }
    __symbolTable->pushSymbolItem("Global", funName, frt_voidType);

    FourYuanInstr tmp;
    tmp.setfunct(frt_voidType);
    tmp.settarget(funName);
    tmp.setopcode(FUNDEC);
    itgenerator.pushIntermediateItem(tmp);

    //参数
    currentToken = next();
    if(getCurrentToken() == SY_RPAREN) {   //无参数
        ;
    }else {   //参数列表
        Parse_paraList(funName);
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
    Parse_compoundStmt(funName);


    // }
    if(getCurrentToken() != SY_RBRACE) {
        panic("SyntaxError: funcDefinition expects } at line %d, column %d", line, column); 
    }
    return true;
}

//<声明头部> ::= int<标识符> | char<标识符>
bool Parser::Parse_functionDeclarHead() {

    //函数返回值
    if(overallSymbol.type != frt_intType && overallSymbol.type != frt_charType){
        panic("SyntaxError: funcDefinition's returnValue error at line %d, column %d", line, column);
        return false;
    }

    //函数返回类型
    funcReturnType frt;
    frt = (overallSymbol.type == frt_intType) ? frt_intType : frt_charType;

    //函数名称
    std::string funcName = overallSymbol.Name; 
    //std::cout << funcName << std::endl;
    __symbolTable->pushSymbolItem("Global", funcName, frt);

    FourYuanInstr tmp;
    tmp.setfunct(frt);
    tmp.settarget(funcName);
    tmp.setopcode(FUNDEC);
    itgenerator.pushIntermediateItem(tmp); 

    return true;
}



//<参数表> ::= <类型标识符><标识符>{,<类型标识符><标识符>}
bool Parser::Parse_paraList(std::string scope) {

    //类型标识符  int | char
    if(getCurrentToken() != KW_INT && getCurrentToken() != KW_CHAR) {
        panic("SyntaxError: paralist error at line %d, column %d", line, column);
        return false; 
    }
    itemType it;
    it = (getCurrentToken() == KW_INT)  ? it_intType : it_charType;
    //标识符
    currentToken = next();
    if(getCurrentToken() != TK_IDENT) {
        panic("SyntaxError: paralist error at line %d, column %d", line, column);
        return false;
    }
    std::string itemname = getCurrentLexeme();
    bool afterArr = false;
    while(true) {
        currentToken = next();
        if(getCurrentToken() == SY_RPAREN)  { // ) 参数结束 
            if(afterArr == true) {
                afterArr = false; 
            }else {
                if(it == it_intType) {
                    __symbolTable->pushSymbolItem(scope, itemname, lm_parameter, it_intType,  ERROR_CODE);
                }else if(it == it_charType) {
                    __symbolTable->pushSymbolItem(scope, itemname, lm_parameter, it_charType, '-');
                }
                FourYuanInstr tmp;
                tmp.setopcode(PARAM);
                tmp.settarget(itemname);
                tmp.setparat(it);
                tmp.setparaArr(false);  //参数不是数组
                itgenerator.pushIntermediateItem(tmp);
            }
            break; 

        } else if(getCurrentToken() == SY_COMMA) {   //  , 继续解析
            if(afterArr == true) {
                afterArr = false;
            }else {
                if(it == it_intType) {
                    __symbolTable->pushSymbolItem(scope, itemname, lm_parameter, it_intType, ERROR_CODE);
                }else if(it == it_charType) {
                    __symbolTable->pushSymbolItem(scope, itemname, lm_parameter, it_charType, '-');
                }
                FourYuanInstr tmp;
                tmp.setopcode(PARAM);
                tmp.settarget(itemname);
                tmp.setparat(it);
                tmp.setparaArr(false);  //参数不是数组
                itgenerator.pushIntermediateItem(tmp);
            }

            currentToken = next();
            if(getCurrentToken() != KW_INT && getCurrentToken() != KW_CHAR) {
                panic("SyntaxError: paralist error at line %d, column %d", line, column);
                return false;
            }
            it = (getCurrentToken()  == KW_INT) ? it_intType : it_charType;
            currentToken = next();
            if(getCurrentToken() != TK_IDENT){
                panic("SyntaxError: paralist error at line %d, column %d", line, column);
                return false; 
            }
            itemname = getCurrentLexeme();

        }else if(getCurrentToken() == SY_LBRACKET) {    //   [ 参数类型为数组
            /*参数数组可以没有数字  int func(int a[]);*/
            afterArr = true;
            currentToken = next();
            if(getCurrentToken() == SY_RBRACKET){   //无数字
                if(it == it_intType) {
                    __symbolTable->pushSymbolItem(scope, itemname, lm_parameter, it_arrayType, it_intType, ERROR_CODE);
                }else if(it == it_charType) {
                    __symbolTable->pushSymbolItem(scope, itemname, lm_parameter, it_arrayType, it_charType, ERROR_CODE);
                }

                FourYuanInstr tmp;
                tmp.setopcode(PARAM);
                tmp.settarget(itemname);
                tmp.setparat(it); 
                tmp.setparaArr(true);
                itgenerator.pushIntermediateItem(tmp);
            }else if(getCurrentToken() == CONST_INT) {  //有数字
                std::string strnum = getCurrentLexeme();
                int num = 0;
                for(unsigned int i = 0; i < strnum.length(); i ++) {
                    num += strnum[i] - '0'; 
                    num *= 10; 
                } 
                num /= 10;
                if(num <= 0) panic("SyntaxError: elements of array must be positive at line %d, column %d", line, column);
                if(it == it_intType) {
                    __symbolTable->pushSymbolItem(scope, itemname, lm_parameter, it_arrayType, it_intType, num);
                }else if(it == it_charType) {
                    __symbolTable->pushSymbolItem(scope, itemname, lm_parameter, it_arrayType, it_charType, num);
                }

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

/*执行部分  至此函数开始涉及表达式运算   属于核心算法*/

//<复合语句> ::= [<常量说明>][<变量说明>]{<语句>}
bool Parser::Parse_compoundStmt(std::string scope) {
    currentToken = next();
    Parse_constDeclaration(scope);
    Parse_varDeclaration(false, scope);
    while(true) {
        if(!Parse_Stmt(scope))  
            break;     
    }
    return true;
}



/*<语句> ::= <条件语句> | <循环语句> |  <标识符>['('<值参数表>')'];
  | <赋值语句>; | <读语句>; | <写语句>; | ; | <返回语句>;  */
bool Parser::Parse_Stmt(std::string scope) {

    std::string name = "";
    FourYuanInstr tmp;

    //紧接着constDeclaration 和 varDeclaration  最后一步是!next
    //std::cout << getCurrentLexeme() << std::endl;
    switch (getCurrentToken()) {
        case KW_IF:   //<条件语句>
            Parse_conditionStmt(scope); 
            break;
        case KW_WHILE:    //<循环语句>
            Parse_loopStmt(scope);
            break;
        case KW_SCANF:   //<读语句>
            Parse_scanf(scope);
            if(getCurrentToken() != SY_SEMICOLON)
                panic("SyntaxError: Statement lack ; at line %d, column %d", line, column);
            break;
        case KW_PRINTF:    //<写语句>
            Parse_printf(scope);
            if(getCurrentToken() != SY_SEMICOLON)
                panic("SyntaxError: Statement lack ; at line %d, column %d", line, column);
            break;
        case SY_SEMICOLON:   //<空语句>
            break;
        case KW_RETURN:     //<返回语句>
            Parse_returnStmt(scope);
            if(getCurrentToken() != SY_SEMICOLON) 
                panic("SyntaxError: Statement lack ; at line %d, column %d", line, column);
            break;
        case TK_IDENT:   //调用语句   赋值语句
            currentToken = next();
            name = getCurrentLexeme();
            if(getCurrentToken() == SY_ASSIGN || getCurrentToken() == SY_LBRACKET) {
                //赋值语句
                Parse_assignStmt(scope, name); 
                if(getCurrentToken() != SY_SEMICOLON) {
                    panic("SyntaxError: Statement lack ; at line %d, column %d", line, column);
                }
            }else if(getCurrentToken() == SY_LPAREN) {
                //函数调用 
                name = getCurrentLexeme();  //函数名称
                tmp.setopcode(FUNCALL);
                tmp.settarget(name);
                currentToken = next();
                if(getCurrentToken() == SY_RPAREN) {  //无参数
                    itgenerator.pushIntermediateItem(tmp); 
                }else { //有参数
                    std::cout << "Start FunCall with param..." << std::endl;
                    std::vector<itemType> valueList = Parse_valueParamList(scope);
                    if(funCheck(name, true, valueList)) {  //函数参数  符号表检查
                        itgenerator.pushIntermediateItem(tmp); 
                    }else {
                        panic("SyntaxError: FunCall Error at line %d, column %d", line, column); 
                    }
                    if(getCurrentToken() != SY_RPAREN) {   //  ) 
                        panic("SyntaxError: Statement lack ) at line %d, column %d", line, column);
                    }
                }
                currentToken = next();
                if(getCurrentToken() != SY_SEMICOLON) {
                    panic("SyntaxError: Statement lack ; at line %d, column %d", line, column);
                }
            }else {
                panic("SyntaxError: No meaning of ident at line %d, column %d", line, column);
                return false;
            }
            break;
        default:
            return false;
            break;
    }
    currentToken = next();    //开始检测下一个语句  或者   
    //std::cout << getCurrentLexeme() << std::endl;
    return true;
}


//函数检查
bool Parser::funCheck(std::string name, bool inExpr, std::vector<itemType> paralist) {

    return true;
    bool isfun = false;
    SymbolItem *head = __symbolTable->getHead();
    SymbolItem *tail = __symbolTable->getTail();
    std::vector<itemType> param;   //从运行栈中读取参数

    //函数名称检查
    while(head != tail) {
        if((head->getscope() == "GLOBAL") && (head->getname() == name) 
                && (head->getSt() == st_funcType)) {   //函数
            isfun = true; 
            FuncItem* fci = static_cast<FuncItem *>(head);
            if(inExpr && fci->getReturnType() == frt_voidType) {
                panic("RuntimeError: VoidFunc can't match in expression at line %d, column %d", line, column);
            }

            head = head->next;
            while(head != tail) {
                LocalItem *fcii = static_cast<LocalItem *>(head);    
                if(fcii->getscope() == name && fcii->getLm() == lm_parameter) {
                    param.push_back(fcii->getIt());    //参数检查
                }else {
                    break; 
                }
                head = head->next;
            } 
            break;    
        }
        head = head->next; 
    }

    //函数参数检查
    if(isfun) {
        if(paralist.size() == 0) {  //实参数量
            isfun = false;
            panic("RuntimeError: Actual Para number error at line %d, colummn %d", line, column); 
        } 
        if(param.size() != paralist.size()) {   //形参数量
            isfun = false;
            panic("RuntimeError:  Formal Para number error at line %d, colummn %d", line, column); 
        }

        for(unsigned int i = 0; i < paralist.size(); i ++) {
            itemType formal = paralist[i];
            itemType actual = param[i];
            if(formal != actual) {
                isfun = false;
                panic("RuntimeError: Para Type not matched at line %d, column %d", line, column); 
            }
        }
    }
    if(isfun == true) {
        std::cout << "FunCall with para Checking Succeed!..." << std::endl; 
    }
    return isfun;
}



//<值参数表> ::= <表达式>{,<表达式>}
std::vector<itemType> Parser::Parse_valueParamList(std::string scope) {

    std::vector<itemType> paralist;
    std::vector<std::string> paramTable;
    FourYuanInstr fyi;


    exprRet er = Parse_expression(scope);
    /*if(!er.isEmpty) {*/
    //paralist.push_back(er.it);
    //}else {
    //return paralist;  
    /*}*/
    if(er.isSure) {
        char x[15] = {'\0'}; 
        sprintf(x, "%d", er.it == it_intType ? er.value : er.cvalue); 
        fyi.setopcode(ASS);
        fyi.setsrcArr(false);
        fyi.settargetArr(false);
        fyi.settarget(varGenerator());
        fyi.setop('+'); 
        fyi.setright("0");
        fyi.setleft(x);
        itgenerator.pushIntermediateItem(fyi);
        paramTable.push_back(fyi.gettarget());
    }else {
        if(er.name.size() > 0 && er.name[0] == 'T') {
            FourYuanInstr tmp;
            tmp.setopcode(ASS);
            tmp.settarget(varGenerator()); 
            tmp.setsrcArr(false);
            tmp.settargetArr(false);
            tmp.setleft(er.name);
            tmp.setop('+');
            tmp.setright("0");
            itgenerator.pushIntermediateItem(tmp);
            paramTable.push_back(tmp.gettarget());
        }else {
            paramTable.push_back(er.name); 
        } 
    }

    while(true) {
        // ,
        if(getCurrentToken() != SY_COMMA) {
            break; 
        } 
        currentToken = next();
        er = Parse_expression(scope);  
        paralist.push_back(er.it); 
        if(er.isSure) {
            char x[15] = {'\0'}; 
            sprintf(x, "%d", er.it == it_intType ? er.value : er.cvalue); 
            fyi.setopcode(ASS);
            fyi.setsrcArr(false);
            fyi.settargetArr(false);
            fyi.settarget(varGenerator());
            fyi.setop('+'); 
            fyi.setright("0");
            fyi.setleft(x);
            itgenerator.pushIntermediateItem(fyi);
            paramTable.push_back(fyi.gettarget());
        }else {
            if(er.name.size() > 0 && er.name[0] == 'T') {
                FourYuanInstr tmp;
                tmp.setopcode(ASS);
                tmp.settarget(varGenerator()); 
                tmp.setsrcArr(false);
                tmp.settargetArr(false);
                tmp.setleft(er.name);
                tmp.setop('+');
                tmp.setright("0");
                itgenerator.pushIntermediateItem(tmp);
                paramTable.push_back(tmp.gettarget());
            }else {
                paramTable.push_back(er.name); 
            } 
        }
    }

    for(unsigned int i = 0; i < paramTable.size(); i ++) {
        fyi.setopcode(PUSH);         
        fyi.settarget(paramTable[i]); 
        itgenerator.pushIntermediateItem(fyi); 
    }

    return paralist;
}


//<表达式> ::= [ + | -]<项>{<加法运算符><项>}
exprRet Parser::Parse_expression(std::string scope) {

    //std::cout << "Start expression" << std::endl;
    bool previs = false;   //有前缀项的flag
    exprRet er;
    std::vector<PostfixExpression> pfeListBefore;  //中缀表达式

    //[+ | -]
    if(getCurrentToken() == SY_PLUS || getCurrentToken() == SY_MINUS){
        //生成代码相关
        previs = true;
        PostfixExpression pfe;
        pfe.it = it_charType;
        pfe.isOpcode = true;
        pfe.value = (getCurrentToken() == SY_PLUS)  ? '+' : '-';
        pfeListBefore.push_back(pfe);
        currentToken = next();
    }

    Parse_item(scope, pfeListBefore);
    while(true) {
        if(getCurrentToken() == SY_PLUS || getCurrentToken() == SY_MINUS) {
            //生成代码相关
            PostfixExpression pfe;
            pfe.it = it_charType; 
            pfe.isOpcode = true;
            pfe.value = (getCurrentToken() == SY_PLUS)  ? '+' : '-';
            pfeListBefore.push_back(pfe); 
            currentToken = next();
            if(getCurrentToken() == SY_PLUS || getCurrentToken() == SY_MINUS) {
                isPre = (getCurrentToken() == SY_PLUS) ? 1 : 2;
                currentToken = next();  //直接解析掉前驱符号
            }else if(getCurrentToken() == SY_TIMES || getCurrentToken() == SY_DEV) {
                panic("SyntaxError: too many operators at line %d, column %d", line, column);
            }
        }else break; 
        Parse_item(scope, pfeListBefore);
    }

    //表达式计算
    if(pfeListBefore.size() == 0) {
        er.isEmpty = true;  
    }else {
        er.isEmpty = false; 
    }

    std::vector<PostfixExpression> pfeListAfter;   //后缀表达式
    postfixReverse(pfeListBefore, pfeListAfter);
    itemType it;
    int value;
    er.name = expressEvaluation(pfeListAfter, it, value);
    if(it == it_intType) {
        er.value = value; 
    } else {
        er.cvalue = value; 
    }
    return er;
}


//<项> ::= <因子>{<乘法运算符><因子>}
bool Parser::Parse_item(std::string scope, std::vector<PostfixExpression> pfeList) {

    //std::cout << "Start item" << std::endl;
    Parse_factor(scope, pfeList);

    while(true) {
        if(getCurrentToken() == SY_TIMES || getCurrentToken() == SY_DEV) {
            PostfixExpression pfe;
            pfe.it = it_charType;
            pfe.str = (getCurrentToken() == SY_TIMES) ? '*' : '/';
            pfe.isOpcode = true;
            pfeList.push_back(pfe);
            currentToken = next();
            if(getCurrentToken() == SY_PLUS || getCurrentToken() == SY_MINUS) {
                isPre = (getCurrentToken() == SY_PLUS) ? 1 : 2;
                currentToken = next();  //直接解析掉前驱符号
            }else if(getCurrentToken() == SY_TIMES || getCurrentToken() == SY_DEV) {
                panic("SyntaxError: too many operators at line %d, column %d", line, column);
            }
        }else break;
        //因子
        Parse_factor(scope, pfeList);
    }

    return true;
} 

//<因子> ::= <标识符>['('<值参数表>')'] | <标识符> '['<表达式>']' | <整数> | <字符> | '('<表达式>')'
/*
   有返回值函数
   数组元素
   括号
   整数
   字符 
   */
bool Parser::Parse_factor(std::string scope, std::vector<PostfixExpression> pfeList) {

    //std::cout << "Start factor" << std::endl;
    PostfixExpression pfe;
    exprRet er;
    FourYuanInstr fy;
    std::string name;

    switch (getCurrentToken()) {
        case TK_IDENT:    //标识符   ————>  变量   函数   数组
            name = getCurrentLexeme();
            currentToken = next();
            if(getCurrentToken() == SY_LBRACKET) {   // [  数组
                currentToken = next();
                er = Parse_expression(scope);
                std::string res;
                int index;
                if(er.isSure) {
                    fy.setopcode(ASS); 
                    fy.settarget(varGenerator()); 
                    fy.setsrcArr(false);
                    fy.settargetArr(false);
                    if(er.it == it_charType) {
                        index = checkArr(name, scope, true, er.cvalue);                         
                        char x[10] = {'\0'}; 
                        sprintf(x, "%d", er.cvalue);
                        fy.setleft(x);
                        fy.setop('+');
                        fy.setright("0");
                    }else {
                        index = checkArr(name, scope, true, er.value);                         
                        char x[10] = {'\0'}; 
                        sprintf(x, "%d", er.value);
                        fy.setleft(x);
                        fy.setop('+');
                        fy.setright("0");
                    }
                    res = fy.gettarget();
                }else{
                    index = checkArr(name, scope, false); 
                    if(er.name.size() > 0 && er.name[0] == 'T') {
                        fy.setopcode(ASS);                
                        fy.settarget(varGenerator());
                        fy.setsrcArr(false);
                        fy.settargetArr(false);
                        fy.setop('+');
                        fy.setright("0");
                        itgenerator.pushIntermediateItem(fy);
                        res = fy.gettarget();
                    }else {
                        res = er.name; 
                    }
                } 

                if(index >= 0) {
                    fy.setopcode(ASS); 
                    fy.settarget(varGenerator()); 
                    fy.setsrcArr(true);
                    fy.settargetArr(false);
                    char x[10] = {'\0'};
                    sprintf(x, "%d", index); 
                    fy.setleft("G" + std::string(x) + name); 
                    itgenerator.pushIntermediateItem(fy);

                    pfe.it = it_stringType;
                    SymbolItem*pt = __symbolTable->getHead();
                    for(int i = 0; i < index; i ++) {
                        if(pt->next != NULL) {
                            pt = pt->next; 
                        } 
                    } 
                    pfe.isCharvar = false;
                    if(static_cast<LocalItem *>(pt)->getIt() == it_charType) {
                        pfe.isCharvar = true; 
                    }
                    pfe.str = fy.gettarget();
                    pfeList.push_back(pfe);
                }else {
                    pfe.it = it_stringType;
                    pfe.str = name;
                }

                if(getCurrentToken() != SY_RBRACKET) {   // ]  
                    panic("SyntaxError:  lack ] at line %d, column %d", line, column); 
                }else {
                    currentToken = next();
                }
            }else if(getCurrentToken() == SY_LPAREN) {   //  (  函数
                currentToken = next();
                if(getCurrentToken() == SY_RPAREN) {
                    currentToken = next();
                    break;
                }
                std::vector<itemType>  paraTable = Parse_valueParamList(scope);
                if(!funCheck(name, true, paraTable)) {
                    panic("SyntaxError: FunCall Error at line %d, column %d", line, column); 
                }
                fy.setopcode(FUNCALL);
                fy.settarget(name);
                itgenerator.pushIntermediateItem(fy);
                fy.setopcode(ASS);
                fy.settarget(varGenerator());
                fy.settargetArr(false);
                fy.setsrcArr(false);
                fy.setleft("Ret");
                fy.setop('+');
                fy.setright("0");
                itgenerator.pushIntermediateItem(fy);

                pfe.it = it_stringType;
                pfe.str = fy.gettarget();
                pfe.isCharvar = false;
                pfeList.push_back(pfe); 


                if(getCurrentToken() != SY_RPAREN) {   // )  
                    panic("SyntaxError:  lack )  at line %d, column %d", line, column); 
                }else {
                    currentToken = next();
                }
            }else {   //变量
                int index = checkInfactor(name, scope);
                if(index >= 0) {
                    SymbolItem * pt = __symbolTable->getHead();
                    for(int i = 0; i < index; i ++) {
                        if(pt->next != NULL) {
                            pt = pt->next; 
                        } 
                    } 
                    if(static_cast<LocalItem*>(pt)->getLm() == lm_constant) {
                        pfe.it = static_cast<LocalItem*>(pt)->getIt();
                        pfe.value = (pfe.it == it_intType) ? (static_cast<LocalItem *>(pt)->getInteger()) 
                            : (static_cast<LocalItem *>(pt)->getCharacter());
                        if(pfe.it == it_charType) 
                            pfe.isOpcode = false;

                    }else {
                        pfe.isCharvar = false;
                        if((static_cast<LocalItem*>(pt)->getLm() == lm_variable || static_cast<LocalItem*>(pt)->getLm() == lm_parameter) 
                                || static_cast<LocalItem*>(pt)->getIt() == it_charType){
                            pfe.isCharvar = true; 
                        }
                        pfe.it = it_stringType;
                        char x[10] = {'\0'};
                        sprintf(x, "%d", index); 
                        pfe.str = "G" + std::string(x) + name;
                    }
                }else {
                    pfe.it = it_stringType; 
                    pfe.isCharvar = false;
                    pfe.str = name;
                    itgenerator.pushIntermediateItem(fy); 
                }
                pfeList.push_back(pfe); 
            }
            break;
        case CONST_INT:
            factor_symbol(isPre);
            break;
        case SY_PLUS:
        case SY_MINUS:
            panic("SyntaxError: too many operators at line %d, column %d", line, column);
            break;
        case CONST_CHAR:
            pfe.it  = it_charType;
            pfe.value = getCurrentLexeme()[0];
            pfe.isOpcode = false;
            pfeList.push_back(pfe);
            break;
        case SY_LPAREN:   //  ()括号
            currentToken = next();
            er = Parse_expression(scope);
            if(er.isSure) {
                pfe.it = it_intType;
                pfe.value = (er.it == it_intType)   ? er.value : er.cvalue;      
            }else {
                pfe.it = it_stringType; 
                if(er.name.size() > 0 && er.name[0] == 'T') {
                    fy.setopcode(ASS);
                    fy.settarget(varGenerator());
                    fy.settargetArr(false);
                    fy.setsrcArr(false); 
                    fy.setop('+');
                    fy.setright("0");
                    fy.setleft(er.name); 
                    itgenerator.pushIntermediateItem(fy);
                    pfe.str = fy.gettarget();
                }else {
                    pfe.str = er.name; 
                }
                pfe.isCharvar = (er.it == it_intType) ? true : false; 
            }
            pfeList.push_back(pfe);
            if(getCurrentToken() != SY_RPAREN) {
                //std::cout << getCurrentLexeme() << std::endl;
                panic("SyntaxError: lack ) at line %d, columne %d ", line, column); 
            }else {
                currentToken = next();
            }
            break;
        default:
            return false;
    }
    return true;
}

//因子中的常量和前驱正负号解析
void Parser::factor_symbol(int isPre) {

    if(isPre == 0) {  //没有前驱符号

    }else if(isPre == 1) { // + 

    }else if(isPre == 2) {  // -

    }
    isPre = 0;
    currentToken = next();  //吃掉该字符
    return ;
}


//<条件语句> ::= if'('<条件>')'<语句>else<语句>
bool Parser::Parse_conditionStmt(std::string scope) {

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
    currentToken = next();

    //识别<条件>
    Parse_condition(scope);

    //代码生成

    // )
    if(getCurrentToken() != SY_RPAREN) {
        panic("2SytaxError: lack )  at line %d, column %d", line, column); 
        return false; 
    } 

    currentToken = next();   //   {
    if(getCurrentToken() != SY_LBRACE) {
        panic("SytaxErro: lack  {  at line %d, column %d", line, column); 
        return false;
    }

    //分析语句
    Parse_compoundStmt(scope);
    //std::cout << getCurrentLexeme() << std::endl;
    if(getCurrentToken() != SY_RBRACE) {   
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

        Parse_compoundStmt(scope);

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
bool Parser::Parse_condition(std::string scope) {

    Parse_expression(scope);
    if(getCurrentToken() == SY_RPAREN) {   //表达式
        return true;
    }

    while(true) {
        if(getCurrentToken() == SY_RPAREN) {
            break; 
        }
        if(getCurrentToken() != SY_GE && getCurrentToken() != SY_GT 
                && getCurrentToken() != SY_LE && getCurrentToken() != SY_LT
                && getCurrentToken() != SY_EQ && getCurrentToken() != SY_NE) {
            panic("SyntaxError: wrong operator at line %d, column %d", line, column); 
        }
        currentToken = next();
        Parse_expression(scope); 

        if(getCurrentToken() == SY_AND || getCurrentToken() == SY_OR) {
            currentToken = next();
            Parse_expression(scope); 
        }
    }

    if(getCurrentToken() != SY_RPAREN) {
        panic("SyntaxError: lack )  at line %d, column %d", line, column);
    }


    return true;
}



//<循环语句> ::= while'('<条件>')'<语句>
bool Parser::Parse_loopStmt(std::string scope) {

    std::cout << "Parse_loopStmt Start..." << std::endl;
    if(getCurrentToken() != KW_WHILE) {
        return false;
    }

    currentToken = next();    //   (
    if(getCurrentToken() != SY_LPAREN) {
        panic("SyntaxError: lack  ( at line %d, column %d", line, column); 
    }
    currentToken = next();    //   (

    Parse_condition(scope);

    if(getCurrentToken() != SY_RPAREN) {
        panic("SyntaxError: lack  ) at line %d, column %d", line, column); 
    }

    currentToken = next();   //  {
    if(getCurrentToken() != SY_LBRACE) {
        panic("SyntaxError: lack  { at line %d, column %d", line, column); 
    }

    Parse_compoundStmt(scope);

    if(getCurrentToken() != SY_RBRACE) {
        panic("SyntaxError: lack }  at line %d, column %d", line, column); 
    }
    std::cout << "Parse_loopStmt Over..." << std::endl;
    return true;
}




//<读语句> ::= scanf'('<标识符>{,<标识符>}')'
bool Parser::Parse_scanf(std::string scope) {

    if(getCurrentToken() != KW_SCANF) {
        return false; 
    }
    scope = "";  //检查标识符作用域
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
bool Parser::Parse_printf(std::string scope) {

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
        Parse_expression(scope);    //  表达式
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
bool Parser::Parse_returnStmt(std::string scope) {
    if(getCurrentToken() != KW_RETURN) {
        return false; 
    }

    currentToken = next();
    if(getCurrentToken() != SY_LPAREN) {
        panic("SyntaxError: lack  ( at line %d, column %d", line, column); 
    }
    currentToken = next();

    Parse_expression(scope);

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
bool Parser::Parse_assignStmt(std::string scope, std::string name) {

    if(getCurrentToken() == SY_ASSIGN) {   //标识符
        currentToken  = next();

        //std::cout << getCurrentLexeme() << std::endl;
        Parse_expression(scope); 

        //std::cout << getCurrentLexeme() << std::endl;
        if(getCurrentToken() != SY_SEMICOLON) {
            panic("SyntaxError: lack ; at line %d, colunm, %d", line, column); 
        }
    }else if(getCurrentToken() == SY_LBRACKET){   //数组元素

        currentToken = next();
        Parse_expression(scope);     

        if(getCurrentToken() != SY_RBRACKET) {
            panic("SyntaxError: lack ; at line %d, colunm, %d", line, column); 
        }

        currentToken = next();
        if(getCurrentToken() != SY_ASSIGN) {
            panic("SyntaxError: lack ; at line %d, colunm, %d", line, column); 
        }

        currentToken = next();
        Parse_expression(scope);

    }else {
        panic("SyntaxError:  wrong format of assignment at line %d, column %d", line, column); 
    }

    return true;
}



//<整数> ::= [+ | -]<无符号整数> | 0
//注意。0前面不能有任何正负号
int Parser::Parse_integer(std::string value) {

    /*
       符号表内容  判断数字
       wrong format 019
       */
    int num = 0;
    if(value.length() > 1 && value[0] == '0') {
        panic("IntegerParserError: wrong format at line %d, column %d", line, column);
    }

    if(value[0] == '-') {
        for(unsigned int i = 1; i < value.length(); i ++)  {
            num += value[i] - '0'; 
            num *= 10;
        }
        return -num/10;
    }else {
        for(unsigned int i = 0; i < value.length(); i ++) {
            num += value[i] - '0'; 
            num *= 10; 
        }
        return num/10; 
    }
}


//中缀表达式转后缀表达式
void Parser::postfixReverse(std::vector<PostfixExpression> pfeListBefore, std::vector<PostfixExpression> pfeListAfter) {

    std::vector<PostfixExpression> tmp;
    if(pfeListBefore.size() == 1) {
        pfeListAfter.push_back(pfeListBefore[0]); 
        return;
    }else if(pfeListBefore.size() > 1) {
        if(pfeListBefore[0].it == it_charType && (pfeListBefore[0].value == '+' || pfeListBefore[0].value == '-') && pfeListBefore[0].isOpcode == true) {
            if(pfeListBefore[0].value == '-') {
                PostfixExpression pfe;
                pfe.it = it_intType;
                pfe.value = 0;
                pfe.isOpcode = true;
                pfeListBefore.insert(pfeListBefore.begin(), pfe);   //如果第一个元素是-  插入0
            }else {
                pfeListBefore.erase(pfeListBefore.begin());  //消除 +
            }
        } 
    }

    for(unsigned int i = 0; i < pfeListBefore.size(); i ++) {
        PostfixExpression pfe = pfeListBefore[i]; 
        if(pfe.it == it_charType) {   //后缀运算符优先级
            switch (pfe.value) {
                case '+':
                case '-':
                    if(!pfe.isOpcode) {
                        pfeListAfter.push_back(pfe); 
                        break; 
                    } 
                    while(tmp.size() != 0) {
                        pfeListAfter.push_back(tmp[tmp.size() - 1]);                    
                        tmp.pop_back();
                    }
                    tmp.push_back(pfe);
                    break;
                case '*':
                case '/':
                    if(!pfe.isOpcode) {
                        pfeListAfter.push_back(pfe); 
                        break; 
                    } 
                    while(tmp.size() != 0) {
                        if(tmp[tmp.size() - 1].value == '*' || tmp[tmp.size() - 1].value == '/') {
                            pfeListAfter.push_back(tmp[tmp.size() - 1]); 
                            tmp.pop_back();
                        }else {
                            break; 
                        } 
                    }
                    tmp.push_back(pfe);
                    break;
                default: pfeListAfter.push_back(pfe);
            }        
        }else {  //运算数直接插入
            pfeListAfter.push_back(pfe); 
        } 
    }

    while(tmp.size() != 0) {
        pfeListAfter.push_back(tmp[tmp.size() - 1]); 
        tmp.pop_back(); 
    }
    return;
}


//表达式求值
std::string Parser::expressEvaluation(std::vector<PostfixExpression> &pfeList, itemType &type, int &value) {

    PostfixExpression pfeA, pfeB;
    FourYuanInstr fyi;
    std::vector<PostfixExpression> tmp;
    if(pfeList.size() == 1) {
        pfeA = pfeList[0]; 
        if(pfeA.it == it_intType) {
            type = it_intType; 
            value = pfeA.value;
            return "";
        }else if(pfeA.it == it_charType) {
            type = it_charType; 
            value = pfeA.value;
            return ""; 
        }else {
            if(pfeA.isCharvar) {  //字符型变量
                type = it_charType; 
            } else {
                type = it_intType; 
            }
            return pfeA.str; 
        } 
    }else {
        for(unsigned int i = 0; i < pfeList.size();  i ++) {
            pfeA = pfeList[i];
            if(pfeA.it == it_charType) {
                char x[15] = {'\0'};    
                fyi.setopcode(ASS);     
                fyi.settargetArr(false); 
                fyi.setsrcArr(false); 
                switch (pfeA.value) {
                    case '+':
                    case '-':{
                                 if(!pfeA.isOpcode) {
                                     tmp.push_back(pfeA); 
                                     break;
                                 }
                                 if(tmp.size() > 1) {
                                     bool isAble = true;
                                     int leftD, rightD; 
                                     if(tmp[tmp.size() - 1].it == it_stringType) {
                                         fyi.setright(tmp[tmp.size()  - 1].str); 
                                         tmp.pop_back();
                                         isAble = false;
                                     }else {
                                         sprintf(x, "%d", tmp[tmp.size() - 1].value); 
                                         fyi.setright(x); 
                                         rightD = tmp[tmp.size() - 1].value;
                                         tmp.pop_back();
                                     } 
                                     memset(x, 0, 15); 
                                     if(tmp[tmp.size() - 1].it == it_stringType) {
                                         fyi.setleft(tmp[tmp.size()  - 1].str); 
                                         tmp.pop_back();
                                         isAble = false;
                                     }else {
                                         sprintf(x, "%d", tmp[tmp.size() - 1].value); 
                                         fyi.setleft(x); 
                                         leftD = tmp[tmp.size() - 1].value;
                                         tmp.pop_back();
                                     } 
                                     if(isAble) {
                                         pfeB.it = it_intType; 
                                         pfeB.value  = (pfeA.value == '+') ?  (leftD + rightD) : (leftD - rightD);
                                         tmp.push_back(pfeB);
                                         break;
                                     }
                                     std::string var = varGenerator(); 
                                     fyi.settarget(var);
                                     fyi.setop(pfeA.value);  
                                     itgenerator.pushIntermediateItem(fyi);
                                     pfeB.it = it_stringType;
                                     pfeB.str = var;
                                     tmp.push_back(pfeB);
                                 }
                                 break;
                             }
                    case '*':
                    case '/':{
                                 if(!pfeA.isOpcode) {
                                     tmp.push_back(pfeA); 
                                     break;
                                 }
                                 if(tmp.size() > 1) {
                                     bool isAble = true;
                                     int leftD, rightD; 
                                     if(tmp[tmp.size() - 1].it == it_stringType) {
                                         fyi.setright(tmp[tmp.size()  - 1].str); 
                                         tmp.pop_back();
                                         isAble = false;
                                     }else {
                                         sprintf(x, "%d", tmp[tmp.size() - 1].value); 
                                         fyi.setright(x); 
                                         rightD = tmp[tmp.size() - 1].value;
                                         tmp.pop_back();
                                     } 
                                     memset(x, 0, 15); 
                                     if(tmp[tmp.size() - 1].it == it_stringType) {
                                         fyi.setleft(tmp[tmp.size()  - 1].str); 
                                         tmp.pop_back();
                                         isAble = false;
                                     }else {
                                         sprintf(x, "%d", tmp[tmp.size() - 1].value); 
                                         fyi.setleft(x); 
                                         leftD = tmp[tmp.size() - 1].value;
                                         tmp.pop_back();
                                         if(pfeA.value == '/' && fyi.getright() == "0") {
                                             panic("RuntimeError: div 0 situation"); 
                                         }
                                     } 
                                     if(isAble) {
                                         pfeB.it = it_intType; 
                                         pfeB.value  = (pfeA.value == '*') ?  (leftD * rightD) : (leftD / rightD);
                                         tmp.push_back(pfeB);
                                         break;
                                     }
                                     std::string var = varGenerator(); 
                                     fyi.settarget(var);
                                     fyi.setop(pfeA.value);  
                                     itgenerator.pushIntermediateItem(fyi);
                                     pfeB.it = it_stringType;
                                     pfeB.str = var;
                                     tmp.push_back(pfeB);
                                 }
                                 break;
                             }
                    default: tmp.push_back(pfeA);
                }
            }else {
                tmp.push_back(pfeA); 
            } 
        }    
        if(tmp.size() >= 1) {
            if(tmp[0].it == it_intType) {
                type = it_intType;
                value = tmp[0].value;
                return ""; 
            } 
            return tmp[0].str;
        }else {
            return ""; 
        }
    }
}


//创建临时变量
std::string Parser::varGenerator() {
    char x[10] = {'\0'};
    sprintf(x, "%d", ++varCount);
    return ("T"+std::string(x));
}

//创建标签
std::string Parser::labelGenetar() {
    char x[10] = {'\0'};
    sprintf(x, "%d", ++labelCount);
    return ("Label"+std::string(x));
}

//创建字符串
std::string Parser::stringGenetar() {
    char x[10] = {'\0'};
    sprintf(x, "%d", ++stringCount);
    return ("String" + std::string(x));
}

//语法分析器测试函数
void Parser::printParser() {

    parse();
    return;
}


int Parser::checkInfactor(std::string name, std::string scope)  {

    return 0;
}


int Parser::checkArr(std::string name, std::string scope, bool exp, int index) {
    return 0;
}


int Parser::checkInStmt(std::string name) {


    return 0;
}

