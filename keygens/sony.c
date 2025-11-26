#include "sony.h"
#include <stdlib.h>  // for malloc, size_t, NULL
#include <string.h>  // for strlen

//1234567 = 9648669

char *sonyKeygen(const char *serial) {
    const char *table = "0987654321876543210976543210982109876543109876543221098765436543210987";
    size_t len = strlen(serial);
    char *code = malloc(len + 1); // +1 for null terminator
    if (!code) return NULL;

    for (size_t i = 0; i < len; ++i) {
        int index = (serial[i] - '0') + 10 * i;
        code[i] = table[index];
    }
    code[len] = '\0';
    return code;
}
