// 15 May 2020.
// Lawrence Wong. CS50x. Readability.

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>

int main(void)
{
    float letter = 0, word = 0, sentence = 0;
    float eqgrade = 0.0, eqletter = 0.0, eqsentence = 0.0;
    string text = get_string("Text: ");
    //Determine where each string in text belongs
    for (int i = 0; i < strlen(text); i++)
    {
        if (((text[i] >= 'a') && (text[i] <= 'z')) || ((text[i] >= 'A') && (text[i] <= 'Z')))
        {
            letter++;
        }
        else if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentence++;
        }
        else if (text[i] == ' ')
        {
            word++;
        }
    }
    //Apply the equation
    eqletter = letter * (100 / (word + 1));
    eqsentence = sentence * (100 / (word + 1));
    eqgrade = (0.0588 * eqletter) - (0.296 * eqsentence) - 15.8;
    //Round and convert floating point grade to integer
    int grade = round(eqgrade);
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade > 1 && grade < 16)
    {
        printf("Grade %i\n", grade);
    }
    else
    {
        printf("Grade 16+\n");
    }
}
