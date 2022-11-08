#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Get length of long number
int number_length(long number)
{
    int l = 1;
    while (number > 9)
    {
        l++;
        number /= 10;
    }
    return l;
}

// Checks if card format is valid using Luhn's algorithm
bool is_card_valid(long card_number)
{
    int card_number_length = number_length(card_number);
    int sum = 0;

    // Start loop from 0. Add to sum multiplied even and untouched odd. Go from end to start, because first digit at the end shouldn't be multiplied.
    for (int i = 0; i < card_number_length; i++)
    {
        if (i % 2 == 0)
        {
            sum += card_number % 10;
        }
        else
        {
            int multiplied = (card_number % 10) * 2;
            while (multiplied != 0)
            {
                sum += multiplied % 10;
                multiplied = floor(multiplied / 10);
            }
        }

        card_number = floor(card_number / 10);
    }

    return sum % 10 == 0;
}

// Returns first n digits of long number
int get_first_digits(long input, int amount_of_digits)
{
    long local = input;

    while (local >= pow(10, amount_of_digits))
    {
        local /= 10;
    }

    return local;
}

int main(void)
{
    long card_number;
    int card_number_length;

    do
    {
        card_number = get_long("Number: ");
        card_number_length = number_length(card_number);
    }
    while (card_number_length < 10 || card_number_length > 16); // Has between 13 and 16 digits

    if (is_card_valid(card_number))
    {
        int first_two_digits = get_first_digits(card_number, 2);
        int first_digit = get_first_digits(first_two_digits, 1);

        if (card_number_length == 15 && (first_two_digits == 34 || first_two_digits == 37))
        {
            printf("AMEX\n");
        }
        else if (card_number_length == 16 && (first_two_digits >= 51 && first_two_digits <= 55))
        {
            printf("MASTERCARD\n");
        }
        else if ((card_number_length == 13 || card_number_length == 16) && first_digit == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}