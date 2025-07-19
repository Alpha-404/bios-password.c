#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "keygens/sony.h"

typedef char* (*KeygenFunc)(const char*);

struct {
    const char *vendor;
    KeygenFunc func;
} keygens[] = {
    { "sony", sonySolver },
    { NULL, NULL }
};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s VENDOR SERIAL\n", argv[0]);
        return 1;
    }

    const char *vendor = argv[1];
    const char *serial = argv[2];
    char *code = NULL;

    for (int i = 0; keygens[i].vendor; ++i) {
        if (strcmp(keygens[i].vendor, vendor) == 0) {
            code = keygens[i].func(serial);
            break;
        }
    }

    if (!code) {
        fprintf(stderr, "Unknown vendor or failed to generate code.\n");
        return 1;
    }

    printf("Unlock code: %s\n", code);
    free(code);
    return 0;
}
