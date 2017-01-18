/**
 * Implements a dictionary's functionality.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "dictionary.h"

// Define node type
typedef struct node
{
    bool is_word;
    struct node *letter[27];
}
node;

// Initialize global node pointer root which will always point at root node of trie and set to NULL
node *root = NULL;

// Initialize global dictionary words counter for size function
int dictionaryWords = 0;
    
/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // Declare node pointer (traversal) to move through trie and point it at root node
    node *traversal = root;
    int wordLength = strlen(word);
    
    // Loop through individual letters of input word
    for (int i = 0; i < wordLength; i++)
    {
        // Declare variable to store array index of current character
        int index;
        if (isupper(word[i]))
            index = word[i] - 65;
        else if (islower(word[i]))
            index = word[i] - 97;
        else
            index = 26;
        
        /*
         * For each letter in input word, check if corresponding element in letter array of current node is NULL
          * If it is NULL, the word must be mispelled and we can return false.
          */
        if (traversal->letter[index] == NULL)
            return false;
        
        //Else, index of letter is not NULL so we move traversal to the node pointed to by the pointer of the current node
        else
            traversal = traversal->letter[index];
    }
    
    // Check bool is_word to prevent invalid substrings from being returned as correctly spelled words.
    if (traversal->is_word == true)
        return true;
        
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // Open dictionary file for reading and check for success
    FILE *fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        fclose(fp);
        return false;
    }
    
    // Malloc root node, maintain pointer, and check that memory allocation was successful
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        unload();
        return false;
    }
    
    // Create word array to store current dictionary word (up to 45 characters) plus the terminating null byte
    char word[LENGTH+1] = {};
    int wordLength = 0;
    
    // Loop through dictionary, adding words to trie until reaching end of file
    while (fscanf(fp, "%s", word) != EOF)
    {
        // Increment dictionary words counter for size function
        dictionaryWords++;
        wordLength = strlen(word);
    
        // Initialize traversal pointer to keep track of movement through trie and point at root node.
        node *traversal = root;

        /* 
         * Loop through alphanumeric characters and apostraphe's of current word.
         * Check if current character exists in the letter array of the current node.
         * If it does not, malloc a new node and set traversal pointer to this new node.
         * Otherwise, move the traversal pointer to that node pointed to by the letter array pointer and advance to next character.
         */
        
        for (int j = 0; j < wordLength; j++)
        {
            // Declare integer variable to store array index of current character
            int index;
            if (isupper(word[j]))
                index = word[j] - 65;
            else if (islower(word[j]))
                index = word[j] - 97;
            else
                index = 26;
                
            /* 
             * Check if current node letter index is NULL.
             * If it is, malloc new node and point the appropriate node letter[index] pointer at new node.
             */
            if (traversal->letter[index] == NULL)
            {
                traversal->letter[index] = malloc(sizeof(node));
                traversal = traversal->letter[index];
            }
            // Else, move traversal pointer to the node pointed to by the letter array of current node at correct index.
            else
                traversal = traversal->letter[index];
        }
        // Set the bool is_word on final character node of word to true
        traversal->is_word = true;
    }
    // Close dictionary file and return true
    fclose(fp);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // Global dictionaryWords is incremented during load for use here
    return dictionaryWords;
}

void free_recursive(node* traversal)
{
    
    // recursive case (go to end of trie)
    for (int i = 0; i < 27; i++)
    {
        if (traversal->letter[i] != NULL)
        {
            free_recursive(traversal->letter[i]);
        }
    }

    // base case where we free current node pointed to by traversal and set it to NULL
    free(traversal);
    traversal = NULL;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    free_recursive(root);
    return true;
}

