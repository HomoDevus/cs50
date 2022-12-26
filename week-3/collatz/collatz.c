#include <cs50.h>
#include <stdio.h>

int collatz(int num);

int main(void)
{
    int num = get_int("Num: ");
    int ans = collatz(num);
    printf("%i\n", ans);
}

int collatz(int num)
{
    if (num == 1) {
        return 0;
    }

    if (num % 2 == 0)
    {
        return 1 + collatz(num / 2);
    }
    else {
        return 1 + collatz(3 * num + 1);
    }
}