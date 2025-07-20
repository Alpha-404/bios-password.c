#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint32_t crc;
    const uint32_t* table;
} Crc32;

static uint32_t crc32_ieee_table[256];
static int crc32_table_computed = 0;

static void make_crc32_table(uint32_t poly, uint32_t* table) {
    for (int i = 0; i < 256; i++) {
        uint32_t crc = (uint32_t)i;
        for (int j = 0; j < 8; j++) {
            if (crc & 1)
                crc = (crc >> 1) ^ poly;
            else
                crc >>= 1;
        }
        table[i] = crc;
    }
}

static const uint32_t* get_crc32_table(uint32_t poly) {
    if (!crc32_table_computed && poly == 0xEDB88320) {
        make_crc32_table(poly, crc32_ieee_table);
        crc32_table_computed = 1;
    }
    return crc32_ieee_table;
}

void crc32_init(Crc32* c, uint32_t poly) {
    c->table = get_crc32_table(poly);
    c->crc = 0;
}

void crc32_reset(Crc32* c) {
    c->crc = 0;
}

void crc32_update(Crc32* c, const uint8_t* data, size_t len) {
    c->crc ^= 0xFFFFFFFF;
    for (size_t i = 0; i < len; i++) {
        uint8_t index = (uint8_t)((c->crc ^ data[i]) & 0xFF);
        c->crc = (c->crc >> 8) ^ c->table[index];
    }
    c->crc ^= 0xFFFFFFFF;
}

uint32_t crc32_digest(Crc32* c) {
    return c->crc;
}

void crc32_hexdigest(Crc32* c, char* out) {
    sprintf(out, "%08x", c->crc);
}
