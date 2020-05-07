#include "../include/riscvGenerator.h"
#include "../include/intermediateGenerator.h"
#include "../include/symbolTable.h"
#include "../include/utils.h"
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
    if(!itgenerator.dataSet.empty()) {
        out << "    .section    .rodata" << std::endl;
    }
    std::map<std::string, std::string>::iterator iter;  //遍历.data
    for(iter = itgenerator.dataSet.begin(); iter != itgenerator.dataSet.end(); iter++) {
        out << "    .align  2" << std::endl; 
        out << iter->second <<  ":" << std::endl;
        out << "    .string \"" << iter->first << "\"" << std::endl;
    }

    /* 函数模型
     * 1. 有main函数  无自定义函数  无系统函数(指printf scanf)   ismain == true && isfunc == false
     * 2. 有main函数  无自定义函数  有系统函数(指printf scanf)   ismain == true && isfunc == true
     * 3. 有main函数  有自定义函数  有系统函数(指printf scanf)   ismain == false 
     * 
     */
    out << "    .text" << std::endl;
    bool ismain = true, isfunc = false;
    if(itgenerator.getIntermediateList()[0].gettarget() == "main") {  
        for(unsigned int i = 0; i < itgenerator.getIntermediateList().size(); i ++) {
            FourYuanInstr tmp = itgenerator.getIntermediateList()[i];
            fourYuanOpcode fy = tmp.getopcode();
            if(fy == PrintId || fy == PrintInt || fy == PrintStr || fy == PrintChar || fy == ReadInt || fy == ReadChar) {
                isfunc = true;
            }
        }
    }else if(itgenerator.getIntermediateList()[0].gettarget() != "main") {
        ismain = false; 
    }

    if(ismain && !isfunc) {   //无系统函数  无自定义函数
        /*只需要维护main函数栈*/
        out << "    .align  1" << std::endl;
        out << "    .global main"  << std::endl;
        out << "    .type   main" << ", @function" << std::endl;
        out << "main:" << std::endl; 
        out << "    addi sp, sp, -16" << std::endl;
        out << "    sw s0, 12(sp)" << std::endl;
        out << "    addi s0, sp, 16" << std::endl;

        /*函数内部翻译*/
        nofun_asmCodeGen();

        out << "    li a5, 0" << std::endl;
        out << "    mv a0, a5" << std::endl;
        out << "    lw s0, 12(sp)" << std::endl;
        out << "    addi sp, sp, 16" << std::endl;
        out << "    jr ra" << std::endl;
        out << "    .size   main, .-main" << std::endl;
    }else if(ismain && isfunc){     //有系统函数  无自定义函数

        out << "    .align  1" << std::endl;
        out << "    .global main"  << std::endl;
        out << "    .type   main" << ", @function" << std::endl;
        out << "main:" << std::endl; 
        out << "    addi sp, sp, -16" << std::endl;
        out << "    sw ra, 12(sp)" << std::endl;
        out << "    sw s0, 8(sp)" << std::endl;
        out << "    addi s0, sp, 16" << std::endl;

        for(unsigned int i = 1; i < itgenerator.getIntermediateList().size(); i ++) {
            FourYuanInstr   tmp = itgenerator.getIntermediateList()[i]; 
        }
        /*函数内部翻译*/
        nofun_asmCodeGen();


        out << "    li a5, 0" << std::endl;
        out << "    mv a0, a5" << std::endl;
        out << "    lw ra, 12(sp)" << std::endl;
        out << "    lw s0, 8(sp)" << std::endl;
        out << "    addi sp, sp, 16" << std::endl;
        out << "    jr ra" << std::endl;
        out << "    .size   main, .-main" << std::endl;
    }else if(!ismain) {  //有系统函数   有自定义函数
    
       fun_asmCodeGen(); 
    
    }
    
    out << "    .ident  \"GCC: (GNU) 8.3.0\"" << std::endl;
    return ;
}



void nofun_asmCodeGen() {

}




void fun_asmCodeGen() {


}

