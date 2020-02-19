#include <cstdarg>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "../include/utils.h"


//抛出异常
void panic(const char* const format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
    exit(EXIT_FAILURE);
}



bool formatC(const  std::string &fileName) {
    if(fileName[fileName.length() - 1] != 'c') return false;
    else return true; 
}
