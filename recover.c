/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <stdint.h>
 
typedef struct
{
    uint32_t type;
    uint8_t other[508];
}__attribute__((__packed__))
BUFFER;
 
int main(int argc, char* argv[])
{
    // open file
    FILE* card = fopen("card.raw", "r");
    
    if (card == NULL)
    {
        printf("Could not open card.\n");
        return 1;
    }
    
    // remember the number of found JPEG
    int count = 0;
    
    // create buffer where we save the cluster
    BUFFER buffer;
    
    // declare the image
    FILE* img = NULL;
    
    while(fread(&buffer, 512, 1, card) == 1)
    {
        if(buffer.type == 0xe0ffd8ff || buffer.type == 0xe1ffd8ff)
        {
            if(count != 0)
            {
                fclose(img);
            }
            char title[8];
            sprintf(title,"%03d.jpg", count);
            img = fopen(title, "w");
            if(img == NULL)
            {
                printf("Could not create %s.\n", title);
                return 2;
            }
            count++;
        }
        if(img != NULL)
        {
            fwrite(&buffer, 512, 1, img);
        }
    }
    fclose(img);
    fclose(card);
}
