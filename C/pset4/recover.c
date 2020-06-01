//31 May, 2020.
//Lawrence Wong. CS50x. Pset4 - Recover.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    //Ensure correct usage
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    
    FILE *image = fopen(argv[1], "r");
    if (!image)
    {
        printf("%s does not exist!\n", argv[1]);
        return 1;
    }
    
    int blockSize = 512, fileCount = 0, jpegFound = 0;
    unsigned char bytes[blockSize];
    char fileName[3];
    FILE *jpeg;

    while (fread(bytes, blockSize, 1, image) == 1)
    {
        //Check first 3 bytes
        if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff)
        {
            //Iterate thought the possible cases to check if the fourth btye is correct
            for (int j = 0; j < 16; j++)
            {
                //Create new file if the starting signature of JPEG is detected
                if (bytes[3] == 0xe0 + j)
                {
                    sprintf(fileName, "%03d.jpg", fileCount);
                    jpeg = fopen(fileName, "w");
                    fileCount++;
                    jpegFound++;
                }
            }
        }
    
        //Reset jpegFound if the next JPEG signature is found
        if (jpegFound > 1)
        {
            jpegFound = 1;
        }
        
        //Write data in blockSize into new file, 512 bytes at a time
        if (jpegFound == 1)
        {
            fwrite(bytes, blockSize, 1, jpeg);
        }
    }
    fclose(jpeg);
    fclose(image);
}
