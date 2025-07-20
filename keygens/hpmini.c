#include<stdio.h>
#include <string.h>
#include <regex.h>
#include "hpmini.h"
#include <stdlib.h>
#include "../utils/utils.h"

CharMap table1[] = {
    {'1', '3'}, {'0', '1'}, {'3', 'F'}, {'2', '7'}, {'5', 'Q'}, {'4', 'V'},
    {'7', 'X'}, {'6', 'G'}, {'9', 'O'}, {'8', 'U'}, {'a', 'C'}, {'c', 'E'},
    {'b', 'P'}, {'e', 'M'}, {'d', 'T'}, {'g', 'H'}, {'f', '8'}, {'i', 'Y'},
    {'h', 'Z'}, {'k', 'S'}, {'j', 'W'}, {'m', '4'}, {'l', 'K'}, {'o', 'J'},
    {'n', '9'}, {'q', '5'}, {'p', '2'}, {'s', 'N'}, {'r', 'B'}, {'u', 'L'},
    {'t', 'A'}, {'w', 'D'}, {'v', '6'}, {'y', 'I'}, {'x', '4'}, {'z', '0'}
};

CharMap table2[] = {
    {'1', '3'}, {'0', '1'}, {'3', 'F'}, {'2', '7'}, {'5', 'Q'}, {'4', 'V'},
    {'7', 'X'}, {'6', 'G'}, {'9', 'O'}, {'8', 'U'}, {'a', 'C'}, {'c', 'E'},
    {'b', 'P'}, {'e', 'M'}, {'d', 'T'}, {'g', 'H'}, {'f', '8'}, {'i', 'Y'},
    {'h', 'Z'}, {'k', 'S'}, {'j', 'W'}, {'m', '4'}, {'l', 'K'}, {'o', 'J'},
    {'n', '9'}, {'q', '5'}, {'p', '2'}, {'s', 'N'}, {'r', 'B'}, {'u', 'L'},
    {'t', 'A'}, {'w', 'D'}, {'v', '6'}, {'y', 'I'}, {'x', 'R'}, {'z', '0'}
};

#define TABLE1_SIZE (sizeof(table1) / sizeof(CharMap))
#define TABLE2_SIZE (sizeof(table2) / sizeof(CharMap))

char *hpminiKeygen(const char *serial) {
    char password1[64] = {0};
    char password2[64] = {0};

    char *lowserial = strToLower(serial);

    for (int i = 0; i < strlen(serial); i++) {
        char c1 = lookup(table1, TABLE1_SIZE, lowserial[i]);
        char c2 = lookup(table2, TABLE2_SIZE, lowserial[i]);

        if (c1) password1[strlen(password1)] = c1;
        if (c2) password2[strlen(password2)] = c2;
    }

    free(lowserial);

    if (strcmp(password1, password2) == 0) {
        return strToLower(strdup(password1));
    } else {
        size_t total_len = strlen(password1) + strlen(password2) + 3;
        char *combined = malloc(total_len);
        if (!combined) return NULL;
        snprintf(combined, total_len, "%s, %s", strToLower(password1), strToLower(password2));
        return combined;
    }
}