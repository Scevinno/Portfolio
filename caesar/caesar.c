#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

string converter(string plaintext, int key);

int main(int argc, string argv[])
{
    // Make sure program was run with just one command-line argument
    if (argc == 1)
    {
        printf("\033[31mUsage: ./caesar key\033[0m\n");
        return 1;
    }
    else if  (argv[2] != NULL)
    {
        printf("\033[31mUsage: ./caesar key\033[0m\n");
        return 1;
    }

    for (int i = 0; i < strlen(argv[1]); i++)
        {
            if (!isdigit(argv[1][i]))
            {
                printf("\033[31mUsage: ./caesar key\033[0m\n");
                return 1;
            }
        }


    int k = atoi(argv[1]) % 26;
    string plaintext = get_string("plaintext: ");
    string ciphertext = converter(plaintext,k);
    printf("ciphertext: %s\n",ciphertext);

    // Make sure every character in argv[1] is a digit

    // Convert argv[1] from a `string` to an `int`

    // Prompt user for plaintext

    // For each character in the plaintext:

        // Rotate the character if it's a letter
}


string converter(string text, int key)
{
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (isupper(text[i]))
        {
            text[i] = (text[i] - 'A' + key) % 26 + 'A';
        }
        else if (islower(text[i]))
        {
            text[i] = (text[i] - 'a' + key) % 26 + 'a';
        }
    }
    return text;
}
