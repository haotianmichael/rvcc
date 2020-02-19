#include <iostream>
#include <algorithm>
#include <string>
#include "../include/parser.h"


using namespace std;


int main(int argc, char *argv[])
{
    if(argc < 2) cout << "ERROR" << endl;
    Parser p(argv[1]);
    p.printLex();
    return 0;
}
