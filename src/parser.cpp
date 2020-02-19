#include "../include/parser.h"
#include "../include/utils.h"



/*
   构造函数  列表初始化成员关键字 
 */
Parser::Parser(std::string &fileName) : keywords({
        {"main", KW_MAIN},
        {"if", KW_IF}, 
        {"while", KW_WHILE}, 
        {"else", KW_ELSE},
        {"true", KW_TRUE},
        {"false", KW_FALSE},
        {"return", KW_RETURN},
        {"null", KW_NULL}
        }){
    fs.open(fileName);
    if(fs.is_open() && formatC(fileName)) {    //输入以.c结尾的文件
        panic("ParserError: Please open files with .c\n");
    } 
}


/*
   词法分析器 
 */
std::tuple<std::string, TOKEN> Parser::next() {

    char ctr = getNextChar();
    if(ctr == EOF) return std::make_tuple("", SY_EOF);

    while(ctr == ' ' || ctr == '\n'  || ctr == '\t' || ctr == '\r') {
        if(ctr == '\n' || ctr == '\r') {
            line++; 
            column = 0; 
        }
        ctr = getNextChar();
    }

    //整数字面值
    if(ctr >= '0' && ctr <= '9') {
        std::string num = ""; 
        num += ctr;
        char ctrn = peekNextChar();
        while(ctrn >= '0' && ctrn <= '9'){
            ctr = getNextChar();
            ctrn = peekNextChar();
            num += ctr;
        }
        return std::make_tuple(num, KW_INT); 
    }

    //字符串字面值
    if(ctr == '"') {
        std::string str = ""; 
        char ctrn = peekNextChar(); 
        while(ctrn != '"') {
            ctr = getNextChar(); 
            str += ctr; 
            ctrn = peekNextChar(); 
        } 
        return std::make_tuple(str, KW_STR); 
    }
   
    //字符字面值
    if(ctr == '\'') {
        std::string str = ""; 
        ctr = getNextChar();
        str += ctr;
        char ctrn = getNextChar();
        if(ctrn != '\'')  panic("Synax Error at line:%d  column:%d\n", line, column);
        return std::make_tuple(str, KW_CHAR);
    }


    //标识符  关键字
    if((ctr >= 'a' && ctr <= 'z') || (ctr >= 'A' && ctr <= 'Z')) {
        std::string str = "";
        str += ctr; 
        char ctrn = peekNextChar();
        while((ctrn >= 'a' && ctrn <= 'z') || (ctrn >= 'A' && ctrn <= 'Z') || (ctrn >= '0' && ctrn <= '9')) {
            ctr = getNextChar(); 
            ctrn = peekNextChar();
            str += ctr; 
        } 
        
        auto result = keywords.find(str); 
        return (result != keywords.end()) ? std::make_tuple(str, result->second)  : std::make_tuple(str, TK_IDENT);
    }


    //内置符号
    if(ctr == '+')  return std::make_tuple("+", SY_PLUS);
    if(ctr == '-')  return std::make_tuple("-", SY_MINUS);
    if(ctr == '*')  return std::make_tuple("*", SY_TIMES);
    if(ctr == '/')  return std::make_tuple("/", SY_DEV);
    if(ctr == ',')  return std::make_tuple(",", SY_COMMA);
    if(ctr == '(')  return std::make_tuple("(", SY_LPAREN);
    if(ctr == ')')  return std::make_tuple(")", SY_RPAREN);
    if(ctr == '{')  return std::make_tuple("{", SY_LBRACE);
    if(ctr == '}')  return std::make_tuple("}", SY_RBRACE);
    if(ctr == '[')  return std::make_tuple("[", SY_LBRACKET);
    if(ctr == ']')  return std::make_tuple("]", SY_RBRACKET);
    if(ctr == '=') {
        char ctrn = getNextChar();
        if(ctrn != '=') panic("Synax Error at line:%d column:%d\n", line, column);
        return std::make_tuple("==", SY_EQ);
    }
    if(ctr == '!') {
        char ctrn = getNextChar(); 
        if(ctrn != '=') panic("Synax Error at line:%d column:%d\n", line, column);
        return std::make_tuple("!=", SY_NE); 
    }

        
    if(ctr == '>') {
        char ctrn = peekNextChar(); 
        if(ctrn == '=') {
            getNextChar();
            return std::make_tuple(">=", SY_GE); 
        }else {
            return std::make_tuple(">", SY_GT);                
        }
    }

    if(ctr == '<') {
        char ctrn = peekNextChar(); 
        if(ctrn == '=')  {
            getNextChar();
            return std::make_tuple("<=", SY_LE); 
        }else{
            return std::make_tuple("<", SY_LT); 
        }
    }


    panic("ParserError: Synax Error");
    return std::make_tuple("invalid", INVALD); 
}
