#ifndef AES128_H
#define AES128_H

#include <stdint.h>

typedef struct {
    uint8_t roundKey[176]; // 11 * 16 bytes
} AES128;

void aes128_key_expansion(AES128 *ctx, const uint8_t key[16]);
void aes128_encrypt_block(const AES128 *ctx, const uint8_t input[16], uint8_t output[16]);

#endif // AES128_H