/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// trie structure
typedef struct node_t
{
    bool is_word;
    struct node_t* children[27];
} node_t;

// function prototypes
void delete(node_t* node);
node_t* new_node();

// global variable
node_t* root;
int dictionary_size;

/**
 * Create a new node of trie
 */
node_t* new_node()
{
    node_t* node = malloc(sizeof(node_t));
    if(node == NULL)
    {
        return NULL;
    }
    node->is_word = false;
    for(int i = 0; i < 27; i++)
    {
        node->children[i] = NULL;
    }
    return node;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    node_t* node = root;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        int j = (word[i] == '\'') ? 26: tolower(word[i]) - 'a';
        if (node->children[j]==NULL)
        {
            return false;
        } 
        else
        {
            node = node->children[j];
        }
    }
    return node->is_word;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // open file with dictionary
    FILE* f = fopen(dictionary, "r");
    if (f == NULL)
    {
        return false;
    }
    
    
    // create root of trie
    root = new_node();
    if (root == NULL)
    {
        return false;
    }
    
    // prepare to load words
    node_t* node = root;
    dictionary_size = 0;
    
    //load words to trie
    for (int c = fgetc(f); c != EOF; c = fgetc(f))
    {
        if(c != '\n')
        {
            // add symbol to trie
            int i = (c == '\'') ? 26 : c - 'a'; 
            if (node->children[i] == NULL)
            {
                node->children[i] = new_node();
                if (node->children[i] == NULL)
                {
                    return false;
                }
                node = node->children[i];
            }
            else
            {
                node = node->children[i];
            }
        }
        else
        {
            // end of word
            node->is_word = true;
            node = root;
            ++dictionary_size;
        }
    }
    
    // check whether there was an error
    if (ferror(f))
    {
        return false;
    }
    
    fclose(f);

    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return dictionary_size;
}

void delete(node_t* node)
{
    for(int i = 0; i < 27; i++)
    {
        if(node->children[i] != NULL)
        {
            delete(node->children[i]);
            free(node->children[i]);
        }
    }
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    delete(root);
    free(root);
    return true;
}


