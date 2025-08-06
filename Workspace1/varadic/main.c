#include <stdarg.h>
#include <stdio.h>

int sum(int count, ...)
{
    va_list args;
    va_start(args, count);

    int total = 0;
    for(int i = 0; i < count; i++) {
        total += va_arg(args, int);
    }

    va_end(args);
    return total;
}
int diff(int count, ...)
{
    va_list arg;
    va_start(arg, count);
    int a = va_arg(arg, int);
    double b = va_arg(arg, double);
    return a - b;
}

int main()
{
    int result1 = sum(3, 10, 20, 30);
    int result2 = sum(5, 1, 2, 3, 4, 5);
    int result3 = diff(2, 1, 3.0);
    printf("Result 1: %d\n", result1);
    printf("Result 2: %d\n", result2);
    printf("result 3: %d\n", result3);

    return 0;
}
