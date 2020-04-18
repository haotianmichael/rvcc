#include "../include/symbolTable.h"
#include "../include/parser.h"
#include "../include/utils.h"
#include <fstream>

/*__symbolItem*/
SymbolItem::SymbolItem(symbolType st, std::string scope, std::string name) {
    _st = st;
    _scope = scope; 
    _name = name;
    next = NULL;
    prev = NULL;
}

/*symbolTable*/
//检查是否可以填表
bool SymbolTable::ispushSymbolItem(std::string scope, std::string itemname) {

    SymbolItem *si = __symbolItemHead;
    while(si != NULL) {
        if(si->getname() == itemname && si->getscope() == scope){
            return false;     
        } 
        si = si->next; 
    }
    return true;
}

//localItem 整形
bool SymbolTable::pushSymbolItem(std::string scope, std::string itemname, localMold mod, itemType it, int value) {
    if(!ispushSymbolItem(scope, itemname)) {
        panic("RuntimeError: duplicate symbolTable definition"); 
        return false;
    }

    LocalItem *localitem = new LocalItem(st_localType, scope, itemname);
    localitem->setLm(mod);    //常量  变量  参数
    localitem->setIt(it);   //整形   
    localitem->setInteget(value);
    //插入符号表 尾插法
    if(__symbolItemHead == NULL && __symbolItemTail == NULL) {
        __symbolItemHead = localitem;   
        __symbolItemTail = localitem;
        __symbolItemSize++;
    }else if(__symbolItemHead != NULL && __symbolItemTail != NULL){
        __symbolItemTail->next = localitem;
        localitem->prev = __symbolItemTail;
        __symbolItemTail = localitem;
        __symbolItemSize++;
    }else {
        panic("SymbolTableError: pushItem not allowed"); 
    }

    return true;
}


//localItem 字符类型
bool SymbolTable::pushSymbolItem(std::string scope, std::string itemname, localMold mod, itemType it, char value) {
    if(!ispushSymbolItem(scope, itemname)) {
        panic("RuntimeError: duplicate symbolTable definition"); 
        return false;
    }

    LocalItem *localitem = new LocalItem(st_localType, scope, itemname);
    localitem->setLm(mod);   //常量   变量 参数
    localitem->setIt(it);   //数据类型
    localitem->setCharacter(value);
    //this->__symbolItem.push_back(localitem);
    if(__symbolItemHead == NULL && __symbolItemTail == NULL) {
        __symbolItemHead = localitem; 
        __symbolItemTail = localitem;
        __symbolItemSize++;
    }else if(__symbolItemHead != NULL && __symbolItemTail != NULL){
        __symbolItemTail->next = localitem;
        localitem->prev = __symbolItemTail;
        __symbolItemTail = localitem;
        __symbolItemSize++;
    }else {
        panic("SymbolTableError: pushItem not allowed"); 
    }
    return true;
}

//locaItem 数组类型
bool SymbolTable::pushSymbolItem(std::string scope, std::string itemname, localMold mod, itemType it_array, itemType arrayType, int length) {
    if(!ispushSymbolItem(scope, itemname)) {
        panic("RuntimeError: duplicate symbolTable definition"); 
        return false;
    }

    LocalItem *localitem = new LocalItem(st_localType, scope, itemname); 
    localitem->setLm(mod);
    localitem->setIt(arrayType);
    localitem->setLenght(length);

    if(it_array == it_arrayType) {
        localitem->setisArr(true); 
    }
    if(__symbolItemHead == NULL && __symbolItemTail == NULL) {
        __symbolItemHead = localitem; 
        __symbolItemTail = localitem;
        __symbolItemSize++;
    }else if(__symbolItemHead != NULL && __symbolItemTail != NULL){
        __symbolItemTail->next = localitem;
        localitem->prev = __symbolItemTail;
        __symbolItemTail = localitem;
        __symbolItemSize++;
    }else {
        panic("SymbolTableError: pushItem not allowed"); 
    }

    return true;
}


//funcitem
bool SymbolTable::pushSymbolItem(std::string scope, std::string name, funcReturnType frt) {
    if(!ispushSymbolItem(scope, name)){
        panic("RuntimeError: duplicate symbolTable definition"); 
        return false;
    }

    FuncItem *funcitem = new FuncItem(st_funcType, scope, name);
    funcitem->setReturnType(frt);
    //this->__symbolItem.push_back(funcitem);
    if(__symbolItemHead == NULL && __symbolItemTail == NULL) {
        __symbolItemHead = funcitem; 
        __symbolItemTail = funcitem;
        __symbolItemSize++;
    }else if(__symbolItemHead != NULL && __symbolItemTail != NULL){
        __symbolItemTail->next = funcitem;
        funcitem->prev = __symbolItemTail;
        __symbolItemTail = funcitem;
        __symbolItemSize++;
    }else {
        panic("SymbolTableError: pushItem not allowed"); 
    }
    return true;
}


//打印符号表
bool SymbolTable::printTable() {

    std::ofstream out("symbloTableFile.txt");
    out.setf(std::ios::left);   //打印左对齐
    SymbolItem *si = __symbolItemHead;
    if(si == NULL) {
        return false; 
    }
    /*标题  格式打印14字符*/
    out.width(14);
    out << "symbolType";
    out.width(14);
    out << "name";
    out.width(14);
    out << "scope";
    out.width(14);
    out << "localMold";
    out.width(14);
    out << "itemType";
    out.width(14);
    out << "value";
    out.width(14);
    out << "isArr";
    out.width(14);
    out << "arrLength";
    out.width(14);
    out << "funcReturnType";
    out << std::endl;



    while(si != NULL) {
        symbolType st = si->getSt();
        if(st == st_localType) {
            LocalItem *li = static_cast<LocalItem *>(si); 
            std::string name = li->getname();  //名称
            std::string scope = li->getscope();  //函数作用域
            localMold lm = li->getLm();  //常量 变量
            itemType it = li->getIt();
            int value = ERROR_CODE;
            char cvalue = ' ';
            out.width(14);
            out << "localType";
            out.width(14);
            out << name; 
            out.width(14);
            out << scope;
            if(lm == lm_constant) {
                out.width(14);
                out << "constant";
            }else if(lm == lm_variable) {
                out.width(14);
                out << "variable"; 
            }else if(lm == lm_parameter) {
                out.width(14);
                out << "parameter"; 
            }
            if(it == it_intType && lm == lm_constant) {
                value = li->getInteger(); 
                out.width(14);
                out << "int";
                out.width(14);
                out << value;
            }else if(it == it_charType && lm == lm_constant){
                cvalue = li->getCharacter(); 
                out.width(14);
                out << "char";
                out.width(14);
                out << cvalue;
            }else if(it == it_intType && (lm == lm_variable || lm == lm_parameter)) {
                out.width(14);
                out << "int";
                out.width(14);
                out << "--------";
            }else if(it == it_charType && (lm == lm_variable || lm == lm_parameter)) {
                out.width(14);
                out << "char";
                out.width(14);
                out << "--------";
            }


            if(li->getisArr()) {
                out.width(14);
                out << "YES";
                out.width(14);
                if(li->getLength() == ERROR_CODE) {
                    out << "--------";
                }else {
                    out << li->getLength(); 
                }
            }else{
                out.width(14);
                out << "NO"; 
                out.width(14);
                out << "--------";
            }
            out.width(14);
            out << "--------";
            out << std::endl;

            /*
               }else if(st == st_arrayType){
               ArrayItem *ai = static_cast<ArrayItem *>(si);
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
             */

        }else if(st == st_funcType){
            FuncItem *fi = static_cast<FuncItem *>(si);
            std::string scope = fi->getscope();
            std::string name = fi->getname();
            funcReturnType fr = fi->getReturnType();

            out.width(14);
            out << "funcType";
            out.width(14);
            out << name; 
            out.width(14);
            out << scope;
            out.width(14);
            out << "--------";
            out.width(14);
            out << "--------";
            out.width(14);
            out << "--------";
            out.width(14);
            out << "--------";
            out.width(14);
            out << "--------";
            out.width(14);
            if(fr == frt_intType) {
                out << "frt_intType"; 
            
            }else if(fr == frt_charType) {
                out << "frt_voidType";
            
            }else if(fr == frt_voidType) {
                out << "frt_voidType";
            
            }
            out << std::endl;
        }
        si = si->next; 
    }
    return true;
}


