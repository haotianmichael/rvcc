#pragma  once
#include "fouryuanInstr.h"
#include <vector>



class intermediateGenerator
{
public:
    intermediateGenerator ();
    virtual ~intermediateGenerator ();

private:
    
    std::vector<FourYuanInstr> intermediateList;   //四元式生成器
};

