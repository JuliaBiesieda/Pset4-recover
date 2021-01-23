#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t BYTE;
#define chunk 512
BYTE buffer[chunk];
char filename[9];
bool first = false;
int countJPEG = 0;

int main(int argc, char *argv[])
{
    // check if user provided only one argument
    if (argc != 2)
    {
        printf("Usage: recover [name of the image ...]\n");
        return 1;
    }
    // try to open the file
    FILE *card = fopen(argv[1], "r");

    // check if there's an error in opening the file
    if (card == NULL)
    {
        printf("Error! Try again!\n");
        return 1;
    }

    sprintf(filename, "%03d.jpg", countJPEG);
    FILE *img = fopen(filename, "w");



    //read trough the card in 512 bytes blocks
    while (0 < fread(buffer, sizeof(BYTE), chunk, card))
    {
        // find the first jpeg and start writing into img file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // if the first is false, then it's the 1st image
            if (!first)
            {
                fwrite(buffer, sizeof(BYTE), chunk, img);
                first = true;
                continue;
            }
            //  if new header is detected, close the previous and create a new one
            if (first)
            {
                fclose(img);
                countJPEG++;
                sprintf(filename, "%03i.jpg", countJPEG);
                img = fopen(filename, "w");
                fwrite(buffer, sizeof(BYTE), chunk, img);
            }
        }
        // if no header is detected keep writing
        else
        {
            if (first)
            {
                fwrite(buffer, sizeof(BYTE), chunk, img);
            }
        }
    }

    fclose(card);
    printf("Success! %d JPEGs found.\n", countJPEG + 1);
    return 0;
}

