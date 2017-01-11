#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    //prompt for input of name
    string name = get_string();
    
    //print capitalized first character if not a space
    if (name[0] != ' ')
    {
        printf("%c", toupper(name[0]));
    }
    
    //loop through each letter of name
    int n;
    for (int i = 0; n = strlen(name), i < n; i++)
    {
        /* 
        Use a space as a reference to find the first letter of each word
        Check that the index following the space is not itself a space and is not the end of the string
        */
        if (name[i] == ' ' && name[i + 1] != ' ' && name[i + 1] != '\0')
        {
            printf("%c", toupper(name[i + 1]));
        }
    }
    printf("\n");
}