#include "../include/riscvGenerator.h"
#include "../include/intermediateGenerator.h"
#include "../include/symbolTable.h"
#include "../include/utils.h"
#include <fstream>


extern IntermediateGenerator itgenerator;   //四元式产生表

riscvGenerator::riscvGenerator(std::string name){
    this->filename = name;
    //[> 初始化临时寄存器组<]
    Reg = {true, true, false, false, false, false, false, false};

}



void riscvGenerator::printAsmCode(Parser &p) {

    p.printParser();  //打印符号表和四元式
    std::cout << "Starting To Print RiscvCodeFile..." << std::endl;
    std::ofstream out("riscvCodeFile.s");
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
    if(itgenerator.getIntermediateList()[0].getopcode() != FUNDEC) {  //四元式一定以函数开始
        panic("CodeGenError: Wrong Instruction!"); 
    }

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
        /*  只需要维护main函数栈
         *  变量作用域：
         *     1. main函数内部  压栈
         *     2. main函数外部(全局变量) 使用Label
         *  四元式类型：
         *      赋值语句   运算语句  控制语句
         * */
        int fp = 16;
        //计算栈帧
        SymbolItem *head = p.getSymbolTable()->getHead();
        SymbolItem *tail = p.getSymbolTable()->getTail();
        int tmp = 0;
        while(head != tail) {
            if(head->getSt() == st_localType && head->getscope() == "main") {   //寻找main函数中的局部变量
                tmp++; 
            } 
            head = head->next; 
        }
        if(head->getSt() == st_localType && head->getscope() == "main"){
            tmp++; 
        }

        if(tmp%4 != 0) {
            tmp = tmp/4+1; 
        }else {
            tmp = tmp / 4; 
        }
        fp += tmp*16; 
        out << "    .text" << std::endl;
        out << "    .align  1" << std::endl;
        out << "    .global main"  << std::endl;
        out << "    .type   main" << ", @function" << std::endl;
        out << "main:" << std::endl; 
        out << "    addi sp, sp, -" << fp << std::endl;
        out << "    sw s0, " << fp-4 <<"(sp)" << std::endl;
        out << "    addi s0, sp, "<< fp << std::endl;

        /*函数内部翻译*/
        nofun_asmCodeGen(out, p, fp);

        out << "    li a5, 0" << std::endl;
        out << "    mv a0, a5" << std::endl;
        out << "    lw s0, "<< fp-4 << "(sp)" << std::endl;
        out << "    addi sp, sp, "<< fp << std::endl;
        out << "    jr ra" << std::endl;
        out << "    .size   main, .-main" << std::endl;
    }else if(ismain && isfunc){     //有系统函数  无自定义函数
        int fp = 16;
        //计算栈帧
        SymbolItem *head = p.getSymbolTable()->getHead();
        SymbolItem *tail = p.getSymbolTable()->getTail();
        int tmp = 0;
        while(head != tail) {
            if(head->getSt() == st_localType && head->getscope() == "main") {   //寻找main函数中的局部变量
                LocalItem *li = static_cast<LocalItem *>(head); 
                if(li->getisArr()) {
                    tmp += li->getLength(); 
                }else {
                    tmp++; 
                }
            } 
            head = head->next; 
        }
        if(head->getSt() == st_localType && head->getscope() == "main"){
            LocalItem *li = static_cast<LocalItem *>(head);
            if(li->getisArr()) {
                tmp += li->getLength(); 
            }else {
                tmp ++; 
            }
        }

        if(tmp%4 != 0) {
            tmp = tmp/4+1; 
        }else {
            tmp = tmp / 4; 
        }
        fp += tmp*16; 
        //为系统函数分配标签
        if(itgenerator.dataSet.empty()) {
            out << "    .section    .rodata" << std::endl;
        }
        out << "    .align 2" << std::endl; 
        out << ".PL0:" << std::endl;
        out << "    .string \"%d\"" << std::endl;    
        out << "    .text" << std::endl;

        out << "    .align  1" << std::endl;
        out << "    .global main"  << std::endl;
        out << "    .type   main" << ", @function" << std::endl;
        out << "main:" << std::endl; 
        out << "    addi sp, sp, -" << fp << std::endl;
        out << "    sw ra, " << fp-4<< "(sp)" << std::endl;
        out << "    sw s0, "<< fp-8<< "(sp)" << std::endl;
        out << "    addi s0, sp, " << fp << std::endl;

        /*函数内部翻译*/
        nofun_asmCodeGen(out, p, fp);


        out << "    li a5, 0" << std::endl;
        out << "    mv a0, a5" << std::endl;
        out << "    lw ra, " << fp-4<< "(sp)" << std::endl;
        out << "    lw s0, " << fp-8 << "(sp)" << std::endl;
        out << "    addi sp, sp, "<< fp << std::endl;
        out << "    jr ra" << std::endl;
        out << "    .size   main, .-main" << std::endl;
    }else if(!ismain) {  //有系统函数   有自定义函数

        fun_asmCodeGen(out); 

    }

    out << "    .ident  \"GCC: (GNU) 8.3.0\"" << std::endl;
    std::cout << "Print Succeeded!" << std::endl << std::endl << "Closing Complier..." << std::endl;
    return ;
}



void riscvGenerator::nofun_asmCodeGen(std::ofstream &out, Parser &p, int sp) {

    //第零项为main函数
    for(unsigned int i = 1;  i < itgenerator.getIntermediateList().size(); i ++) {

        FourYuanInstr fy = itgenerator.getIntermediateList()[i];
        std::string name;
        itemType it;
        int  isglobal = -1;  // 1 Global  0 local  -1 不存在
        int tmp;  //临时计数器

        /*ASS局部变量*/
        std::string target;
        std::string left;
        std::string right;
        riscvsp = sp + 4;   //运行栈分内存地址从sp下一个地址开始   遇到有效地址 清空
        riscvs0 = riscvsp;
        /*ASS局部变量*/

        SymbolItem *head, *tail;
        switch (fy.getopcode()) {
            case PrintStr:
                name = fy.gettarget();
                out << "    lui a5,%hi(" << name << ")" << std::endl;            
                out << "    addi    a0, a5,%lo(" << name << ")" << std::endl;
                out << "    call printf" << std::endl;
                break;
            case PrintId:
                it = fy.getparat();
                name = fy.gettarget();
                //判断ID是全局还是局部变量
                head =  p.getSymbolTable()->getHead();
                tail =  p.getSymbolTable()->getTail();
                while(head != tail) {
                    if(head->getname() == name && head->getSt() == st_localType) {
                        LocalItem *li = static_cast<LocalItem *>(head);
                        it = li->getIt();
                        if(head->getscope() == "Global"){
                            isglobal = 1; 
                        }else {
                            isglobal = 0;
                        }
                        break;
                    } 
                    head = head->next; 
                } 
                if(head == tail && head->getname() == name && head->getSt() == st_localType) {
                    LocalItem *li = static_cast<LocalItem *>(head);
                    it = li->getIt();
                    if(head->getscope() == "Global") {
                        isglobal = 1;
                    }else {
                        isglobal = 0; 
                    }
                }           
                if(isglobal == 1) {   //全局变量   标签寻址
                    if(it == it_intType) {
                        out << "    lui a5, %hi(" << name << ")" << std::endl;
                        out << "    lw a5, %lo(" << name << ")(a5)"  << std::endl;
                        out << "    mv a1, a5" << std::endl;
                        out << "    lui a5, %hi(.PL0)" << std::endl; 
                        out << "    addi a0, a5, %lo(.PL0)" << std::endl;
                        out << "    call printf" << std::endl;
                    }else if(it == it_charType) {
                        out << "    lui a5, %hi(" << name << ")" << std::endl;
                        out << "    lbu a5, %lo(" << name << ")(a5)"  << std::endl;
                        out << "    mv a0, a5" << std::endl;
                        out << "    call putchar" << std::endl;                         
                    }

                }else if(isglobal == 0) {  //main中的局部变量  在栈中寻址
                    head = p.getSymbolTable()->getHead();
                    tail = p.getSymbolTable()->getTail();         
                    tmp  = 0;
                    while(head != tail) {
                        if(head->getscope() == "main" && head->getSt() == st_localType) {
                            tmp++;
                            if(head->getname() == name) {
                                break; 
                            } 
                        } 
                        head = head->next; 
                    }
                    if(head == tail && head->getscope() == "main" && head->getSt() == st_localType) {
                        if(head->getname() == name) {
                            tmp ++; 
                        } 
                    }
                    tmp = 16 + tmp*4;
                    out << "    lw  a1, -" << tmp << "(s0)" << std::endl;
                    out << "    lui a5, %hi(.PL0)" << std::endl;
                    out << "    addi  a0, a5, %lo(.PL0)"  << std::endl;
                    out << "    call printf" << std::endl;

                }else if(isglobal == -1) {  //表达式
                    //std::cout << fy.getvalue() << std::endl;                     
                    /*
                     * 数组 
                     * 表达式
                     * */

                }


                break;
            case PrintInt:
                out << "    li a1, " << std::stoi(fy.gettarget())<< std::endl;
                out << "    lui a5, %hi(.PL0)" << std::endl; 
                out << "    addi a0, a5, %lo(.PL0)" << std::endl;
                out << "    call printf" << std::endl;
                break;
            case PrintChar:
                out << "    li a0, " << static_cast<int>(fy.gettarget()[0]) << std::endl;
                out << "    call putchar" << std::endl;
                break;
            case ReadInt:
            case ReadChar:
                /*判断变量是全局变量还是局部变量  scanf目前不支持数组读*/
                name = fy.gettarget();
                head = p.getSymbolTable()->getHead();
                tail = p.getSymbolTable()->getTail();
                while(head != tail) {
                    if(head->getname() == name && head->getSt() == st_localType) {
                        if(head->getscope() == "Global") {
                            isglobal = 1;    
                        } else {
                            isglobal = 0; 
                        }
                        break; 
                    }
                    head = head->next; 
                } 
                if(head->getname() == name && head->getSt() == st_localType) {
                    if(head->getscope() == "Global") {
                        isglobal = 1; 
                    }else {
                        isglobal = 0; 
                    } 
                }
                if(isglobal == 1) {  //全局变量
                    out << "    lui a5, %hi(" << name << ")" << std::endl;
                    out << "    addi a1, a5, %lo(" << name << ")" << std::endl;
                    out << "    lui a5, %hi(.PL0)" << std::endl; 
                    out << "    addi a0, a5, %lo(.PL0)" << std::endl;
                    out << "    call scanf" << std::endl;
                }else if(isglobal == 0) {  //局部变量
                    /*
                     * 局部变量在运行栈中完成
                     *   注意压栈的顺序
                     */ 
                    head = p.getSymbolTable()->getHead();
                    tail = p.getSymbolTable()->getTail();         
                    tmp  = 0;
                    while(head != tail) {
                        if(head->getscope() == "main" && head->getSt() == st_localType) {
                            tmp++;
                            if(head->getname() == name) {
                                break; 
                            } 
                        } 
                        head = head->next; 
                    }
                    if(head == tail && head->getscope() == "main" && head->getSt() == st_localType) {
                        if(head->getname() == name) {
                            tmp++; 
                        } 
                    }
                    tmp = 16 + tmp*4;
                    out << "    addi a5, s0, -" << tmp << std::endl;
                    out << "    mv a1, a5" << std::endl;
                    out << "    lui a5, %hi(.PL0)" << std::endl; 
                    out << "    addi a0, a5, %lo(.PL0)" << std::endl;
                    out << "    call scanf" << std::endl;
                }else if(isglobal == -1) {
                    panic("CodeGenError: Wrong Instruction!");
                }
                break;
                /*            case ADD:*/
                //break;
                //case SUB:
                //break;
                //case MUL:
                //break;
                //case DIV:
                //break;
            case ASS:
                /*
                 *  寄存器分配和内存访问策略:
                 *      a2-a7共6个寄存器
                 *      加上从sp指针向下的栈空间
                 */
                target = fy.gettarget();
                left = fy.getleft();
                right = fy.getright();
                if(assCheckType(target, p) == intermediaT) {  //赋值语句是中间变量
                    intermedia_TargetCodeGen(fy, out, riscvsp);
                }else {
                    //赋值语句是内存有效地址， 复制结束之后回复寄存器状态和内存栈空间
                    head = p.getSymbolTable()->getHead();
                    tail = p.getSymbolTable()->getTail();
                    while(head != tail) {
                        if(head->getname() == target && head->getSt() == st_localType) {
                            if(head->getscope() == "Global") {
                                isglobal = 1;    
                            } else {
                                isglobal = 0; 
                            }
                            break; 
                        }
                        head = head->next; 
                    } 
                    if(head->getname() == target && head->getSt() == st_localType) {
                        if(head->getscope() == "Global") {
                            isglobal = 1; 
                        }else {
                            isglobal = 0; 
                        } 
                    }

                    if(isglobal == 1) {  //全局变量
                        nointermedia_globalTargetCodeGen(fy, out, riscvsp); 
                    }else if(isglobal == 0) {  //局部变量
                        nointermedia_localTargetCodeGen(fy, out, riscvsp);
                    }else {
                        panic("CodeGenError: Wrong Instructions!"); 
                    }

                }
                //panic("DEBUG");
                break;
            case JMP:

                break;
            case LABEL:
                break;
            case JT:
                break;
            case JNT:
                break;
            case GT:
                break;
            case GE:
                break;
            case LT:
                break;
            case LE:
                break;
            case ENQ:
                break;
            case BNE:
                break;
            case FUNDEC:
            case FUNCALL:
            case PARAM:
                panic("CodeGenError: Wrong Instruction!");
                break; 
            default:
                return;                
        } 
    }

}




void riscvGenerator::fun_asmCodeGen(std::ofstream &out) {

    for(unsigned int i = 0; i < itgenerator.getIntermediateList().size(); i ++)  {
        FourYuanInstr fy = itgenerator.getIntermediateList()[i]; 
        std::string name;
        out << "TODO " << std::endl;
    }
}


/*赋值变量类型判断*/
riscvGenerator::TYPE riscvGenerator::assCheckType(std::string name, Parser &p) {
    bool isarray = false;
    SymbolItem *head = p.getSymbolTable()->getHead();
    SymbolItem *tail = p.getSymbolTable()->getTail();


    if(name[0] == '$') {
        return intermediaT; 
    }else {
        while(head != tail) {
            if(head->getname() == name && head->getSt() == st_localType) {
                LocalItem *li = static_cast<LocalItem *>(head); 
                if(li->getisArr()) {
                    isarray = true; 
                }
                break;
            } 
            head = head->next;
        }
        if(head == tail && head->getname() == name && head->getSt() == st_localType) {
            LocalItem *li = static_cast<LocalItem *>(head);
            if(li->getisArr()) {
                isarray = true; 
            } 
        }

        if(isarray) {
            return variableArrayT; 
        }else{
            return variableNoArrayT; 
        }
    }
}


/* 数组地址分配
 * name 数组名称  length下标 */
int riscvGenerator::arrayAlloc(std::string name, int length, Parser &p) {

    int res = -1, tmp = 0,len = 0;
    SymbolItem *head = p.getSymbolTable()->getHead();
    SymbolItem *tail = p.getSymbolTable()->getTail();
    while(head != tail) {
        if(head->getscope() == "main" && head->getSt() == st_localType) {
            LocalItem *li = static_cast<LocalItem *>(head); 
            if(li->getisArr() && li->getname() == name){  
                len  = li->getLength();
                break;
            }else {
                tmp ++; 
            }
        } 
        head = head->next;
    }
    if(head == tail && head->getscope() == "main" && head->getSt() == st_localType) {
        LocalItem *li = static_cast<LocalItem *>(head); 
        if(li->getisArr() && li->getname() == name) {
            len = li->getLength(); 
        }
    }

    tmp *= 4; 
    tmp += 20;  //数组的基地址
    res = tmp + 4 * (len - 1 - length);
    return res;
}


/*
 * 寄存器分配   num 中间变量序号
 * */
bool riscvGenerator::registerAlloc(int num) {

    if(!this->Reg[2]) {
        this->Reg[2] = true;
        intermediaToReg.insert({num, 2});
        return true; 
    }else if(!this->Reg[3]){
        this->Reg[3] = true;
        intermediaToReg.insert({num, 3});
        return true; 
    }else if(!this->Reg[4]) {
        this->Reg[4] = true;
        intermediaToReg.insert({num, 4});
        return true; 
    }else if(!this->Reg[5]) {
        this->Reg[5] = true;
        intermediaToReg.insert({num, 5});
        return true; 
    }else if(!this->Reg[6]) {
        this->Reg[6] = true;
        intermediaToReg.insert({num, 6});
        return true; 
    }else if(!this->Reg[7]) {
        this->Reg[7] = true;
        intermediaToReg.insert({num, 7});
        return true; 
    }else {
        return false;      //寄存器组已满
    }
}


/*临时运行栈分配 num 中间变量序号   sp临时栈帧*/
void riscvGenerator::stackAlloc(int num, int &sp) {
    intermediaToStack.insert({num, sp + 4});
}


/*用户内存分配函数*/
std::pair<bool, int > riscvGenerator::addressAlloc(int num, int &riscvsp) {
    bool isreg = true;
    int res = -1; 
    bool isexist = false;    
    /*先查找是否存在*/
    
    std::map<int , int >::iterator iter = intermediaToReg.find(num);
    if(iter != intermediaToReg.end()) {
        isexist = true; 
    }else {
    
        std::map<int , int>::iterator iter = intermediaToStack.find(num);
        if(iter != intermediaToStack.end()) {
            isexist = true; 
        } 
    }

    if(isexist) {
        
    }else {

        /*不存在则分配*/
        if(registerAlloc(num)) {
            res =   intermediaToReg.find(num)->second;   //res就是寄存器号
            isreg = true; 
        }else {
            isreg = false; 
            stackAlloc(num, riscvsp); 
            res = intermediaToStack.find(num)->second;
        }

        if(!isreg && res < riscvs0) {
            panic("CodeGenError: stackoverflow!"); 
        }
    }
    return std::pair<bool, int>(isreg, res);
}


/*
 * 寄存器释放 (将寄存器恢复原值)
 * */
void riscvGenerator::registerFree() {

    this->Reg[2] = false;
    this->Reg[3] = false;
    this->Reg[4] = false;
    this->Reg[5] = false;
    this->Reg[6] = false;
    this->Reg[7] = false;

    /*清空映射表*/
    intermediaToReg.clear();
}


/*临时栈空间释放*/
void riscvGenerator::stackFree() {
    riscvsp = riscvs0;  //清空运行栈空间
    intermediaToStack.clear();  //清空映射表
}

//用户内存释放函数
void riscvGenerator::addressFree() {
    registerFree();
    registerFree();
}




/*赋值汇编生成*/
void riscvGenerator::intermedia_TargetCodeGen(FourYuanInstr &fy, std::ofstream &out, int &sp) {
    out << "    intermedia_TargetCodeGen" << fy.gettarget() <<  " " << fy.getleft() << " " << fy.getright() << " " << fy.getop() << std::endl; 
    int targetRegNum = -1, leftRegNum = -1, rightRegNum = -1;
    int targetStackNum = -1, leftStackNum = -1, rightStackNum = -1;
    bool leftIsconstant = false, rightIsconstant = false;

    if(fy.getissrcArr()) {
        //赋值模型  数组 target = left[right];


    }else {
        //赋值模型   target = left op right

        /*结果*/
        int targetTmpNum = fy.gettarget()[1] - '0';
        std::pair<bool, int> res = addressAlloc(targetTmpNum, sp);
        if(res.first) {
            targetRegNum = res.second;
            targetStackNum = -1;
        }else {
            targetStackNum = res.second; 
            targetRegNum = -1;
        }

        /*左操作数*/
        if(fy.getleft()[0] == '$') {  // 中间变量


        }else if(fy.getleft()[0] >= '0' && fy.getleft()[0] <= '9') {  //常量
            leftIsconstant = true;
            if(registerAlloc(10000)) {
                leftRegNum = intermediaToReg.find(10000)->second; 
            }else {

            }
            out << "    li $" << leftRegNum << ", " << fy.getleft() << std::endl; 


        }else {   //有效地址



        }



        /*右操作数*/
        if(fy.getright()[0] == '$') {   //中间变量



        }else if(fy.getright()[0] >= '0' && fy.getright()[0] <= '9'){  //常量
            rightIsconstant = true;
            if(registerAlloc(20000)) {
                rightRegNum = intermediaToReg.find(20000)->second; 
            }else {


            }
            out << "    li $ " << rightRegNum << ",  " << fy.getright() << std::endl;

        }else {   //有效地址



        }


        /*操作符*/
        char op = fy.getop();
        if(op == '+') {
            if(res.first) { //结果分配到了寄存器

                out << "    addi $" << targetRegNum << ", $" << leftRegNum << ", $" << rightRegNum<< std::endl; 


            }else {

                out << "    addi $a1, $" << leftRegNum << ", $" << rightRegNum<< std::endl; 
                out << "    lw a1, -" << targetRegNum << "(s0)"  << std::endl; 
            }

        }else if(op == '-') {


        }else if(op == '*') {
            out << "    mul $" << targetRegNum << ", $" << leftRegNum << ", $" << rightRegNum<< std::endl; 


        }else if(op == '/') {


        }


        if(leftIsconstant) {
            if(leftRegNum != -1) {
                Reg[leftRegNum] = false;
                leftRegNum = -1;
            }
        } 

        if(rightIsconstant) {
            if(rightRegNum != -1) {
                Reg[rightRegNum] = false; 
                rightRegNum = -1;
            }
        }

        //panic("DEBUG"); 

    }
}


void riscvGenerator::nointermedia_localTargetCodeGen(FourYuanInstr &fy, std::ofstream &out, int &sp) {
    out << "    nointermedia_localTargetCodeGen" << fy.gettarget() << " " << fy.getleft() << " " << fy.getright() << " " << fy.getop() << std::endl;




}


void riscvGenerator::nointermedia_globalTargetCodeGen(FourYuanInstr &fy, std::ofstream &out, int &sp) {
    out << "    nointermedia_globalTargetCodeGen" << fy.gettarget() << " " << fy.getleft() << " "<< fy.getright() << " " << fy.getop() << std::endl;






}






