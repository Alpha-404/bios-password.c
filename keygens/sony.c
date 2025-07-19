#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "sony.h"

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

char *sonySolver(const char *serial) {
    regex_t regex;
    int reti;

    const char *pattern = "^[0-9]{8}$";

    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        return 0;
    }

    reti = regexec(&regex, serial, 0, NULL, 0);
    regfree(&regex);

    if (reti) {
        return sonyKeygen(serial);
    } else {
        fprintf(stderr, "Invalid serial\n");
        return 0;
    }
}