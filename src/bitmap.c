/** @file bitmap.c
 *
 * @brief Read bitmap file from long term storage and parse to structures in memory.
 * @author Daniel Jaramillo
 */

#include "bitmap.h"

/**
 * Reads the bitmap file header and parses it to a bmpFileHeader struct.
 *
 * In addition to reading the file header, it reads the next 4 bytes to retrieve the size of the DIB header. Calls
 * parseBMPFileHeader() to parse the file.
 *
 * @param bitmapFilePtr FILE pointer for bitmap file.
 * @param bmpFileHeader bitmap file header struct.
 * @return size of BMP file header on success, 0 otherwise.
 */
uint32_t readBMPFileHeader(FILE *bitmapFilePtr, bmpFileHeader_t *bmpFileHeader) {
    uint8_t buffer[BMPFILEHEADERSIZE + 4];

    size_t count = fread(&buffer, 1, BMPFILEHEADERSIZE + 4, bitmapFilePtr);
    if(count != (BMPFILEHEADERSIZE + 4))
        return -1;

    return parseBMPFileHeader((const uint8_t *) &buffer, bmpFileHeader);
}

/**
 * Parses the block of memory containing bitmap file header into bitmap file header struct.
 *
 * The first 4 bytes of the DIB header are read by readBMPFileHeader() to determine type of header.
 *
 * @param buffer block of memory containing bitmap file header.
 * @param bmpFileHeader bitmap file header struct.
 * @return size of BMP file header on success, 0 otherwise.
 */
uint32_t readDIBHeader(FILE *bitmapFilePtr, uint32_t dibHeaderSize, dibHeader_t *dibHeader) {
    uint8_t buffer[dibHeaderSize - 4];

    size_t count = fread(&buffer, 1, dibHeaderSize - 4, bitmapFilePtr);
    if(count != (dibHeaderSize - 4))
        return -1;

    return parseDIBHeader(buffer, dibHeaderSize - 4, dibHeader);
}

/**
 * Parses the block of memory containing bitmap file header into bitmap file header struct.
 *
 * Parses 4 bytes from DIB header to get size and type of DIB header.
 * @param buffer block of memory containing bitmap file header.
 * @param bmpFileHeader bitmap file header struct.
 * @return size of BMP file header on success, 0 otherwise.
 */
uint32_t parseBMPFileHeader(const uint8_t *buffer, bmpFileHeader_t *bmpFileHeader) {
    bmpFileHeader->signature =
            ((uint16_t)buffer[0] << 8) | buffer[1];
    bmpFileHeader->file_size =
            ((uint32_t)buffer[5] << 24) | ((uint32_t)buffer[4] << 16) | ((uint16_t)buffer[3] << 8) | buffer[2];
    bmpFileHeader->rsv0 =
            ((uint16_t)buffer[7] << 8) | buffer[6];
    bmpFileHeader->rsv1 =
            ((uint16_t)buffer[9] << 8) | buffer[8];
    bmpFileHeader->img_offset =
            ((uint32_t)buffer[13] << 24) | ((uint32_t)buffer[12] << 16) | ((uint16_t)buffer[11] << 8) | buffer[10];

    // retrieve bitmap information header size
    return ((uint32_t)buffer[17]) | ((uint32_t)buffer[16]) | ((uint16_t)buffer[15]) | buffer[14];
}

/**
 * Parses DIB header, bitmap information header, of BITMAPCOREHEADER format.
 *
 * @param buffer block of memory containing bitmap header, dib header.
 * @param dibHeader struct to hold bitmap header, dib header.
 * @return 1 on success, 0 on failure.
 */
uint32_t parseBITMAPCOREHEADER(const uint8_t *buffer, dibHeader_t *dibHeader) {
    dibHeader->type = BITMAPCOREHEADER;
    dibHeader->header.bitMapCoreHeader.header_size = BITMAPCOREHEADER;
    dibHeader->header.bitMapCoreHeader.bitmap_width = ((uint16_t)buffer[1] << 8) | buffer[0];
    dibHeader->header.bitMapCoreHeader.bitmap_height = ((uint16_t)buffer[3] << 8) | buffer[2];
    dibHeader->header.bitMapCoreHeader.color_planes = ((uint16_t)buffer[5] << 8) | buffer[4];
    dibHeader->header.bitMapCoreHeader.bits_per_pixel = ((uint16_t)buffer[7] << 8) | buffer[6];

    return 1;
}

/**
 * Parses DIB header, bitmap information header, of BITMAPINFOHEADER format.
 *
 * @param buffer block of memory containing bitmap header, dib header.
 * @param dibHeader struct to hold bitmap header, dib header.
 * @return 1 on success, 0 on failure.
 */
uint32_t parseBITMAPINFOHEADER(const uint8_t *buffer, dibHeader_t *dibHeader) {
    dibHeader->type = BITMAPINFOHEADER;
    dibHeader->header.bitMapInfoHeader.header_size = BITMAPINFOHEADER;
    dibHeader->header.bitMapInfoHeader.bitmap_width =
            ((uint32_t)buffer[3] << 24) |((uint32_t)buffer[2] << 16) | ((uint16_t)buffer[1] << 8) | buffer[0];
    dibHeader->header.bitMapInfoHeader.bitmap_height =
            ((uint32_t)buffer[7] << 24) |((uint32_t)buffer[6] << 16) | ((uint16_t)buffer[5] << 8) | buffer[4];
    dibHeader->header.bitMapInfoHeader.color_planes =
            ((uint16_t)buffer[9] << 8) | buffer[8];
    dibHeader->header.bitMapInfoHeader.bits_per_pixel =
            ((uint16_t)buffer[11] << 8) | buffer[10];
    dibHeader->header.bitMapInfoHeader.compression_method =
            ((uint32_t)buffer[15] << 24) |((uint32_t)buffer[14] << 16) | ((uint16_t)buffer[13] << 8) | buffer[12];
    dibHeader->header.bitMapInfoHeader.image_size =
            ((uint32_t)buffer[19] << 24) |((uint32_t)buffer[18] << 16) | ((uint16_t)buffer[17] << 8) | buffer[16];
    dibHeader->header.bitMapInfoHeader.horizontal_res =
            ((uint32_t)buffer[23] << 24) |((uint32_t)buffer[22] << 16) | ((uint16_t)buffer[21] << 8) | buffer[20];
    dibHeader->header.bitMapInfoHeader.vertical_res =
            ((uint32_t)buffer[27] << 24) |((uint32_t)buffer[26] << 16) | ((uint16_t)buffer[25] << 8) | buffer[24];
    dibHeader->header.bitMapInfoHeader.color_palette =
            ((uint32_t)buffer[31] << 24) |((uint32_t)buffer[30] << 16) | ((uint16_t)buffer[29] << 8) | buffer[28];
    dibHeader->header.bitMapInfoHeader.important_colors =
            ((uint32_t)buffer[35] << 24) |((uint32_t)buffer[34] << 16) | ((uint16_t)buffer[33] << 8) | buffer[32];

    return 1;
}

/**
 * Parses all formats of DIB header/bitmap information header.
 *
 * Uses dibHeader size to identify specific header and calls corresponding function.
 *
 * @param buffer block of memory containing bitmap header, dib header.
 * @param dibHeaderSize size of header being parsed.
 * @param dibHeader struct to hold bitmap information header, dib header.
 * @return returns 1 on success, 0 otherwise.
 */
uint32_t parseDIBHeader(const uint8_t *buffer, uint32_t dibHeaderSize, dibHeader_t *dibHeader) {
    switch(dibHeaderSize + 4) {
        case BITMAPCOREHEADER:
            return parseBITMAPCOREHEADER(buffer, dibHeader);

        case BITMAPINFOHEADER:
            return parseBITMAPINFOHEADER(buffer, dibHeader);

        case OS22XBITMAPHEADER:
        case OS22XBITMAPHEADER_S:
        case BITMAPV2INFOHEADER:
        case BITMAPV3INFOHEADER:
        case BITMAPV4HEADER:
        case BITMAPV5HEADER:
        default:
            printf("Bitmap format with header size %u not yet supported.\n", dibHeaderSize + 4);
            return -1;
    }
}

/**
 * Reads the bitmap file and parses it into structs in memory.
 *
 * Calls readBMPFileHeader() and readDIBHeader to parse bitmap file header and dib header.
 *
 * @param bitmapFilePtr file pointer to the bitmap file being read.
 * @param bitmap struct containing bitmap in memory.
 * @return 1 on success, 0 on error.
 */
uint32_t readBitmapFile(FILE *bitmapFilePtr, bitmap_t *bitmap) {
    // read file header
    uint32_t dibHeaderType = readBMPFileHeader(bitmapFilePtr, &(bitmap->bmpFileHeader));
    if(dibHeaderType == 0) {
        return 0;
    }

    // read dib header
    uint32_t error = readDIBHeader(bitmapFilePtr, dibHeaderType, &(bitmap->dibHeader));
    if(error == 0)
        return 0;

    // calculate space for pixel_array
    uint32_t pixel_array_size;
    switch(bitmap->dibHeader.type) {
        case BITMAPCOREHEADER:
            pixel_array_size  = ((bitmap->dibHeader.header.bitMapCoreHeader.bits_per_pixel *
                    bitmap->dibHeader.header.bitMapCoreHeader.bitmap_width + 31) / 32) * 4 *
                    bitmap->dibHeader.header.bitMapCoreHeader.bitmap_height;
            break;

        case BITMAPINFOHEADER:
            pixel_array_size  = ((bitmap->dibHeader.header.bitMapInfoHeader.bits_per_pixel *
                    bitmap->dibHeader.header.bitMapInfoHeader.bitmap_width + 31) / 32) * 4 *
                    bitmap->dibHeader.header.bitMapInfoHeader.bitmap_height;
            break;

        case OS22XBITMAPHEADER:
        case OS22XBITMAPHEADER_S:
        case BITMAPV2INFOHEADER:
        case BITMAPV3INFOHEADER:
        case BITMAPV4HEADER:
        case BITMAPV5HEADER:
        default:
            printf("Bitmap format with header size %u not yet supported.\n", bitmap->dibHeader.type);
            return 0;
    }

    // allocate space for pixel array
    bitmap->pixel_array = malloc(pixel_array_size);

    // move to offset of array
    fseek(bitmapFilePtr, bitmap->bmpFileHeader.img_offset, SEEK_SET);
    fread(bitmap->pixel_array, 1, pixel_array_size, bitmapFilePtr);

    return 1;
}

/**
 * Prints all fields of bitmap file header.
 *
 * @param bmpFileHeader bitmap file header struct to be printed.
 */
void printBMPFileHeader(const bmpFileHeader_t *bmpFileHeader) {
    printf("Signature: 0x%X\n", bmpFileHeader->signature);
    printf("File Size: %u\n", bmpFileHeader->file_size);
    printf("Reserved: 0x%X%X\n", bmpFileHeader->rsv0, bmpFileHeader->rsv1);
    printf("Image Offset: %u\n", bmpFileHeader->img_offset);
}

/**
 * Prints all fields of the DIB header/bitmap information header.
 *
 * Currently works for all supported DIB header types but will split to separate functions as more support added.
 *
 * @param dibHeader DIB header to be printed.
 */
void printDIBHeader(const dibHeader_t *dibHeader) {
    switch(dibHeader->type) {
        case BITMAPCOREHEADER:
            printf("DIB Header size: %u\n", dibHeader->header.bitMapCoreHeader.header_size);
            printf("Width in pixels: %u\n", dibHeader->header.bitMapCoreHeader.bitmap_width);
            printf("Height in pixels: %u\n", dibHeader->header.bitMapCoreHeader.bitmap_height);
            printf("Color planes: %u\n", dibHeader->header.bitMapCoreHeader.color_planes);
            printf("Bits per pixel: %u: \n", dibHeader->header.bitMapCoreHeader.bits_per_pixel);
            break;

        case BITMAPINFOHEADER:
            printf("DIB Header size: %u\n", dibHeader->header.bitMapInfoHeader.header_size);
            printf("Width in pixels: %u\n", dibHeader->header.bitMapInfoHeader.bitmap_width);
            printf("Height in pixels: %u\n", dibHeader->header.bitMapInfoHeader.bitmap_height);
            printf("Color planes: %u\n", dibHeader->header.bitMapInfoHeader.color_planes);
            printf("Bits per pixel: %u: \n", dibHeader->header.bitMapInfoHeader.bits_per_pixel);
            printf("Compression method: %u: \n", dibHeader->header.bitMapInfoHeader.compression_method);
            printf("Image size: %u: \n", dibHeader->header.bitMapInfoHeader.image_size);
            printf("Horizontal res: %u: \n", dibHeader->header.bitMapInfoHeader.horizontal_res);
            printf("Vertical res: %u: \n", dibHeader->header.bitMapInfoHeader.vertical_res);
            printf("Colors in palette: %u: \n", dibHeader->header.bitMapInfoHeader.color_palette);
            printf("Important colors: %u: \n", dibHeader->header.bitMapInfoHeader.important_colors);
            break;

        case OS22XBITMAPHEADER:
        case OS22XBITMAPHEADER_S:
        case BITMAPV2INFOHEADER:
        case BITMAPV3INFOHEADER:
        case BITMAPV4HEADER:
        case BITMAPV5HEADER:
        default:
            printf("Bitmap format with header size %u not yet supported for print.\n", dibHeader->type);
    }
}

/**
 * Prints all information fields of bitmap file.
 *
 * Calls printBMPFileHeader() and printDIBHeader() to print bitmap file header and DIB header.
 *
 * @param bitmap bitmap struct to be printed.
 */
void printBitmapHeaders(const bitmap_t *bitmap) {
    printBMPFileHeader(&(bitmap->bmpFileHeader));
    printDIBHeader(&(bitmap->dibHeader));
}
