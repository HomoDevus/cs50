#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Asks your name in terminal
    string name = get_string("What's your name? ");
    // Prints hello and your name   
    printf("hello, %s\n", name);
}