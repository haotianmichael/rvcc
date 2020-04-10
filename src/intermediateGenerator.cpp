#include "../include/intermediateGenerator.h"
#include "../include/utils.h"
#include <fstream>

IntermediateGenerator itgenerator;


void IntermediateGenerator::pushIntermediateItem(FourYuanInstr& tmp) {

    this->intermediateList.push_back(tmp);
    return;
}


void IntermediateGenerator::printTmpItem() {

    std::ofstream out("tmpCodeFile.txt");
    for(unsigned int i = 0; i < this->intermediateList.size(); i ++)  {
        FourYuanInstr tmp = this->intermediateList[i]; 
        switch (tmp.getopcode()) {
            case PUSH:
                out << "Push " << tmp.gettarget() << std::endl;        
                break;
            case FUNCALL:
                out << "Call " << tmp.gettarget() << std::endl;        
                break;
            case ASS:
                if(tmp.getistargetArr()) {
                    //赋值数组
                
                
                
                }else {
                
                
                
                } 
                break;
            case LABEL:
                out << tmp.gettarget() << ":" << std::endl;
                break;
            case FUNDEC:
                if(tmp.getfunct() == frt_voidType) {
                    out << "void "  << tmp.gettarget() << "()" << std::endl;
                }else if(tmp.getfunct() == frt_intType) {
                    out << "int "  << tmp.gettarget() << "()" << std::endl;
                }else if(tmp.getfunct() == frt_charType) {
                    out << "char "  << tmp.gettarget() << "()" << std::endl;
                } 
                break;
            case PARAM:
                if(tmp.getparat() == it_intType) {
                    out << "Param int " << tmp.gettarget() << std::endl;
                }else if(tmp.getparat() == it_charType) {
                    out << "Param char " << tmp.gettarget() << std::endl; 
                }
                break;
            case JMP:
                out <<  "Jump " << tmp.gettarget() << std::endl;
                break;
            case JT:
                out << "JT " << tmp.getleft() << " " << tmp.gettarget() << std::endl;
                break;
            case JNT:
                out << "JNT " << tmp.getleft() << " " << tmp.gettarget() << std::endl;
                break;
            case GT:
                out << "GT " << tmp.getleft() << " " << tmp.gettarget() << std::endl;
                break;
            case GE:
                out << "GE " << tmp.getleft() << " " << tmp.gettarget() << std::endl;
                break;
            case LT:
                out << "LT " << tmp.getleft() << " " << tmp.gettarget() << std::endl;
                break;
            case LE:
                out << "LE " << tmp.getleft() << " " << tmp.gettarget() << std::endl;
                break;
            case ENQ:
                out << "ENQ " << tmp.getleft() << " " << tmp.gettarget() << std::endl;
                break;
            case BNE:
                out << "BNE " << tmp.getleft() << " " << tmp.gettarget() << std::endl;
                break;
            case ReadInt:
                out << "Read Char " << tmp.gettarget() << std::endl;
                break;
            case ReadChar:
                out << "Read Int " << tmp.gettarget() << std::endl;
                break;
            case PrintStr:
                out << "Print string " << "\"" << tmp.gettarget() << "\"" << std::endl;
                this->pushDataItem(tmp.gettarget());
                break;
            case PrintInt:
                out << "Print int " << sToi(tmp.gettarget()) << std::endl;
                break;
            case PrintChar:
                if(tmp.gettarget() == "\n") {
                    out << "New Line" << std::endl; 
                }else {
                    out << "Print char \'" << tmp.gettarget()[0] << "\'" << std::endl;
                }
                break;
            case PrintId:
                out << "Print id " << tmp.gettarget() << std::endl;
                break;
            case ReturnChar:
                out << "Ret char \'" << tmp.gettarget()[0] << std::endl;
                break;
            case ReturnInt:
                out << "Ret int " << sToi(tmp.gettarget()) << std::endl;
                break;
            case ReturnEmpty:
                out << "Ret " << std::endl;
                break;
            case ReturnId:
                out << "Ret id " << tmp.gettarget() << std::endl;
                break;
            default :break;
        }
    }
    out.close();
    return;
}


