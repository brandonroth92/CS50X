#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    //validate number of command-line arguments
    if (argc != 2)
    {
        printf("Error: Command-line arguments must equal two\n");
        return 1;
    }
    
    //keyphrase must contain only alphabetical characters
    int n;
    for (int i = 0; n = strlen(argv[1]), i < n; i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("Error: Keyphrase must include alphabetical characters only\n");
            return 1;
        }
    }
    
    /*
     * Prompt user for plaintext
     * Do not proceed until user has entered some plaintext
     */
    string plainText;
    do {
        printf("plaintext: ");
        plainText = get_string();    
    }
    while (strlen(plainText) == 0);
    
    //initialize length variables so they are not run each time for loop iterates
    int plainTextLength = strlen(plainText);
    int keyLength = strlen(argv[1]);
    
    //initialize a variable to hold returned ciphertext
    char* cipherText = plainText;
    
    /*
     * Alphabetical characters shifted by key letters
     * Key letters are wrapped around for length of plaintext using modulo operator
     */
    for (int i = 0, j = 0; i < plainTextLength; i++)
    {
        
        //output shifted uppercase letter
        if (isupper(plainText[i]))
        {
            char keyReference = (isupper(argv[1][j % keyLength])) ? 'A' : 'a';
            cipherText[i] = ((plainText[i] - 65 + (argv[1][(j % keyLength)] - keyReference)) % 26) + 65;
            //move forward 1 letter in keyphrase
            j++;
        }
        
        //ouput shifted lowercase letter
        if (islower(plainText[i]))
        {
            char keyReference = (isupper(argv[1][(j % keyLength)])) ? 'A' : 'a';
            cipherText[i] = ((plainText[i] - 97 + (argv[1][(j % keyLength)] - keyReference)) % 26) + 97;
            //move forward 1 letter in keyphrase
            j++;
        }
        
        //output non alphabetical character unshifted and do not move key position (j) forward
        cipherText[i] = plainText[i];
    }
    
    printf("ciphertext: %s\n", cipherText);
    
}