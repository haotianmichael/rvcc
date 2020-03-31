#include "../include/symbolTable.h"
#include "../include/parser.h"
#include "../include/utils.h"

/*__symbolItem*/
symbolItem::symbolItem(symbolType st, std::string scope, std::string name) {
    _st = st;
    _scope = scope; 
    _name = name;
    next = NULL;
}

/*symbolTable*/
//检查是否可以填表
bool symbolTable::ispushSymbolItem(std::string scope, std::string itemname) {

    symbolItem *si = __symbolItemHead;
    while(si != NULL) {
        if(si->getname() == itemname && si->getscope() == scope){
            return false;     
        } 
        si = si->next; 
    }
    return true;
}

//localItem 整形类型
bool symbolTable::pushSymbolItem(std::string scope, std::string itemname, localMold mod, int value) {
    if(!ispushSymbolItem(scope, itemname))
        return false;

    localItem *localitem = new localItem(st_localType, scope, itemname);
    localitem->setLm(mod);    //常量  变量  参数
    localitem->setIt(it_intType);   //整形   
    localitem->setInteget(value);
    //插入符号表 尾插法
    if(__symbolItemHead == NULL && __symbolItemTail == NULL) {
        __symbolItemHead = localitem;   
        __symbolItemTail = localitem;
        __symbolItemSize++;
    }else if(__symbolItemHead != NULL && __symbolItemTail != NULL){
        __symbolItemTail->next = localitem;
        __symbolItemTail = localitem;
        __symbolItemSize++;
    }else {
        panic("SymbolTableError: pushItem not allowed"); 
    }

    return true;
}


//localItem 字符类型
bool symbolTable::pushSymbolItem(std::string scope, std::string itemname, localMold mod, char value) {
    if(!ispushSymbolItem(scope, itemname))
        return false;

    localItem *localitem = new localItem(st_localType, scope, itemname);
    localitem->setLm(mod);   //常量   变量 参数
    localitem->setIt(it_charType);   //数据类型
    localitem->setCharacter(value);
    //this->__symbolItem.push_back(localitem);
    if(__symbolItemHead == NULL && __symbolItemTail == NULL) {
        __symbolItemHead = localitem; 
        __symbolItemTail = localitem;
        __symbolItemSize++;
    }else if(__symbolItemHead != NULL && __symbolItemTail != NULL){
        __symbolItemTail->next = localitem;
        __symbolItemTail = localitem;
        __symbolItemSize++;
    }else {
        panic("SymbolTableError: pushItem not allowed"); 
    }
    return true;
}



//arrayItem
bool symbolTable::pushSymbolItem(std::string scope, std::string itemname, itemType it, int length) {
    if(!ispushSymbolItem(scope, itemname))
        return false;

    arrayItem *arrayitem = new arrayItem(st_arrayType, scope, itemname);
    arrayitem->setIt(it);
    arrayitem->setLength(length);
    //this->__symbolItem.push_back(arrayitem);
    if(__symbolItemHead == NULL && __symbolItemTail == NULL) {
        __symbolItemHead = arrayitem; 
        __symbolItemTail = arrayitem;
        __symbolItemSize++;
    }else if(__symbolItemHead != NULL && __symbolItemTail != NULL){
        __symbolItemTail->next = arrayitem;
        __symbolItemTail = arrayitem;
        __symbolItemSize++;
    }
    return true;
}


//funcitem
bool symbolTable::pushSymbolItem(std::string scope, std::string name, funcReturnType frt) {
    if(!ispushSymbolItem(scope, name))
        return false;

    funcItem funcitem(st_funcType, scope, name);
    funcitem.setReturnType(frt);
    //this->__symbolItem.push_back(funcitem);
    return true;
}


bool symbolTable::pushSymbolItem(std::string scope, std::string proname) {



    return true;
}

//打印符号表
bool symbolTable::printTable() {

    symbolItem *si = __symbolItemHead;
    if(si == NULL) {
        return false; 
    }
    /*标题*/
    std::cout << "SymbolTableType\t" << "name\t" << "scope\t"  << "localMold\t" << "itemType\t"<<  "value" << std::endl;


    while(si != NULL) {
        symbolType st = si->getSt();
        if(st == st_localType) {
            localItem *li = static_cast<localItem *>(si); 
            std::string name = li->getname();  //名称
            std::string scope = li->getscope();  //函数作用域
            localMold lm = li->getLm();  //常量 变量
            itemType it = li->getIt();
            int value = ERROR_CODE;
            char cvalue = ' ';
            std::cout << "localType\t";
            std::cout << name << "\t"; 
            std::cout << scope << "\t";
            if(lm == lm_constant) {
                std::cout << "constant\t";
            }else if(lm == lm_variable) {
                std::cout << "variable\t"; 
            }
            if(it == it_intType) {
                value = li->getInteger(); 
                std::cout << "int\t\t" << value << "\t";
            }else if(it == it_charType){
                cvalue = li->getCharacter(); 
                std::cout << "char\t\t" << cvalue << "\t";
            }
            std::cout << std::endl;

        }else if(st == st_arrayType){
            arrayItem *ai = static_cast<arrayItem *>(si);
            std::string name = ai->getname();  //名称
            std::string scope =  ai->getscope();   //函数作用域
            itemType it = ai->getType();   //数据类型
            int length = ai->getLength();
            std::cout << "arrayType\t";
            std::cout << name << "\t";
            std::cout << scope << "\t";
            std::cout << "variable\t";
            if(it == it_intType) {
                std::cout << "int\t\t" << length << "\t";
            }else if(it == it_charType) {
                std::cout << "char\t\t" << length << "\t"; 
            }
            std::cout << std::endl;

        }else if(st == st_procType){


        }else if(st == st_funcType){


        }
        si = si->next; 
    }
    return true;
}


