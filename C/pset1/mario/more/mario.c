//15 May, 2020
//Lawrence Wong, CS50x, Mario

#include <stdio.h>
#include <cs50.h>

int get_height(string prompt);

int main(void)
{
    int height = get_height("Height of the pyramid(1-8): ");
    for (int i = 1, leftHash = 0, rightHash = 0; i <= height; i++, leftHash = 0, rightHash = 0)
    {
        //Print the left side space
        for (int leftSpace = 1; leftSpace <= height - i; leftSpace++)
        {
            printf(" ");
        }
        //Print the left side hashes of the pyramid
        while (leftHash != i)
        {
            printf("#");
            leftHash++;
        }
        printf("  ");
        //{rint the right side hashes of the pyramid
        while (rightHash != i)
        {
            printf("#");
            rightHash++;
        }
        printf("\n");
    }
}

// Prompt user for height (1-8)
int get_height(string prompt) 
{
    int height;   
    do    
    {
        height = get_int("%s", prompt);
    }
    while (height < 1 || height > 8);
    return height;
}
