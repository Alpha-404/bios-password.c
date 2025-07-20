#include "../utils/crc32.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *hpAmiKeygen(const char *input) {
    if (strlen(input) != 8) {
        return NULL;
    }

    uint8_t salt[16] = {
        0xb9, 0xed, 0xf5, 0x69, 0x9d, 0x16, 0x49, 0xf9,
        0x8c, 0x5f, 0x7c, 0xb3, 0x68, 0x3c, 0xd4, 0xa7
    };

    uint32_t backdoor = (uint32_t)strtoul(input, NULL, 16);
    uint8_t temp[20];
    Crc32 crc;

    crc32_init(&crc, 0xEDB88320);

    for (int i = 0; i < 0x10; i++) {
        temp[i] = salt[i] ^ 0x36;
    }
    temp[0x10] = backdoor & 0xFF;
    temp[0x11] = (backdoor >> 8) & 0xFF;
    temp[0x12] = (backdoor >> 16) & 0xFF;
    temp[0x13] = (backdoor >> 24) & 0xFF;

    crc32_update(&crc, temp, 20);
    uint32_t next = crc32_digest(&crc);

    for (int i = 0; i < 0x10; i++) {
        temp[i] = salt[i] ^ 0x5C;
    }
    temp[0x10] = next & 0xFF;
    temp[0x11] = (next >> 8) & 0xFF;
    temp[0x12] = (next >> 16) & 0xFF;
    temp[0x13] = (next >> 24) & 0xFF;

    crc32_reset(&crc);
    crc32_update(&crc, temp, 20);

    char* out = malloc(9);
    if (!out) return NULL;
    crc32_hexdigest(&crc, out);
    return out;
}