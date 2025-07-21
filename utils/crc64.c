#include "crc64.h"
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

uint64_t crc_table[256];

static void crc64_make_table(Crc64 *ctx) {
    for (int i = 0; i < CRC64_TABLE_SIZE; ++i) {
        uint64_t crc = i;
        for (int j = 0; j < 8; ++j) {
            if (crc & 1) {
                crc = (crc >> 1) ^ ctx->poly;
            } else {
                crc >>= 1;
            }
        }
        ctx->table[i] = crc;
    }
}

void crc64_init(Crc64 *ctx) {
    ctx->poly = POLY;
    ctx->crc = 0;
    crc64_make_table(ctx);
}

void crc64_reset(Crc64 *ctx) {
    ctx->crc = 0;
}

void crc64_update(Crc64 *ctx, const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        uint8_t b = data[i];
        uint8_t index = (uint8_t)(ctx->crc ^ b);
        ctx->crc = ctx->table[index] ^ (ctx->crc >> 8);
    }
}

uint64_t crc64_digest(Crc64 *ctx) {
    return ctx->crc;
}

void crc64_hexdigest(Crc64 *ctx, char out[17]) {
    sprintf(out, "%016llx", (unsigned long long)ctx->crc);
}