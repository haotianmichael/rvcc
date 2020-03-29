#pragma  once
#include <vector>
#include "symbolItem.h"



class symbolTable
{
    public:
        symbolTable ();
        virtual ~symbolTable ();
        void printTable();   //打印符号表
        
        /*填表操作*/
        bool insertTable(symbolType st);  //填表
        bool pushItem(std::string itemname, localMold mold, int value);  //int 类型localItem
        bool pushItem(std::string itemname, localMold mold, char value);   //char 类型 localItem
        bool pushItem(std::string proname);  //proItem
        bool pushItem(std::string arrayname, int length);  //arrayItem
        bool pushItem(std::string funcName, funcReturnType frt);   //funcItem

        /*  上下文有关分析
         *    类型检查
         *    语义分析检查
         */
        bool searchTable();  //查表
        bool typeCheck(itemType stype, itemType dtype);   //类型检查


    private:
        std::vector<symbolItem> __symbolItem;    //符号表项
};
