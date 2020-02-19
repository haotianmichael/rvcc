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
        explicit Parser (std::string fileName);
        virtual ~Parser ();

    private:
        //词法分析器
        std::tuple<std::string, TOKEN> next(); 
        //哈希表存储关键字
        const std::unordered_map<std::string, TOKEN> keywords; 

        //文件输入流
        std::fstream fs;
        //当前第一行第零列
        int column = 0, line = 1;
        //读入下一个字符
        char getNextChar() {
            column++;
            return static_cast<char>(fs.get());        
        }
};
