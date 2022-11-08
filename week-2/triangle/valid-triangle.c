#include <stdio.h>
#include <cs50.h>

bool valid_triangle(int a, int b, int c);

int main(void)
{
    bool is_triangle = valid_triangle(get_int("a: "), get_int("b: "), get_int("c: "));
    if (is_triangle)
    {
        printf("It is triangle\n");
    }
    else
    {
        printf("It is not a triangle\n");
    }
}

bool valid_triangle(int a, int b, int c)
{
    if (a > b && a > c)
    {
        return a > b + c;
    }
    else if (b > a && b > c)
    {
        return b > a + c;
    }
    else
    {
        return c > a + b;
    }
}

// int max(int numbers[])
// {
//     int max_val = -INFINITY;
//     int size_of_arr = sizeof(numbers);

//     for (int i = 0; i < size_of_arr; i++)
//     {
//         if (numbers[i] > max_val)
//         {
//             max_val = numbers[i];
//         }
//     }

//     return max_val;
// }