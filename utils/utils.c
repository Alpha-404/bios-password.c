#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char *strToLower(const char *input) {
    if (!input) return NULL;

    size_t len = 0;
    while (input[len]) len++;

    char *lower = malloc(len + 1);
    if (!lower) return NULL;

    for (size_t i = 0; i < len; i++) {
        lower[i] = tolower((unsigned char)input[i]);
    }
    lower[len] = '\0';

    return lower;
}
