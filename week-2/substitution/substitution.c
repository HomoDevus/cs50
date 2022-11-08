#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

string str_to_lower(string str);
bool is_alph_str(string str);
bool has_every_letter(string str);
string encrypt(string str, string key);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string key = str_to_lower(argv[1]);
    int key_len = strlen(key);

    if (key_len != 26 || !is_alph_str(key) || !has_every_letter(key))
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    string input = get_string("plaintext: ");
    string encrypted = encrypt(input, key);
    printf("ciphertext: %s\n", encrypted);
}

string str_to_lower(string str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        str[i] = tolower(str[i]);
    }
    return str;
}

bool is_alph_str(string str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        char letter = str[i];
        if (letter > 122 || letter < 97)
        {
            return false;
        }
    }
    return true;
}

bool has_every_letter(string str)
{
    int sum = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        for (int i1 = 0; i1 < i; i1++)
        {
            if (str[i] == str[i1])
            {
                return false;
            }
        }
    }
    return true;
}

string encrypt(string str, string key)
{
    for (int i = 0; i < strlen(str); i++)
    {
        char letter = str[i];
        if (isalpha(letter))
        {
            int letter_index = toupper(letter) - 65;
            char encrypted_letter = key[letter_index];
            str[i] = isupper(letter) ? toupper(encrypted_letter) : tolower(encrypted_letter);
        }
    }
    return str;
}