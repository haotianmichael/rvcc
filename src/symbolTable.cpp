#include "../include/symbolTable.h"

/*__symbolItem*/
symbolItem::symbolItem(symbolType st, std::string scope, std::string name) {
    _st = st;
    _scope = scope; 
    _name = name;
}


/*symbolTable*/
bool symbolTable::ispushSymbolItem(std::string scop, std::string itemname) {

    for(unsigned int i = 0; i < __symbolItem.size(); i ++){
        if(__symbolItem[i].getscope() == scop 
                && __symbolItem[i].getname() == itemname) {
            return false; 
        } 
    }
    return true;
}

//localItem 类型
bool symbolTable::pushSymbolItem(std::string scop, std::string itemname, localMold mod, int value) {
    if(!ispushSymbolItem(scop, itemname))
            return false;


    return true;
}



bool symbolTable::pushSymbolItem(std::string scop, std::string itemname, localMold mod, char value) {



    return true;
}
