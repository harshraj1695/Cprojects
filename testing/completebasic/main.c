#include <stdio.h>

void func3() {
    int *p = NULL;
    *p = 10;  
}

void func2() {
    func3();
}

void func1() {
    func2();
}

int main() {
    func1();
    return 0;
}
