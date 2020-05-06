#include "../include/riscvGenerator.h"
#include "../include/intermediateGenerator.h"
#include "../include/symbolTable.h"
#include <fstream>


extern IntermediateGenerator itgenerator;   //四元式产生表

void riscvGenerator::printAsmCode(Parser &p) {
    
    p.printParser();  //打印符号表和四元式

    std::ofstream out("riscvcodeFile.s");
    out << "    .file  " << "\"" << this->filename << "\"" << std::endl;
    out << "    .option nopic" << std::endl; 

    /*全局变量 .srodataa  变量 常量 数组*/
    out << "    .text" << std::endl;
    SymbolItem *head = p.getSymbolTable()->getHead();
    SymbolItem *tail = p.getSymbolTable()->getTail();
    int flag = false;
    while(head != tail) {
        if(!(head->getSt() == st_localType && head->getscope() == "Global")) { 
            break; 
        } 
        LocalItem *li = static_cast<LocalItem *>(head);
        if(li->getisArr()) {  //数组
            //名字  类型
            std::string name = li->getname();
            itemType it = li->getIt();
            int length = li->getLength();
            if(it == it_intType) {
                out << "    .comm   " << name << ","<< length*4 << ",4" << std::endl;
            }else if(it == it_charType) {
                out << "    .comm   " << name << "," << length << ",4" << std::endl;
            }
        
        }else {
            if(li->getLm() == lm_constant) {  //常量
                // 名字   数据类型  值
                std::string name = li->getname();
                itemType it = li->getIt();
                out << "    .globl  " << name  << std::endl;
                if(it == it_intType) {
                    int value = li->getInteger();
                    if(!flag) {
                        out << "    .section    .srodata,\"a\"" << std::endl;;
                        flag = true; 
                    }
                    out << "    .align  2" << std::endl; 
                    out << "    .type   " << name << ", @object" << std::endl;
                    out << "    .size   " << name << ", 4" << std::endl;
                    out << name << ":" << std::endl;
                    out << "    .word   " << value << std::endl;

                }else if(it == it_charType) {
                    int cvalue = li->getCharacter();
                    out << "    .type   " << name << ", @object" << std::endl;
                    out << "    .size   " << name << ", 1" << std::endl;
                    out << name << ":" << std::endl;
                    out << "    .byte   " << cvalue << std::endl;
                }
            
            }else if(li->getLm() == lm_variable) {  // 变量
                //名字  类型
                std::string name = li->getname();
                itemType it = li->getIt();
                if(it == it_intType) {
                    out << "    .comm   " << name << ",4,4" << std::endl;
                }else if(it == it_charType) {
                    out << "    .comm   " << name << ",1,1" << std::endl; 
                }
            }
        }
        head = head->next;
    } 

    /*.rodata*/
    out << "    .section    .rodata" << std::endl;
    std::map<std::string, std::string>::iterator iter;  //遍历.data
    for(iter = itgenerator.dataSet.begin(); iter != itgenerator.dataSet.end(); iter++) {
        out << "    .align  2" << std::endl; 
        out << iter->second <<  ":" << std::endl;
        out << "    .string\"" << iter->first << "\"" << std::endl;
    }

    out << "    .text" << std::endl;
    /*函数*/







    return ;
}
