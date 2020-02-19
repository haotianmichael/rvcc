#include "../include/parser.h"


/*
    构造函数  列表初始化成员关键字 
*/
Parser::Parser(std::string fileName) : keywords({
        {"if", KW_IF}, 
        {"while", KW_WHILE}, 
        {"else", KW_ELSE},
        {"true", KW_TRUE},
        {"false", KW_FALSE},
        {"return", KW_RETURN},
        {"null", KW_NULL}
        }){
    fs.open(fileName);
    if(fs.is_open()) {
        std::cout << "ParserError:  Please open the right source file." << std::endl; 
    } 
}



