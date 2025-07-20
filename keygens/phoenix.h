#ifndef PHOENIX_H
#define PHOENIX_H
#include "../utils/solver.h"

typedef struct {
    char* BadHash;
    int NotFound;
} PhoenixErrors;

typedef struct {
    int shift;
    int salt;
    const char* dictionary;
    int minLen;
    int maxLen;
} PhoenixInfo;

typedef struct {
    const char* name;
    const char* description;
    int salt;
    const char* dictionary;
    const char* pattern;
} PhoenixBios;

typedef struct PhoenixSolver {
    Solver base;
    const char* name;
    const char* description;
    PhoenixInfo info;

    int (*validator)(const char* code);
    void (*cleaner)(char* code);
    char** (*keygen)(const char* code, PhoenixInfo* info);  // returns array of strings
    int (*calculateHash)(const char* password, PhoenixInfo* info);
} PhoenixSolver;

extern const char digitsOnly[];
extern const char lettersOnly[];

PhoenixSolver makePhoenixSolver(const PhoenixBios* description);

int validator(const char* code);
void cleaner(char* code);
char** keygen(const char* code, PhoenixInfo* info);
int calculateHash(const char* password, PhoenixInfo* info);

#endif