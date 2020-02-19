#pragma  once
#include <tuple>
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include "ast.h"


class Parser
{




    public:
        explicit Parser (const std::string &fileName);
        virtual ~Parser () {}
        void printLex();   //词法分析测试函数

    private:
        //词法分析器
        std::tuple<std::string, TOKEN> next(); 
        //哈希表存储关键字
        const std::unordered_map<std::string, TOKEN> keywords; 

        //文件输入流
        std::fstream fs;
        //当前第一行第零列
        int column = 1, line = 1;
        //读入下一个字符 并消耗
        inline char getNextChar() { column++; return static_cast<char>(fs.get());}
        //读入下一个字符 不消耗
        inline char peekNextChar() {return static_cast<char>(fs.peek());}
        //词法分析测试函数
};
