#pragma  once
#include <vector>
#include "symbolItem.h"



class symbolTable
{
    public:
        symbolTable ();
        virtual ~symbolTable ();

    private:
        int level;   //符号表作用域层次
        std::vector<symbolItem> __symbolItem;    //符号表项
};
