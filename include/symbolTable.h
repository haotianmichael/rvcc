#pragma  once
#include <vector>
#include "symbolItem.h"



class SymbolTable
{
    public:
        SymbolTable() {}
        virtual ~SymbolTable() {}
        bool  printTable();   //打印符号表
        
        /*填表操作
            scope  符号表的作用域
            itemname 当前标识符的名称
         */
        bool ispushSymbolItem(std::string scope, std::string itemname);  //检查是否重复定义
        bool pushSymbolItem(std::string scope, std::string itemname, localMold mold, int value);  //int 类型localItem
        bool pushSymbolItem(std::string scope, std::string itemname, localMold mold, char value);   //char 类型 localItem
        bool pushSymbolItem(std::string scope, std::string arrayname, itemType it, int length);  //arrayItem
        bool pushSymbolItem(std::string scope, std::string funcName, funcReturnType frt);   //funcItem
        bool pushSymbolItem(std::string scope, std::string proname);  //proItem

        /*  上下文有关分析
         *    类型检查
         *    语义分析检查
         */
        bool searchTable();  //查表
        bool typeCheck(itemType stype, itemType dtype);   //类型检查


    private:
        SymbolItem *__symbolItemHead = NULL;  //符号表头指针
        SymbolItem *__symbolItemTail = NULL;  //符号表尾指针
        int __symbolItemSize = 0;  //符号表大小
};
