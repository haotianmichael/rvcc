#pragma  once
#include "fouryuanInstr.h"
#include <vector>



class IntermediateGenerator
{
    public:
        IntermediateGenerator (){};
        virtual ~IntermediateGenerator (){};
        void  pushIntermediateItem(FourYuanInstr &);  //填写四元式
        void  printTmpItem();   //输出四元式

    private:

        std::vector<FourYuanInstr> intermediateList;   //四元式生成器
};

