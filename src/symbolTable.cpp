#include "../include/symbolTable.h"
#include <vector>


//全局符号表
std::vector<SymbolTable> globalSymbolTable;

SymbolTable::SymbolTable(std::string id, std::string funcName) {
    functinoName = funcName;
    identifier = id;
    length = 0; 
    weight = 0;
}


