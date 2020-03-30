#include "../include/symbolTable.h"

/*__symbolItem*/
symbolItem::symbolItem(symbolType st, std::string scope, std::string name) {
    _st = st;
    _scope = scope; 
    _name = name;
}


/*symbolTable*/
bool symbolTable::ispushSymbolItem(std::string scope, std::string itemname) {

    for(unsigned int i = 0; i < __symbolItem.size(); i ++){
        if(__symbolItem[i].getscope() == scope 
                && __symbolItem[i].getname() == itemname) {
            return false; 
        } 
    }
    return true;
}

//localItem 整形类型
bool symbolTable::pushSymbolItem(std::string scope, std::string itemname, localMold mod, int value) {
    if(!ispushSymbolItem(scope, itemname))
            return false;

    localItem localitem(st_localType, scope, itemname);
    localitem.setLm(mod);    //常量  变量  参数
    localitem.setIt(it_intType);   //整形   
    localitem.setInteget(value);
    this->__symbolItem.push_back(localitem);
    return true;
}


//localItem 字符类型
bool symbolTable::pushSymbolItem(std::string scope, std::string itemname, localMold mod, char value) {
    if(!ispushSymbolItem(scope, itemname))
        return false;

    localItem localitem(st_localType, scope, itemname);
    localitem.setLm(mod);   //常量   变量 参数
    localitem.setIt(it_charType);   //数据类型
    localitem.setCharacter(value);
    this->__symbolItem.push_back(localitem);
    return true;
}



//arrayItem
bool symbolTable::pushSymbolItem(std::string scope, std::string itemname, itemType it, int length) {
    if(!ispushSymbolItem(scope, itemname))
        return false;

    arrayItem arrayitem(st_arrayType, scope, itemname);
    arrayitem.setIt(it);
    arrayitem.setLength(length);
    this->__symbolItem.push_back(arrayitem);
    return true;
}


//funcitem
bool symbolTable::pushSymbolItem(std::string scope, std::string name, funcReturnType frt) {
    if(!ispushSymbolItem(scope, name))
        return false;

    funcItem funcitem(st_funcType, scope, name);
    funcitem.setReturnType(frt);
    this->__symbolItem.push_back(funcitem);
    return true;
}




