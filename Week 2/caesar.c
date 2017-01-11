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
    
    //calculate key number by converting from string and using modulo 26 to get a number between 0-26
    int keyNumber = atoi(argv[1]) % 26;
    
    /*
     * Prompt user for plaintext
     * Do not proceed until user has entered some plaintext
     */
    string plainText;
    do {
        printf("plaintext: ");
        plainText = get_string();    
    }
    while (plainText == NULL);
    
    //create variable to hold ciphertext
    char* cipherText = plainText;
    
    //iterate through plaintext characters
    for (int i = 0, n = strlen(plainText); i < n; i++)
    {
        
        //if uppercase letter, rotate by key number   
        if (isupper(plainText[i]))
        {
            cipherText[i] = ((plainText[i] - 65 + keyNumber) % 26) + 65;
        } 
        //if lowercase letter, rotate by key number
        else if (islower(plainText[i]))
        {
            cipherText[i] = ((plainText[i] - 97 + keyNumber) % 26) + 97;
        }
        //if not a letter, add to ciphertext without shifting
        else
        {
            cipherText[i] = plainText[i];
        }
    }
    printf("ciphertext: %s\n", cipherText);
}