#include "insyde.h"
#include <regex.h>            // for regfree, regcomp, regexec, REG_EXTENDED
#include <stdint.h>           // for uint8_t
#include <stdio.h>            // for NULL, snprintf, size_t
#include <stdlib.h>           // for malloc, atoi
#include <string.h>           // for strlen, memcpy, strcmp, strncpy
#include "../utils/aes128.h"  // for aes128_encrypt_block, aes128_key_expansion
#include "../utils/crc64.h"   // for crc64_hexdigest, crc64_init, crc64_update
#include "../utils/sha256.h"  // for sha256_final, sha256_init, sha256_update

#define INSYDE_SALT "Insyde Software Corp."

uint8_t* fun0(const uint8_t arr[16], uint8_t* output);
uint8_t* fun1(const uint8_t arr[16], uint8_t* output);
uint8_t* fun2(const uint8_t arr[16], uint8_t* output);
uint8_t* fun3(const uint8_t arr[16], uint8_t* output);
uint8_t* fun4(const uint8_t arr[16], uint8_t* output);
uint8_t* fun5(const uint8_t arr[16], uint8_t* output);
uint8_t* keyProcess(const uint8_t arr[32], uint8_t* output);

uint8_t* insydeAcerSwitch(const uint8_t arr[32], uint8_t* output) {
    if (!arr) return NULL;

    uint8_t temp[16];
    keyProcess(arr, temp);

    switch (arr[8] % 6) {
        case 0:
            return fun0(temp, output);
        case 1:
            return fun1(temp, output);
        case 2:
            return fun2(temp, output);
        case 3:
            return fun3(temp, output);
        case 4:
            return fun4(temp, output);
        default:
            return fun5(temp, output);
    }
}

// Definitions

uint8_t* fun0(const uint8_t arr[16], uint8_t* output) {
    int k = 0;
    for (int i = 3; i >= 0; i--) {
        for (int j = 0; j < 16; j += 4) {
            output[k++] = arr[i + j];
        }
    }
    return output;
}

uint8_t* fun1(const uint8_t arr[16], uint8_t* output) {
    int k = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 12; j >= 0; j -= 4) {
            output[k++] = arr[i + j];
        }
    }
    return output;
}

uint8_t* fun2(const uint8_t arr[16], uint8_t* output) {
    int k = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            output[k++] = (arr[((j + i) & 3) + i * 4] + i) & 0xFF;
        }
    }
    return output;
}

uint8_t* fun3(const uint8_t arr[16], uint8_t* output) {
    int k = 0;
    uint8_t acc1 = 0;
    uint8_t acc2 = 0;
    for (int i = 0; i < 4; i++) {
        acc1 ^= arr[i * 5];
        acc2 ^= arr[i * 3 + 3];
    }
    for (int i = 0; i < 16; i++) {
        uint8_t pivot = ((i & 1) == 0) ? acc1 : acc2;
        output[k++] = arr[i] ^ pivot;
    }
    return output;
}

uint8_t* fun4(const uint8_t arr[16], uint8_t* output) {
    int k = 0;
    for (int i = 0; i < 16; i++) {
        uint8_t temp1 = arr[i];
        uint8_t temp2 = arr[(i + 1) & 0xF];
        uint8_t pivot = (temp2 < temp1) ? temp2 : 0xFF;
        output[k++] = temp1 ^ pivot;
    }
    return output;
}

uint8_t* fun5(const uint8_t arr[16], uint8_t* output) {
    for (int i = 0; i < 4; i++) {
        uint8_t acc = 0;
        for (int j = 0; j < 16; j += 4) {
            acc ^= arr[j + i];
        }
        for (int j = 0; j < 16; j += 4) {
            int idx = i + j;
            output[idx] = (arr[idx] * acc) & 0xFF;
        }
    }
    return output;
}

uint8_t* keyProcess(const uint8_t arr[32], uint8_t* output) {
    for (int i = 0; i < 16; i++) {
        int acc = 0;
        for (int j = 0; j < 8; j++) {
            acc += arr[((i >> 2) << 3) + j] * arr[j * 4 + (i & 3)];
        }
        output[i] = acc & 0xFF;
    }
    return output;
}

static void rotatefun(const uint8_t *input, uint8_t *output) {
    uint8_t idx = input[9] & 0xF;
    for (int i = 0; i < 16; i++) {
        output[i] = input[((idx * 2 + 1) * i) % 32];
    }
}

char *acerInsydeKeygen(const char *serial) {
    size_t len = strlen(serial);
    if (len == 0 || len > 256) return NULL;

    uint8_t inputBytes[256];
    for (size_t i = 0; i < len; i++) {
        inputBytes[i] = serial[i] & 0xFF;
    }

    uint8_t digest[32];

    Sha256 ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, inputBytes, len);
    sha256_final(&ctx, digest);

    //sha256(inputBytes, len, digest);

    uint8_t key[16];
    insydeAcerSwitch(digest, key);

    uint8_t blockData[16];
    rotatefun(digest, blockData);

    uint8_t encrypted[16];
    AES128 aes;
    aes128_key_expansion(&aes, key);
    aes128_encrypt_block(&aes, blockData, encrypted);

    Crc64 crc;
    crc64_init(&crc);
    crc64_update(&crc, encrypted, 16);

    char *hex = malloc(17); // 16 chars + null terminator
    if (!hex) return NULL;
    crc64_hexdigest(&crc, hex);

    return hex; // caller must free()
}

char *insydeKeygen(const char *serial) {
    const char *salt1 = INSYDE_SALT;
    const char *salt2 = ":\x16@>\x14""96H\x07.\x0f\x0e\nG-MDGHBT";

    int num = atoi(serial);
    char num_str[32];
    snprintf(num_str, sizeof(num_str), "%d", num);

    char serial2[9] = {0};
    size_t len = strlen(num_str);
    if (len > 8) len = 8;
    memcpy(serial2, num_str, len);

    char password1[9] = {0};
    char password2[9] = {0};
    char password3[9] = {0};

    for (int i = 0; i < 8; i++) {
        int b = (salt1[i] + i) ^ serial[i];
        password1[i] = '0' + (b % 10);

        b = (salt1[i] + i) ^ serial2[i];
        password2[i] = '0' + (b % 10);

        b = salt2[i] ^ serial2[i];
        password3[i] = '0' + (b % 10);
    }

    password1[8] = '\0';
    password2[8] = '\0';
    password3[8] = '\0';

    char *combined;
    if (strcmp(password1, password2) == 0) {
        size_t total_len = strlen(password1) + strlen(password3) + 3; // for ", " and \0
        combined = malloc(total_len);
        if (!combined) return NULL;
        snprintf(combined, total_len, "%s, %s", password1, password3);
    } else {
        size_t total_len = strlen(password1) + strlen(password2) + strlen(password3) + 6; // ", " twice + \0
        combined = malloc(total_len);
        if (!combined) return NULL;
        snprintf(combined, total_len, "%s, %s, %s", password1, password2, password3);
    }

    return combined;
}

char *hpInsydeKeygen(const char *input) {
    const char *salt1 = "c6B|wS^8";
    const char *salt2 = INSYDE_SALT;
    regex_t regex;
    regmatch_t matches[2];

    if (regcomp(&regex, "^i[[:space:]]*([0-9]{8})$", REG_EXTENDED | REG_ICASE) != 0) {
        return NULL;
    }

    if (regexec(&regex, input, 2, matches, 0) != 0) {
        regfree(&regex);
        return NULL;
    }

    char serial[9];
    int len = matches[1].rm_eo - matches[1].rm_so;
    strncpy(serial, input + matches[1].rm_so, len);
    serial[len] = '\0';

    regfree(&regex);
    
    char password1[9] = {0};
    char password2[9] = {0};

    for (int i = 0; i < 8; i++) {
        int b = (salt1[i] + i) ^ serial[i];
        password1[i] = '0' + (b % 10);

        b = (salt2[i] + i) ^ serial[i];
        password2[i] = '0' + (b % 10);
    }

    char *combined;
    size_t total_len = strlen(password1) + strlen(password2) + 3; // for ", " and \0
    combined = malloc(total_len);
    if (!combined) return NULL;
    snprintf(combined, total_len, "%s, %s", password1, password2);
    return combined;
}
