#include "../include/fouryuanInstr.h"


//打印四元式
void FourYuanInstr::printFourYuan() {

    std::string opcode = "";
    switch (__opcode) {
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
