// Implements a dictionary's functionality

#include <cs50.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <math.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int hashVal = hash(word);
    node *tablePointer = table[hashVal];
    while (tablePointer != NULL)
    {
        if (strcasecmp(tablePointer->word, word) == 0)
        {
            return true;
        }
        tablePointer = tablePointer->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    int hashVal = 0;
    for (int i = 0; i < 5 && i < strlen(word); i++)
    {
        hashVal += pow(tolower(word[i]) - 'a', 2);
    }

    return hashVal % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *file = fopen(dictionary, "r");
    char line[LENGTH + 1];

    if (!file)
    {
        return false;
    }

    while (fgets(line, sizeof(line), file))
    {
        // Remove \n symbol;
        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) > 0)
        {
            // Allocate memory for new node
            node *newNode = malloc(sizeof(node));
            if (newNode == NULL)
            {
                return false;
            }

            // Apply values to new node
            strcpy(newNode->word, line);
            newNode->next = NULL;

            // Find hash value
            int hashVal = hash(line);

            // Go to the last value and append node
            if (table[hashVal] == NULL)
            {
                table[hashVal] = newNode;
            }
            else
            {
                node *tablePointer = table[hashVal];
                while (tablePointer->next != NULL)
                {
                    tablePointer = tablePointer->next;
                }
                tablePointer->next = newNode;
            }
        }
    }

    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    int size = 0;

    for (int i = 0; i < N; i++)
    {
        node *tablePointer = table[i];
        while (tablePointer != NULL)
        {
            size++;
            tablePointer = tablePointer->next;
        }
    }

    return size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        node *tablePointer = table[i];
        while (tablePointer != NULL)
        {
            node *tmp = tablePointer->next;
            free(tablePointer);
            tablePointer = tmp;
        }
    }

    return true;
}