//17 May, 2020.
//Lawrence Wong. CS50x. Pset2 - Substitution

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <cs50.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    //Return 1 if key is not 26 letters long
    else if (strlen(argv[1]) != 26)
    {
        printf("Key can only be 26 letters long.\n");
        return 1;
    }
    else
    {
        int encrypt[122], checkdup[122] = {0};
        for (int i = 0, ch = 0; i < 26; i++)
        {
            //Check if each digit of key is a letter
            if ((argv[1][i] >= 'a' && argv[1][i] <= 'z') || (argv[1][i] >= 'A' && argv[1][i] <= 'Z'))
            {
                //If the i-th digit of key is a letter, store it in encrypt and checkdup
                encrypt[i + 97] = tolower(argv[1][i]);
                ch = tolower(argv[1][i]);
                //Add 1 to checkdup when a letter in the key first appear
                //Return 1 if checkdup equals to 1: The letter is already used
                if (checkdup[ch] == 1)
                {
                    printf("%i. ", checkdup[ch]);
                    printf("Each letter can only occur once.\n");
                    return 1;
                }
                else
                {
                    checkdup[ch] = 1;
                }
            }
            //Return 1 if the i-th digit of key is not a letter
            else
            {
                printf("Key must only contain letters.\n");
                return 1;
            }
        }
        //Prompt user for plaintext, set the length of ciphertext to the length of plaintext
        string plaintext = get_string("plaintext:  ");
        int plainLength = strlen(plaintext);
        char ciphertext[plainLength];
        printf("ciphertext: ");
        //Replace the i-th digit of plaintext with encrypt
        for (int i = 0; i < plainLength; i++)
        {
            if (isupper(plaintext[i]))
            {
                ciphertext[i] = encrypt[(int)plaintext[i] + 32];
                printf("%c", toupper(ciphertext[i]));
            }
            else if (islower(plaintext[i]))
            {
                ciphertext[i] = encrypt[(int)plaintext[i]];
                printf("%c", ciphertext[i]);
            }
            else
            {
                printf("%c", plaintext[i]);
            }
        }
        printf("\n");
        return 0;
    }
}
