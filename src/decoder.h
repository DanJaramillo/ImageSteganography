/** @file decoder.h
 *
 * @brief Decodes message hidden in bitmap file.
 * @author Daniel Jaramillo
 */

#ifndef FIRMWARE_QUIZ_DECODER_H
#define FIRMWARE_QUIZ_DECODER_H

#include <stdint.h>

#include "bitmap.h"

/**
 * Returns 1 if the bitmap is of a currently implemented decodable type.
 *
 * @param bitmap bitmap in memory to be checked
 * @return 1 if decodeable, 0 otherwise.
 */
uint16_t isDecodeable(bitmap_t *bitmap);

/**
 * Decodes the secret message embedded in bitmap data.
 *
 * @param bitmap bitmap in memory to be decoded.
 * @return pointer to char string with secret message.
 */
char *decodeMessage(bitmap_t *bitmap);

#endif //FIRMWARE_QUIZ_DECODER_H
