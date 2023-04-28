/** @file decoder.c
 *
 * @brief Decodes message hidden in bitmap file.
 * @author Daniel Jaramillo
 */
#include "decoder.h"

/**
 * Returns 1 if the bitmap is of a currently implemented decodable type.
 *
 * File can have any bitmap file signature but
 * must be of type BITMAPCOREHEADER or BITMAPINFOHEADER, have 1 color plane, 24 bits per pixel, no compression, and no
 * color palette.
 *
 * @param bitmap bitmap in memory to be checked
 * @return 1 if decodeable, 0 otherwise.
 */
uint16_t isDecodeable(bitmap_t *bitmap){
    // Check signature type
    if(bitmap->bmpFileHeader.signature != BM)
        return 0;

    switch(bitmap->dibHeader.type) {
        case BITMAPCOREHEADER:
            // must have 1 color plane and 24 bits per pixel
            if(bitmap->dibHeader.header.bitMapCoreHeader.color_planes != 1 ||
            bitmap->dibHeader.header.bitMapCoreHeader.bits_per_pixel != 24)
                return 0;
            break;

        case BITMAPINFOHEADER:
            // must have 1 color plane, 24 bits per pixel, no compression, and no color palette
            if(bitmap->dibHeader.header.bitMapInfoHeader.color_planes != 1 ||
                bitmap->dibHeader.header.bitMapInfoHeader.bits_per_pixel != 24 ||
                bitmap->dibHeader.header.bitMapInfoHeader.compression_method != BI_RGB ||
                bitmap->dibHeader.header.bitMapInfoHeader.color_palette !=0)
                return 0;
            break;

        case OS22XBITMAPHEADER:
        case OS22XBITMAPHEADER_S:
        case BITMAPV2INFOHEADER:
        case BITMAPV3INFOHEADER:
        case BITMAPV4HEADER:
        case BITMAPV5HEADER:
        default:
            // dib header types not yet supported
            return 0;
    }
    return 1;
}

/**
 * Decodes the secret message embedded in bitmap data.
 *
 * Assumes file is of a decodeable type.
 *
 * @param bitmap bitmap in memory to be decoded.
 * @return pointer to char string with secret message.
 */
char *decodeMessage(bitmap_t *bitmap) {
    uint32_t charCount;
    uint32_t byteCount;
    uint32_t width;
    uint32_t height;

    // allocate memory for string and add end of string marker
    // cases are redundant to allow for more formats
    switch(bitmap->dibHeader.type) {
        case BITMAPCOREHEADER:
            width = bitmap->dibHeader.header.bitMapCoreHeader.bitmap_width;
            height = bitmap->dibHeader.header.bitMapCoreHeader.bitmap_height;
            byteCount = 3 * width * height;
            charCount = byteCount / 8;
            break;

        case BITMAPINFOHEADER:
            width = bitmap->dibHeader.header.bitMapInfoHeader.bitmap_width;
            height = bitmap->dibHeader.header.bitMapInfoHeader.bitmap_height;
            byteCount = 3 * width * height;
            charCount = byteCount / 8;
            break;

        case OS22XBITMAPHEADER:
        case OS22XBITMAPHEADER_S:
        case BITMAPV2INFOHEADER:
        case BITMAPV3INFOHEADER:
        case BITMAPV4HEADER:
        case BITMAPV5HEADER:
        default:
            byteCount = 0;
            charCount = 0;
            return NULL;
    }
    char *str = malloc(charCount + 1);
    str[charCount] = '\0';

    //TODO: this version does not work with row padding
    uint8_t tempChar = 0;
    uint32_t cCounter = 0;
    uint8_t bit = 0;
    for(uint32_t bCounter = 0; bCounter < byteCount && cCounter < charCount; bCounter++) {
        tempChar = tempChar | ((bitmap->pixel_array[bCounter] & 1) << bit);
        bit++;
        if(bit > 7) {
            str[cCounter] = tempChar;
            cCounter++;
            bit = 0;
            tempChar = 0;
        }
    }
    return str;
}
