#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>

typedef struct {
    int key;
    char value;
} CharMap;

extern const CharMap keyboardDict[];
extern const size_t keyboardDict_SIZE;

char *strToLower(const char *input);

char lookup(const CharMap *table, size_t size, char key);

char lookupReverse(const CharMap *table, size_t size, char value);

CharMap *generateReverseKeyDict(const CharMap *dict, size_t size);

char keyboardEncToAscii(int number);

char *keyboardEncArrToAscii(int *arr, int len);

int asciiToKeyboardEnc(char code);

#endif