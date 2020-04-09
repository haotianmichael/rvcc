#pragma  once
#include "fouryuanInstr.h"
#include <vector>



class IntermediateGenerator
{
public:
    IntermediateGenerator (){};
    virtual ~IntermediateGenerator (){};
    void  pushIntermediateItem(FourYuanInstr &);

private:
    
    std::vector<FourYuanInstr> intermediateList;   //四元式生成器
};

