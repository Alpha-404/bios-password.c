// cryptoUtils.h
#ifndef CRYPTOUTILS_H
#define CRYPTOUTILS_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint32_t crc;
    const uint32_t* table;
} Crc32;

void crc32_init(Crc32* c, uint32_t poly);
void crc32_reset(Crc32* c);
void crc32_update(Crc32* c, const uint8_t* data, size_t len);
uint32_t crc32_digest(Crc32* c);
void crc32_hexdigest(Crc32* c, char* out);

#endif
