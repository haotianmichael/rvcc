#include <stdio.h>


const int N = 100010, M = 110;
int a, b, c, d;


int fun(char a, int b, int c[]) {

    int vara, varb;
    vara = a + c[0];
    varb = b;

    if(vara > varb) {
        printf("haotian is a good man\n");
    }else {
        printf("he have bad life\n");
    }

    return (vara - c[0]);
}



int main() {

    int mainvara, mainavrb, mainvarc[10];
    mainvara = 10;
    mainavrb = 80;
    mainvarc[0] = 10;

    while(mainvara < M) {
        a = fun(mainvara, mainavrb, mainvarc); 
        mainvara  = a + mainvarc[0]; 
    }
    return (0);
}
