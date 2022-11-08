#include <cs50.h>
#include <stdio.h>

// Prints given amount of symbols
void print_symbols(char symbol, int amount)
{
    for (int i = 0; i < amount; i++)
    {
        printf("%c", symbol);
    }
}

int main(void)
{
    int height;

    // Ask for pyramid height. It should be between 1 and 8, otherwise asks untill get right integer.
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // Loop for printing pyramid
    for (int line = 1; line <= height; line++)
    {
        int spaces = height - line;

        print_symbols(' ', spaces);
        print_symbols('#', line);
        print_symbols(' ', 2);
        print_symbols('#', line);
        printf("\n");
    }
}