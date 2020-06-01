//29 May, 2020.
//Lawrence Wong. CS50x. Pset4 - Filter(More).

#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Get the sum of R, G, and B values, and then average it
            float rgbSum = (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue);
            int rgbAvg = (int)round(rgbSum / 3);

            image[i][j].rgbtRed = rgbAvg;
            image[i][j].rgbtGreen = rgbAvg;
            image[i][j].rgbtBlue = rgbAvg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int halfWidth;
    //Find the mid point of the image
    if (width % 2 == 0)
    {
        halfWidth = width / 2;
    }
    else
    {
        halfWidth = (width + 1) / 2;
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < halfWidth; j++)
        {
            //Get the right-opposite pixel of the current pixel
            int oppPixel = width - j - 1;

            //Swap the left half and the right half of the image
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][oppPixel];
            image[i][oppPixel] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //Make an empty RGBTRIPLE array to store the averaged values
    RGBTRIPLE copyImage[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Initiatize the height and width of the box
            float sumRed = 0, sumBlue = 0, sumGreen = 0;
            int boxCount = 0;
            int minHeight = i - 1;
            int maxHeight = i + 2;
            int minWidth = j - 1;
            int maxWidth = j + 2;

            //Correct the height and width of the box if it is out of bound
            if (minHeight < 0)
            {
                minHeight = i;
            }
            if (maxHeight > height)
            {
                maxHeight--;
            }
            if (minWidth < 0)
            {
                minWidth = j;
            }
            if (maxWidth > width)
            {
                maxWidth--;
            }

            //Get the sum of R, G, and B values respectively and iterate the total boxes
            for (int boxH = minHeight; boxH < maxHeight; boxH++)
            {
                for (int boxW = minWidth; boxW < maxWidth; boxW++)
                {
                    sumRed += image[boxH][boxW].rgbtRed;
                    sumGreen += image[boxH][boxW].rgbtGreen;
                    sumBlue += image[boxH][boxW].rgbtBlue;
                    boxCount++;
                }
            }
            //Apply the rounded int to the copyImage
            copyImage[i][j].rgbtRed = (int)round(sumRed / boxCount);
            copyImage[i][j].rgbtGreen = (int)round(sumGreen / boxCount);
            copyImage[i][j].rgbtBlue = (int)round(sumBlue / boxCount);
        }
    }
    //Apply the copyImage to the actaul image to be output
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = copyImage[i][j].rgbtRed;
            image[i][j].rgbtGreen = copyImage[i][j].rgbtGreen;
            image[i][j].rgbtBlue = copyImage[i][j].rgbtBlue;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //Initiatize an image copy with a dark edge of 1 px
    int copyHeight = height + 2;
    int copyWidth = width + 2;
    
    RGBTRIPLE copyImage[copyHeight][copyWidth];
    
    for (int i = 0; i < copyHeight; i++)
    {
        for (int j = 0; j < copyWidth; j++)
        {
            //Make edge pixels black, copy the original image in the middle
            if (i == 0 || i == (copyHeight - 1) || j == 0 || j == (copyWidth - 1))
            {
                copyImage[i][j].rgbtRed = 0;
                copyImage[i][j].rgbtGreen = 0;
                copyImage[i][j].rgbtBlue = 0;
            }
            else
            {
                copyImage[i][j] = image[i - 1][j - 1];
            }
        }
    }

    //Initiatize both the Gx and Gy matrices
    int gxMatrix[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    int gyMatrix[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };

    for (int i = 1; i < (height + 1); i++)
    {
        for (int j = 1; j < (width + 1); j++)
        {
            int gxRed = 0, gxGreen = 0, gxBlue = 0;
            int gyRed = 0, gyGreen = 0, gyBlue = 0;

            for (int a = -1; a < 2; a++)
            {
                for (int b = -1; b < 2; b++)
                {
                    int ia = i + a;
                    int jb = j + b;
                    
                    gxRed += (copyImage[ia][jb].rgbtRed * gxMatrix[a + 1][b + 1]);
                    gxGreen += (copyImage[ia][jb].rgbtGreen * gxMatrix[a + 1][b + 1]);
                    gxBlue += (copyImage[ia][jb].rgbtBlue * gxMatrix[a + 1][b + 1]);
                    gyRed += (copyImage[ia][jb].rgbtRed * gyMatrix[a + 1][b + 1]);
                    gyGreen += (copyImage[ia][jb].rgbtGreen * gyMatrix[a + 1][b + 1]);
                    gyBlue += (copyImage[ia][jb].rgbtBlue * gyMatrix[a + 1][b + 1]);
                }
            }
            
            float finalRed = round(sqrt(pow(gxRed, 2) + pow(gyRed, 2)));
            float finalGreen = round(sqrt(pow(gxGreen, 2) + pow(gyGreen, 2)));
            float finalBlue = round(sqrt(pow(gxBlue, 2) + pow(gyBlue, 2)));
            
            if (finalRed > 255)
            {
                finalRed = 255;
            }
            if (finalGreen > 255)
            {
                finalGreen = 255;
            }
            if (finalBlue > 255)
            {
                finalBlue = 255;
            }
            
            image[i - 1][j - 1].rgbtRed = (int)finalRed;
            image[i - 1][j - 1].rgbtGreen = (int)finalGreen;
            image[i - 1][j - 1].rgbtBlue = (int)finalBlue;
        }
    }
    return;
}
