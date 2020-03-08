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
        {"null", KW_NULL,}
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
void Parser::parse() {
    
    currentToken = next();
    P_Token nonterminal = std::get<0>(currentToken);
    if(nonterminal == TK_EOF){
        std::cout << "ParseAnalyze succeeded" << std::endl;
        return;
    }
    //if(nonterminal == TK_FILENAME)  




}



