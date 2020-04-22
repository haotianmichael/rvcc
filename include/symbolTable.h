#pragma  once
#include <vector>
#include "symbolItem.h"



class SymbolTable
{
    public:
        SymbolTable() {}
        virtual ~SymbolTable() {}
        bool  printTable();   //打印符号表
        inline int getSize() { return __symbolItemSize;}
        inline SymbolItem* getHead() {  return __symbolItemHead;}
        inline SymbolItem* getTail() {  return __symbolItemTail;} 
        /*填表操作
            scope  符号表的作用域
            itemname 当前标识符的名称
         */
        bool ispushSymbolItem(std::string scope, std::string itemname);  //检查是否重复定义
        /*变量  常量  参数*/
        bool pushSymbolItem(std::string scope, std::string itemname, localMold mold, itemType it, int value);  //int 类型localItem  
        bool pushSymbolItem(std::string scope, std::string itemname, localMold mold, itemType it, char value);   //char 类型 localItem
        bool pushSymbolItem(std::string scope, std::string itemname, localMold mold, itemType it_array, itemType array_type, int length);  //数组  类型localItem
        //funcItem
        bool pushSymbolItem(std::string scope, std::string funcName, funcReturnType frt);  

        /*  
         *  查表操作
         *  上下文有关分析  类型检查   语义分析检查
         *  
         */
        bool searchTable();  //查表
        bool typeCheck(std::string name, std::string scope, itemType dtype);   //类型检查
        bool funCheck(std::string name, bool inExpr, std::vector<itemType> paralist);  //函数参数检查
        bool identCheck(std::string name, std::string scope);  //标识符检查    因子项
        int stmtCheck(std::string name);  //标识符检查  赋值语句
        bool arrCheck(std::string name, std::string scope, bool exp, int index = 0);  //标识符[<表达式>]检查  赋值语句  因子项


    private:
        SymbolItem *__symbolItemHead = NULL;  //符号表头指针
        SymbolItem *__symbolItemTail = NULL;  //符号表尾指针
        int __symbolItemSize = 0;  //符号表大小
};
