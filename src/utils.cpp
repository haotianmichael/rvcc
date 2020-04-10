#include <cstdarg>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "../include/utils.h"
#include "../include/parser.h"


//抛出异常
void panic(const char* const format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
    exit(EXIT_FAILURE);
}


//判断文件格式
bool formatC(const  std::string &fileName) {
    if(fileName[fileName.length() - 1] != 'c') 
        return false;
    else
        return true; 
}

//string转换int
int sToi(std::string str) {
    int res = 0;
    bool is = false;
    if(str.size() <= 0) return ERROR_CODE;
    if(str[0] == '-')     is = true;  

    for(unsigned int i = 0; i < str.size(); i ++) {
        if(!(str[i] >= '0' && str[i] <= '9'))  return ERROR_CODE;
        res += str[i] - '0';
        res *= 10;
    } 
    res /= 10;

    return is?(-res):res;
}
