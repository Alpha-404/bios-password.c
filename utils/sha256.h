#ifndef SHA256_H
#define SHA256_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint32_t state[8];
    uint8_t data[64];
    uint64_t bitlen;
    size_t datalen;
} Sha256;

void sha256_init(Sha256 *ctx);
void sha256_update(Sha256 *ctx, const uint8_t *data, size_t len);
void sha256_final(Sha256 *ctx, uint8_t hash[32]);

#endif // SHA256_H
