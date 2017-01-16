#include <stdio.h>
#include <stdint.h>

// block-size will always be 512 blocks
const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    
    // open memory card file and check for proper open, otherwise return 2
    FILE *infile = fopen(argv[1], "r");
    if (infile == NULL)
    {
        fprintf(stderr, "Could not open file\n");
        return 2;
    }
    
    //create buffer to store block_size from card
    uint8_t buffer[512];
    
    // initialize jpeg_counter for use in naming jpg files
    int jpeg_counter = 0;
  
    /* 
     * Initialize new file pointer with value of NULL.
     * This value is used to determine if jpg is the first jpg on card or a subsequent jpg
     */
    FILE *img = NULL;
    
    // Repeat until the end of the card which occurs when fread does not return 512 bytes
    while(fread(buffer, BLOCK_SIZE, 1, infile))
    {
        // check if first 4 digits indicate the start of a jpg file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff
            && (buffer[3] & 0xf0) == 0xe0)
        {
            // if so, check if this jpg file is the first jpg of the card or a subsequent jpg
            if (img != NULL)
            {
                // if this is not the first jpg file, close the previous jpg file
                fclose(img);
            }
            
            // create new jpg file and add 1 to jpeg_counter to ensure proper naming of subsequent files
            char filename[8];
            sprintf(filename, "%03i.jpg", jpeg_counter);
            img = fopen(filename, "w");
            jpeg_counter++;
        }
        
        /* 
         * If first 4 digits do not indicate start of jpg file, check if jpeg file is open.
         * If so, write buffer to the jpg image file.
         * If not, skip these blocks as they must be at the beginning of the memory card
         * before the initial jpeg image
         */
      
        if (img != NULL)
        {
            fwrite(buffer, BLOCK_SIZE, 1, img);
        }
    }
    
    // close last jpg image file, if open
    if (img != NULL)
    {
        fclose(img);
    }
    
    // close memory card file
    fclose(infile);
  
    return 0;
    
}