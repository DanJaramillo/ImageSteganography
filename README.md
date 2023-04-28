# ImageSteganography
Reads a message hidden in a bitmap image.

Currently only supports 24-bit per pixel bitmaps without compression or padding. The message is a C string. The least significant bit of each subpixel represents a bit of the encoded message. The bits are encoded from least significant to most siginificant bit of each byte and from first byte in the string to the last.
