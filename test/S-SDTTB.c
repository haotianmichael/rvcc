#include <stdio.h>

const int cosa = 10;
const int cosb = 15;
int array[10];


int foo(int a, int b) {
    int key;
    if(a + b > cosa) {
        key = a + b;
        return (key);
    }else {
        array[3] =  a - b;
        return (array[3]);
    }
}


int partion(int q[], int l, int r) {

    int key, j, i, tmpA;
    key = l;
    j = r;
    i = l;
    while(i < j) {

        while(q[i] < q[key]) {
            i = i + 1; 
        }

        while(q[j] > q[key]) {
            j = j - 1; 
        } 

        if(i < j){
            tmpA = q[j];
            q[j] = q[i];
            q[i] = tmpA;
        }else {
            ; 
        }

    }

    q[i] = q[key];
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
    int i; 
    i = 0;
    while(i < 5) {
        array[i] = (5 - i);
        i  = i + 1;
    }
    quickSort(array, 0, 4);
    i = 0;
    while(i < 5) {
        printf("array[i]");
        i  = i + 1;
    }

    return (0);
}



