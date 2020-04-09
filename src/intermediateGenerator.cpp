#include "../include/intermediateGenerator.h"

IntermediateGenerator itgenerator;


void IntermediateGenerator::pushIntermediateItem(FourYuanInstr& tmp) {

    this->intermediateList.push_back(tmp);
    return;
}




