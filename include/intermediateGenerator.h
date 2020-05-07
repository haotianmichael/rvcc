#pragma  once
#include "fouryuanInstr.h"
#include <vector>
#include <map>



class IntermediateGenerator
{
    public:
        /*中间代码生成*/
        IntermediateGenerator (){};
        virtual ~IntermediateGenerator (){};
        void  pushIntermediateItem(FourYuanInstr &);  //填写四元式
        void  printTmpItem();   //输出四元式
        void pushDataItem(std::string);     //添加.data域元素
        std::map<std::string, std::string> dataSet;   //中间代码 .data域  无重复

        /*返回四元式生成器*/
        std::vector<FourYuanInstr> getIntermediateList() { return this->intermediateList;}

    private:

        std::vector<FourYuanInstr> intermediateList;   //四元式生成器
};

