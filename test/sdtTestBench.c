#include <stdio.h>

const int cosa = 10;
const int cosb = 15;
int array[10];


int foo(int a, int b) {
    int vara;
    if(a + b > cosa) {
        vara = a + b;
        return (vara);
    }else {
        array[3] =  a - b;
        return (array[3]);
    }
}


int partion(int q[], int l, int r) {

    int key, j, i, tmpA, tmpB;
    key = l;
    j = r;
    i = l;
    while(i != j) {

        while(q[j] > q[key] && i < j) {
            j = j - 1; 
        } 

        while(q[i] < q[key] && i < j) {
            i = i + 1; 
        }

        if(i < j){
            tmpA = q[j];
            q[j] = q[i];
            q[i] = tmpA;
        }else {
            ; 
        }

    }

    tmpB = q[key];
    q[key] = q[i];
    q[i] = tmpB;

    return (i);

}

void quickSort(int q[], int l, int r) {

    int x;
    if(l < r) {
        x =  partion(q, l, r);
        quickSort(q, l, x);
        quickSort(q, x + 1, r);
    }else {
        ; 
    }
}

int main() {

    return (0);
}



