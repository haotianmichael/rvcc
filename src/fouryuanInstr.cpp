#include "../include/fouryuanInstr.h"

//构造函数
FourYuanInstr::FourYuanInstr(FourYuanOpcode opcode, std::string left, std::string right) {
    __opcode = opcode;
    __left = left;
    __right = right;
}


std::string FourYuanInstr::getleft() { return this->__left;}
std::string FourYuanInstr::getright() { return this->__right;}
FourYuanOpcode FourYuanInstr::getopcode() { return this->__opcode;}


//打印符号表
void FourYuanInstr::printFourYuan() {

    std::string opcode = "";
    switch (__opcode) {
        case ADD:
            opcode = "ADD";
            break;
        case SUB:
            opcode = "SUB";
        case MUL:
            opcode = "MUL";
            break;
        case DIV:
            opcode = "DIV";
            break;
        case NEG:
            opcode = "NEG";
            break;
        case ASS:
            opcode = "ASS";
            break;
        case JMP:
            opcode = "JMP";
            break;
        case LABEL:
            opcode = "LABEL";
            break;
        case JT:
            opcode = "JT";
            break;
        case JNT:
            opcode = "JNT";
            break;
        case GT:
            opcode = "GT";
            break;
        case GE:
            opcode = "GE";
            break;
        case LT:
            opcode = "LT";
            break;
        case ENQ:
            opcode = "ENQ";
            break;
        case BNE:
            opcode = "BNE";
            break;
        default:
            opcode = "NULL";
            break;
    }


    


    return ;
}
