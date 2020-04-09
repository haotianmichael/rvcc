#include <iostream>
#include <algorithm>
#include <string>
#include "../include/parser.h"
#include "../include/intermediateGenerator.h"
#include "../include/riscvGenerator.h"

using namespace std;
extern IntermediateGenerator itgenerator;

int main(int argc, char *argv[])
{
    if(argc < 2) cout << "ERROR" << endl;
    Parser p(argv[1]);   //语法分析器

    p.printParser();
    return 0;
}
