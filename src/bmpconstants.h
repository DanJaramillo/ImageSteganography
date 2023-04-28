#ifndef BMPCONSTANTS_H_
#define BMPCONSTANTS_H_


    /**
     * Size of bitmap file header.
     */
    #define BMPFILEHEADERSIZE       14

/**
 * Value of file signitures for supported file types in bitmap file header.
 */
    #define BM  0x424D
    #define BA  0x4D4C
    #define CI  0x4349
    #define CP  0x4350
    #define IC  0x4943
    #define PT  0x5054

/**
 * DIB header/bitmap information header types identified by size in bytes of header.
 */
    #define BITMAPCOREHEADER        12
    #define OS21XBITMAPHEADER       12
    #define OS22XBITMAPHEADER       64
    #define OS22XBITMAPHEADER_S     16
    #define BITMAPINFOHEADER        40
    #define BITMAPV2INFOHEADER      52
    #define BITMAPV3INFOHEADER      56
    #define BITMAPV4HEADER          108
    #define BITMAPV5HEADER          124

/**
 * Value of compression type field in BITMAPINFOHEADER for different compression types.
 */
    #define BI_RGB              0
    #define BI_RLE8             1
    #define BI_RLE4             2
    #define BI_BITFIELDS        3
    #define BI_JPEG             4
    #define BI_PNG              5
    #define BI_ALPHABITFIELDS   6
    #define BI_CMYK             11
    #define BI_CMYKRLE8         12
    #define BI_CMYKRLE4         13

#endif