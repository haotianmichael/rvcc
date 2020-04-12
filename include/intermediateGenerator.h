#pragma  once
#include "fouryuanInstr.h"
#include <vector>



class IntermediateGenerator
{
    /*中间代码生成*/
    public:
        IntermediateGenerator (){};
        virtual ~IntermediateGenerator (){};
        void  pushIntermediateItem(FourYuanInstr &);  //填写四元式
        void  printTmpItem();   //输出四元式
        void pushDataItem(std::string);     //添加.data域元素




    private:

        std::vector<std::string> dataSet;   //中间代码 .data域
        std::vector<FourYuanInstr> intermediateList;   //四元式生成器
};

