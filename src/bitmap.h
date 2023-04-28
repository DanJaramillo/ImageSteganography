/** @file bitmap.h
 *
 * @brief Read bitmap file from long term storage and parse to structures in memory.
 * @author Daniel Jaramillo
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmpconstants.h"

/**
 * Struct to hold bitmap file header.
 */
typedef struct bmpFileHeader {
    uint16_t    signature;
    uint32_t    file_size;
    uint16_t    rsv0;
    uint16_t    rsv1;
    uint32_t    img_offset;
} bmpFileHeader_t;

/**
 * Struct to hold DIB header of type BITMAPCOREHEADER.
 */
typedef struct bitMapCoreHeader {
    uint32_t    header_size;
    uint16_t    bitmap_width;
    uint16_t    bitmap_height;
    uint16_t    color_planes;
    uint16_t    bits_per_pixel;
} bitMapCoreHeader_t;

/**
 * Struct to hold DIB header of type BITMAPINFOHEADER.
 */
typedef struct bitMapInfoHeader {
    uint32_t header_size;
    uint32_t bitmap_width;
    uint32_t bitmap_height;
    uint16_t color_planes;
    uint16_t bits_per_pixel;
    uint32_t compression_method;
    uint32_t image_size;
    uint32_t horizontal_res;
    uint32_t vertical_res;
    uint32_t color_palette;
    uint32_t important_colors;
} bitMapInfoHeader_t;

/**
 * Struct to hold DIB header of type OS22XBITMAPHEADER.
 * Not yet implemented.
 */
typedef struct os22xBitMapHeader {
    uint32_t header_size;
    uint32_t bitmap_width;
    uint32_t bitmap_height;
    uint16_t color_planes;
    uint16_t bits_per_pixel;
    uint32_t compression_method;
    uint32_t image_size;
    uint32_t horizontal_res;
    uint32_t vertical_res;
    uint32_t color_palette;
    uint32_t important_colors;
    uint16_t units;
    uint16_t fill_direction;
    uint16_t halftoning_alg;
    uint32_t halftoning_param1;
    uint32_t halftoning_param2;
    uint32_t color_encoding;
    uint32_t app_id;
} os22xBitMapHeader_t;

/**
 * Struct with union to hold any type of DIB header.
 */
typedef struct dibHeader {
    uint32_t type;
    union {
        bitMapCoreHeader_t bitMapCoreHeader;
        bitMapInfoHeader_t bitMapInfoHeader;
        os22xBitMapHeader_t os22XBitMapHeader;
    } header;
} dibHeader_t;

/**
 * Struct to hold all bitmap information in memory.
 */
typedef struct bitmap {
    bmpFileHeader_t bmpFileHeader;
    dibHeader_t dibHeader;
    uint8_t *pixel_array;
    uint8_t *color_table;   // not currently implemented.
}bitmap_t;

/**
 * Reads the bitmap file header and parses it to a bmpFileHeader struct.
 *
 * @param bitmapFilePtr FILE pointer for bitmap file.
 * @param bmpFileHeader bitmap file header struct.
 * @return size of BMP file header on success, 0 otherwise.
 */
uint32_t readBMPFileHeader(FILE *bitmapFilePtr, bmpFileHeader_t *bmpFileHeader);

/**
 * Reads the DIB header, bitmap information header, to a dibHeader struct.
 *
 * @param bitmapFilePtr FILE pointer for bitmap file.
 * @param dibHeaderSize size of DIB header.
 * @param dibHeader dib header struct.
 * @return size of DIB header on success, 0 otherwise
 */
uint32_t readDIBHeader(FILE *bitmapFilePtr, uint32_t dibHeaderSize, dibHeader_t *dibHeader);

/**
 * Parses the block of memory containing bitmap file header into bitmap file header struct.
 *
 * @param buffer block of memory containing bitmap file header.
 * @param bmpFileHeader bitmap file header struct.
 * @return size of BMP file header on success, 0 otherwise.
 */
uint32_t parseBMPFileHeader(const uint8_t *buffer, bmpFileHeader_t *bmpFileHeader);

/**
 * Parses DIB header, bitmap information header, of BITMAPCOREHEADER format.
 *
 * @param buffer block of memory containing bitmap header, dib header.
 * @param dibHeader struct to hold bitmap header, dib header.
 * @return 1 on success, 0 on failure.
 */
uint32_t parseBITMAPCOREHEADER(const uint8_t *buffer, dibHeader_t *dibHeader);

/**
 * Parses DIB header, bitmap information header, of BITMAPINFOHEADER format.
 *
 * @param buffer block of memory containing bitmap header, dib header.
 * @param dibHeader struct to hold bitmap header, dib header.
 * @return 1 on success, 0 on failure.
 */
uint32_t parseBITMAPINFOHEADER(const uint8_t *buffer, dibHeader_t *dibHeader);

/**
 * Parses all formats of DIB header/bitmap information header.
 *
 * @param buffer block of memory containing bitmap header, dib header.
 * @param dibHeaderSize size of header being parsed.
 * @param dibHeader struct to hold bitmap information header, dib header.
 * @return returns 1 on success, 0 otherwise.
 */
uint32_t parseDIBHeader(const uint8_t *buffer, uint32_t dibHeaderSize, dibHeader_t *dibHeader);

/**
 * Reads the bitmap file and parses it into structs in memory.
 *
 * @param bitmapFilePtr file pointer to the bitmap file being read.
 * @param bitmap struct containing bitmap in memory.
 * @return 1 on success, 0 on error.
 */
uint32_t readBitmapFile(FILE *bitmapFilePtr, bitmap_t *bitmap);

/**
 * Prints all fields of bitmap file header.
 *
 * @param bmpFileHeader bitmap file header struct to be printed.
 */
void printBMPFileHeader(const bmpFileHeader_t *bmpFileHeader);

/**
 * Prints all fields of the DIB header/bitmap information header.
 *
 * @param dibHeader DIB header to be printed.
 */
void printDIBHeader(const dibHeader_t *dibHeader);

/**
 * Prints all information fields of bitmap file.
 *
 * @param bitmap bitmap struct to be printed.
 */
void printBitmapHeaders(const bitmap_t *bitmap);

#endif