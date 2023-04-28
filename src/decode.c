/** @file decode.c
 * 
 * @brief Decode hidden data from BMP image. 
 * @author Daniel Jaramillo
 */

#include <stdint.h>
#include <stdio.h>

#include "bitmap.h"
#include "decoder.h"

#define DEFAULT_INPUT_FILENAME "nothing_to_see_here.bmp"
#define DEFAULT_OUTPUT_FILENAME "output.txt"

int main(int argc, char *argv[])
{
    FILE *bitmapFilePtr, *outputFilePtr;
    bitmap_t bitmap;
    char *message = NULL;
    uint32_t error;

    // open bitmap file
    bitmapFilePtr = fopen(DEFAULT_INPUT_FILENAME, "r");
    if(bitmapFilePtr == NULL) {
        printf("Error: Unable to open bitmap file.\n");
        return -1;
    }

    // read file
    error = readBitmapFile(bitmapFilePtr, &bitmap);
    if(error != 1) {
        printf("Error: Unable to read/parse bitmap file.\n");
        fclose(bitmapFilePtr);
        return -1;
    }

    // print headers
    printBitmapHeaders(&bitmap);

    // check if decodeable
    error = isDecodeable(&bitmap);
    if(error != 1) {
        printf("Error: File not decodeable.\n");
        fclose(bitmapFilePtr);
        free(bitmap.pixel_array);
        return -1;
    }

    // decode message
    message = decodeMessage(&bitmap);
    if(message == NULL) {
        printf("Error: Unable to decode message.");
        fclose(bitmapFilePtr);
        free(bitmap.pixel_array);
        return -1;
    }

    // open output file
    outputFilePtr = fopen(DEFAULT_OUTPUT_FILENAME, "w");
    if(outputFilePtr == NULL) {
        printf("Error: Unable to open output file.\n");
        fclose(bitmapFilePtr);
        free(bitmap.pixel_array);
        return -1;
    }
    // write message to file
    fputs(message, outputFilePtr);
    // fwrite(message, 145801, 1, outputFilePtr);   // used to ignore char string terminator. Not needed for assignment.
    // printf("Message: %s\n", message);

    // cleanup
    free(bitmap.pixel_array);
    free(message);
    fclose(bitmapFilePtr);
    fclose(outputFilePtr);

    return 0;
}
/*** end of file ***/
