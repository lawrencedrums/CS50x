//15 May, 2020.
//Lawrence Wong. CS50x. Credit.

#include <stdio.h>
#include <cs50.h>

long get_cardnumber(string prompt);

int main(void)
{
    int luhnSum = 0, luhnProduct = 0, p = 0;
    long cardnumber = get_cardnumber("Number: ");
    //Check if the length of cardnumber is valid
    if (cardnumber < 10000000000000000 && cardnumber >= 1000000000000)
    {
        //Check sum
        for (long checknumber = cardnumber; checknumber > 0;)
        {
            p = (((checknumber / 10) % 10) * 2);
            if (p > 9)
            //Split digits if product is greater than 9
            {
                luhnProduct = p / 10 + p % 10;
            }
            else
            {
                luhnProduct = p;
            }
            //Add the each product and the unused digit together
            luhnSum = luhnSum + luhnProduct + (checknumber % 10);
            checknumber = checknumber / 100;
        }
        //Check modulo 10
        if (luhnSum % 10 == 0)
        {
            //Check for VISA starting number and lengths
            if (cardnumber / 1000000000000 == 4 || cardnumber / 1000000000000000 == 4)
            {
                printf("VISA\n");
            }
            //Check for AMEX starting numbers
            else if (cardnumber / 10000000000000 == 34 || cardnumber / 10000000000000 == 37)
            {
                printf("AMEX\n");
            }
            //Check for MASTERCARD starting numbers
            else if (cardnumber / 100000000000000 > 50 && cardnumber / 100000000000000 < 56)
            {
                printf("MASTERCARD\n");
            }
            //Invalid when starting numbers and lengths matches none
            else
            {
                printf("INVALID\n");
            }
        }
        //Invalid when cardnumber failed to pass Luhn's Algorithm
        else
        {
            printf("INVALID\n");
        }
    }
    //Invalid when cardnumber is too long or too short
    else
    {
        printf("INVALID\n");
    }
}
//Prompt user for credit card number
long get_cardnumber(string prompt)
{
    long cardnumber;
    do
    {
        cardnumber = get_long("%s", prompt);
    }
    while (cardnumber < 1);
    return cardnumber;
}
