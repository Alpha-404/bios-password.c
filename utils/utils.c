#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "utils.h"

const CharMap keyboardDict[] = {
    {2, '1'},  {3, '2'},  {4, '3'},  {5, '4'},  {6, '5'},  {7, '6'},  {8, '7'},  {9, '8'},
    {10, '9'}, {11, '0'}, {16, 'q'}, {17, 'w'}, {18, 'e'}, {19, 'r'}, {20, 't'}, {21, 'y'},
    {22, 'u'}, {23, 'i'}, {24, 'o'}, {25, 'p'}, {30, 'a'}, {31, 's'}, {32, 'd'}, {33, 'f'},
    {34, 'g'}, {35, 'h'}, {36, 'j'}, {37, 'k'}, {38, 'l'}, {44, 'z'}, {45, 'x'}, {46, 'c'},
    {47, 'v'}, {48, 'b'}, {49, 'n'}, {50, 'm'}
};

const size_t keyboardDict_SIZE = sizeof(keyboardDict) / sizeof(keyboardDict[0]);


char lookup(const CharMap *table, size_t size, char key) {
    for (size_t i = 0; i < size; i++) {
        if (table[i].key == key)
            return table[i].value;
    }
    return '\0';
}

char lookupReverse(const CharMap *table, size_t size, char value) {
    for (size_t i = 0; i < size; i++) {
        if (table[i].value == value)
            return table[i].key;
    }
    return '\0';
}

CharMap *generateReverseKeyDict(const CharMap *dict, size_t size) {
    CharMap *result = malloc(sizeof(CharMap) * size);
    if (!result) return NULL;

    for (size_t i = 0; i < size; i++) {
        result[i].key = dict[i].value;
        result[i].value = dict[i].key;
    }
    return result;
}

char keyboardEncToAscii(int number) {
    return lookup(keyboardDict, keyboardDict_SIZE, number);
}

char *keyboardEncArrToAscii(int *arr, int len) {
    char *output = malloc(len + 1);
    if (!output) return NULL;

    for (int i = 0; i < len; i++) {
        output[i] = keyboardEncToAscii(arr[i]);
    }
    output[len] = '\0';
    return output;
}


int asciiToKeyboardEnc(char code) {
    //return lookup(generateReverseKeyDict(keyboardDict, keyboardDict_SIZE), keyboardDict_SIZE, code);
    return lookupReverse(keyboardDict, keyboardDict_SIZE, code);
}

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

