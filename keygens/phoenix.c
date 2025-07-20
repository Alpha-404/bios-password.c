#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils/utils.h"
#include "phoenix.h"
 
#define MAX_ATTEMPTS 7000000

static PhoenixSolver* currentPhoenixSolver = NULL;
static PhoenixInfo *currentPhoenixInfo = NULL;

const char digitsOnly[] = "123456789";
const char lettersOnly[] = "abcdefghijklmnopqrstuvwxyz";

PhoenixInfo defaultPhoenix = {
  0,
  0,
  lettersOnly,
  3,
  7  
};

/* The phoenix implementation of the CRC-16 contains a rather severe bug
 * quartering the image space of the function: both the first and second MSB
 * are always zero regardless of the input.
 * For a working implementation, you'd have to change the polynom from 0x2001
 * to e.g. 0xA001.
 */

int badCRC16(const int *pwd, size_t len, int salt) {
    int hash = salt;
    for (size_t c = 0; c < len; c++) {
        hash ^= pwd[c];
        for (int i = 0; i < 8; i++) {
            if (hash & 1) {
                hash = (hash >> 1) ^ 0x2001;
            } else {
                hash = hash >> 1;
            }
        }
    }
    return hash;
}

int searchBadCRC16(const int *pwd, int len, int salt, int requiredHash, int minLen) {
    minLen--;
    int hash = salt;
    for (size_t c = 0; c < len; c++) {
        hash ^= pwd[c];
        for (int i = 0; i < 8; i++) {
            if (hash & 1) {
                hash = (hash >> 1) ^ 0x2001;
            } else {
                hash = hash >> 1;
            }
        }

        if (c >= minLen && hash == requiredHash) {
            return c + 1;
        }
    }
    return -1; // not found
}

void generatePhoenixPassword(int *encodedPwd, int encodedLen, const char *characters, int charLen) {
    srand(0);

    CharMap *reversedScanCodes = generateReverseKeyDict(keyboardDict, keyboardDict_SIZE);

    /*
    for (int i = 0; i < encodedLen; i++) {
        int index = rnd % charLen; 
        char ch = characters[index];

        encodedPwd[i] = lookup(reversedScanCodes, keyboardDict_SIZE, ch);
        rnd *= encodedLen;
    }
    */

    double rnd = ((double) rand() / (RAND_MAX + 1.0)) * charLen;
    for (int i = 0; i < encodedLen; i++) {
        int index = (int)fmod(rnd, charLen);
        encodedPwd[i] = lookup(reversedScanCodes, keyboardDict_SIZE, characters[index]);
        rnd *= encodedLen;
    }

    free(reversedScanCodes);
}

char* bruteforce(int hash, int salt, const char *characters, int minLen, int maxLen) {
    int encodedPwd[64] = {0};

    if (hash > 0x3FFF) {
        return "BadHash";
    }

    int attempt = 0;
    while (1) {
        attempt++;
        if (attempt > MAX_ATTEMPTS) {
            return "NotFound";
        }

        generatePhoenixPassword(encodedPwd, maxLen, characters, strlen(characters));
        
        int found = searchBadCRC16(encodedPwd, maxLen, salt, hash, minLen);
        //printf("%d\n", found);
        if (found != -1) {
            return keyboardEncArrToAscii(encodedPwd, found);
        }
    }
}

void cleaner(char* code) {
    char* src = code;
    char* dst = code;

    while (*src) {
        if (*src != '-' && !isspace((unsigned char)*src)) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}

int validator(const char* code) {
    if (strlen(code) != 5) return 0;
    for (int i = 0; i < 5; i++) {
        if (!isdigit((unsigned char)code[i])) return 0;
    }
    return 1;
}

int calculateHash(const char* password, PhoenixInfo* info) {
    int encoded[64];
    int len = 0;

    while (password[len] != '\0' && len < 64) {
        encoded[len] = asciiToKeyboardEnc(password[len]);
        len++;
    }

    //printf("%d\n", info->salt);

    return badCRC16(encoded, len, info->salt) - info->shift;
}


char** keygen(const char* code, PhoenixInfo* info) {
    /*
    int hash = atoi(code) + info->shift;

    char* pwd = bruteforce(hash, info->salt, info->dictionary, info->minLen, info->maxLen);
    char** results = malloc(sizeof(char*) * 2); // max 1 result + NULL
    results[0] = pwd ? strdup(pwd) : NULL;
    results[1] = NULL;
    return results;
    */
    int hash = calculateHash(code, info);
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%d", hash);

    char** results = malloc(sizeof(char*) * 2);
    if (!results) return NULL;

    results[0] = strdup(buffer);
    results[1] = NULL;

    return results;
}

static char* phoenixKeygenWrapper(const char* code) {
    if (!currentPhoenixInfo) {
        fprintf(stderr, "ERROR: phoenixKeygenWrapper called without PhoenixInfo set\n");
        return NULL;
    }
    char** results = keygen(code, currentPhoenixInfo);
    char* result = NULL;

    if (results) {
        for (int i = 0; results[i]; ++i) {
            if (!currentPhoenixSolver || !currentPhoenixSolver->validator || currentPhoenixSolver->validator(results[i])) {
                result = strdup(results[i]);
                break;
            }
        }

        // Free all results
        for (int i = 0; results[i]; ++i) {
            free(results[i]);
        }
        free(results);
    }

    return result;
}
PhoenixSolver makePhoenixSolver(const PhoenixBios* description) {
    PhoenixSolver solver = {
        .base = makeSolver(
        description && description->name ? description->name : "unknown",
        description && description->description ? description->description : "unknown",
        description && description->pattern ? description->pattern : ".*", // or description->regex if you want regex support
        phoenixKeygenWrapper // correct signature
        ),
        .info = defaultPhoenix,
        .validator = validator,
        .cleaner = cleaner,
        .keygen = keygen,
        .calculateHash = calculateHash
    };

    if (description) {
        if (description->salt) solver.info.salt = description->salt;
        if (description->shift) solver.info.shift = description->shift;
        if (description->dictionary) solver.info.dictionary = description->dictionary;
    }

    PhoenixInfo* infoCopy = malloc(sizeof(PhoenixInfo));
    if (infoCopy == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    *infoCopy = solver.info;
    currentPhoenixInfo = infoCopy;

    PhoenixSolver* persistent = malloc(sizeof(PhoenixSolver));
    *persistent = solver;
    currentPhoenixSolver = persistent;

    return *persistent;
}