#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentances(string text);

int main(void)
{
    string text = get_string("Text: ");
    int letters_amount = count_letters(text);
    int words_amount = count_words(text);
    int sentances_amount = count_sentances(text);
    // In order to make float division instad of integer division (which will round an answer) we change type of number to float in division
    float index = 0.0588 * ((float) св letters_amount / words_amount * 100) - 0.296 * ((float) sentances_amount / words_amount * 100) - 15.8;
    int rounded_grade = (int) round(index);
    if (rounded_grade > 16) {
        printf("Grade 16+\n");
    }
    else if (rounded_grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", rounded_grade);
    }
}

int count_letters(string text)
{
    int letters_amount = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        char letter = text[i];
        if ((letter > 64 && letter < 91) || (letter > 96 && letter < 123))
        {
            letters_amount++;
        }
    }
    return letters_amount;
}

int count_words(string text)
{
    int words_amount = 1;
    for (int i = 0; i < strlen(text); i++)
    {
        char letter = text[i];
        if (letter == 32)
        {
            words_amount++;
        }
    }
    return words_amount;
}

int count_sentances(string text)
{
    int sentances_amount = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        char letter = text[i];
        if (letter == 33 || letter == 46 || letter == 63)
        {
            sentances_amount++;
        }
    }
    return sentances_amount;
}