#pragma  once
#include "lex.h"


class SymbolTable{

    private:
        std::string identifier;     //标识符
        int order;   //符号表所在的项号  start form 0
        std::string functinoName;   //符号表所属函数作用域
        ItemType itemType;    //符号表项类型
        ValueType valueType;    //值类型
        ReturnFucType functionType;     //函数类型
        int constIntValue;   //整数常量
        char constCharValue;   //字符常量
        int length;   //数组的长度  变量的设置为0
        int weight;   //只针对函数内部的简单变量以及参数有效


    public:
        SymbolTable(std::string id, std::string funcName);

        /*get*/
        inline int getOrder(){return order;}
        inline std::string getId(){ return identifier;};
        inline std::string getFuncName() {return functinoName;}
        inline ItemType getItemType() {return itemType;}
        inline ValueType getValueType() {return valueType;}
        inline ReturnFucType getReturnType() {return functionType;}
        inline int getWeight() {return weight;}

        /*set*/
        void setItemType(ItemType type) {itemType = type;}
        void setValueType(ValueType type) {valueType = type;}
        void steArrSize(int size){length = size;}
        void setConstInt(int value) {constIntValue = value;}
        void setConstChar(char value) {constCharValue = value;} 
        void setReturnType(ReturnFucType type) {functionType = type;}
        void addWeight(int num) {weight += num;}
};
