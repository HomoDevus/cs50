from cs50 import get_int


def is_valid(card_number):
    sum = 0

    for i in range(card_number_len):
        last_digit = card_number % 10
        if (i % 2 == 0):
            sum += last_digit
        else:
            multiplied = last_digit * 2
            while multiplied != 0:
                sum += multiplied % 10
                multiplied = multiplied // 10

        card_number = card_number // 10

    return sum % 10 == 0


def get_first_digits(input, amount_of_digits):
    while input >= 10 ** amount_of_digits:
        input //= 10
    return input
    

while True:
    card_number = get_int("Type card number: ")
    card_number_len = len(str(card_number))
    if (10 <= card_number_len <= 16):
        break
    print("Card number should be of length between 13 and 16 digits")

if (is_valid(card_number)):
    first_two_digits = get_first_digits(card_number, 2)
    first_digit = get_first_digits(card_number, 1)

    if card_number_len == 15 and (first_two_digits == 34 or first_two_digits == 37):
        print("AMEX")
    elif card_number_len == 16 and (50 < first_two_digits < 56):
        print("MASTERCARD")
    elif (card_number_len == 13 or card_number_len == 16) and first_digit == 4:
        print("VISA")
    else:
        print("INVALID")
else:
    print("INVALID")