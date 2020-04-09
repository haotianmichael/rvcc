#include "../include/intermediateGenerator.h"

IntermediateGenerator itgenerator;


void IntermediateGenerator::pushIntermediateItem(FourYuanInstr& tmp) {

    this->intermediateList.push_back(tmp);
    return;
}


void IntermediateGenerator::printTmpItem() {

    for(unsigned int i = 0; i < this->intermediateList.size(); i ++)  {
        FourYuanInstr tmp = this->intermediateList[i]; 
        switch (tmp.getopcode()) {
            case PUSH:
                
                break;
            case FUNCALL:
                break;

            case FUNDEC:
                break;
            case RET:
                break;
            case PARAM:
                break;
            case BEGIN:
                break;
            case END:
                break;
            
            default :break;
                
        }
    
    }


    return ;
}


