// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"

typedef struct node {
    char word[LENGTH +1];
    struct node* next;
}
node;


// HashTable buckets
static const unsigned int buckets = 100;

int wordCount = 0;

// Struct type node and points to the each linked list head node
node* hashtable[buckets];

unsigned int hash(const char *word)
{
    unsigned int hash = 1;

    for (int i = 0; word[i] != '\0'; i++)
        hash ^= word[i];


    hash = hash % (buckets - 1);

    return hash;
}

// Returns true if word is in dictionary else false

bool check(const char *word)
{

    int length = strlen(word);

    char lc_word[length + 1];

    for(int i = 0; i < length; i++)
    {
        lc_word[i] = tolower(word[i]);
    }

    lc_word[length] = '\0';

    int loc_bucket = hash(lc_word);

    node* cursor = hashtable[loc_bucket];

    // Compares words
    int compare;

    while(cursor != NULL)
    {
        compare = strcasecmp(cursor->word, word);

        if(compare == 0)
            return true;

        else
            cursor = cursor->next;

    }
    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Loading dictionary

    FILE *dcptr = fopen(dictionary, "r");

    if (dcptr == NULL)
    {
       // fprintf(stderr,"Could not load %s.\n", dictionary);
        return false;
    }

    // Space in memory to store string values
    char word[LENGTH + 1];

    for (int i = 0; i < buckets; i++)
        hashtable[i] = NULL;


    // Parse through each dict word
    while(fscanf(dcptr, "%s", word) != EOF)
    {
        // Malloc a temporary node
        node *new_node = malloc(sizeof(node));

        // Check if new node is not null
        if (new_node == NULL)
        {
            unload();
            return false;
        }

        for(int i = 0; word[i] != '\0'; i++)
           word[i] = tolower(word[i]);


        // Move this word to newnode
        strcpy(new_node -> word, word);

        // Finds what index of the array word should go
        int index = hash(new_node -> word);

        // Handles collision
        if(hashtable[index] == NULL)
        {
            hashtable[index] = new_node;
            new_node->next = NULL;
        }

        else
        {
            new_node-> next = hashtable[index];
            hashtable[index] = new_node;
        }

        wordCount++;
    }
    fclose(dcptr);
        return true;

}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if(wordCount != 0)
    {
        return wordCount;
    }

    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    int index = 0;

    // If NULL move to next index
    while(index < buckets)
    {
            if(hashtable[index] != NULL)
            {
            node* cursor = hashtable[index];
            node* temp = cursor;
            hashtable[index] = cursor->next;
            free(temp);
            }
            else if(hashtable[index] == NULL)
            {
                index++;
            }

        /*
        if(hashtable[index] == NULL)
        {
            index++;
        }
        // Start freeing space

        else
        {
            while(hashtable[index] != NULL)
            {
            node* cursor = hashtable[index];
            hashtable[index] = cursor->next;
            free(cursor);
            }
              index++;
        }
        */
    }

    return true;
}
