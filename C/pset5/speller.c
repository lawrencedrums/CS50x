//Started 5 June, 2020. Submitted 7 June, 2020.
//Lawrence Wong. CS50x. Pset5 - Speller.

// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

//Declare wordsTotal
int totalWords = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *checking = table[hash(word)];
    
    while (checking != NULL)
    {
        if (strcasecmp(checking->word, word) == 0)
        {
            return true;
        }
        checking = checking->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //Return int 0-25 base on the first letter
    int firstLetter = (int)tolower(word[0]) - 97;
    return firstLetter;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    char *wordFromDict = malloc(LENGTH);
    
    if (wordFromDict == NULL)
    {
        return false;
    }
    
    while (fscanf(file, "%s", wordFromDict) != EOF)
    {
        //Initialize a new node
        node *n = malloc(sizeof(node));
        
        if (n == NULL)
        {
            return false;
        }
        
        //Copy word to the current node
        strcpy(n->word, wordFromDict);
        n->next = table[hash(wordFromDict)];
        
        //Point array to the updated linked list
        table[hash(wordFromDict)] = n;
        
        totalWords++;
    }
    
    fclose(file);
    free(wordFromDict);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return totalWords;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *freeing = table[i];
        
        while (freeing != NULL)
        {
            node *tmp = freeing;
            freeing = freeing->next;
            free(tmp);
        }
        free(freeing);
    }
    return true;
}
