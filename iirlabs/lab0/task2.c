#include <stdio.h>

int main(void)
{
    int a, b;
    scanf("%d %d", &a, &b);

    int mul = a * b;
    printf("%d\n", mul);

    int mod1 = a % b;
    int mod2 = b % a;
    printf("%d %d\n", mod1, mod2);
}