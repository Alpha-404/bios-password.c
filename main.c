#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "keygens/sony.h"
#include "keygens/hpmini.h"
#include "keygens/phoenix.h"
#include "keygens/hpami.h"

#include "utils/solver.h"

#define MAX_SOLVERS 16

typedef char* (*KeygenFunc)(const char*);

/*
struct {
    const char *vendor;
    KeygenFunc func;
} keygens[] = {
    { "sony", sonySolver },
    {"hpmini", hpminiSolver},
    { NULL, NULL }
};
*/

void list_solvers(const Solver solvers[]) {
    for (int i = 0; solvers[i].name != NULL; i++) {
        printf("%s: %s\n", solvers[i].name, solvers[i].description);
    }
}

int main(int argc, char *argv[]) {
    Solver solvers[MAX_SOLVERS];
    int count = 0;
    
    #ifdef ENABLE_SONY
        solvers[count++] = makeSolver("sony", "Sony 7-digit numeric keygen", "^[0-9]{7}$", sonyKeygen);
    #endif

    #ifdef ENABLE_HPMINI
        solvers[count++] = makeSolver("hpmini", "HP Mini 10-character alphanumeric keygen", "^[0-9A-Z]{10}$", hpminiKeygen);
    #endif

    #ifdef ENABLE_PHOENIX
        PhoenixSolver phoenix = makePhoenixSolver(&(PhoenixBios){
            .name = "phoenix",
            .description = "Generic Phoenix",
            .pattern = ".*"
        });
        solvers[count++] = phoenix.base;

        PhoenixSolver phoenixHPCompaq = makePhoenixSolver(&(PhoenixBios){
            .name = "phoenixHP",
            .description = "HP/Compaq Phoenix BIOS",
            .salt = 17232,
            .pattern = ".*"
        });
        solvers[count++] = phoenixHPCompaq.base;

        PhoenixSolver phoenixFSI = makePhoenixSolver(&(PhoenixBios){
            .name = "phoenixFSI",
            .description = "Fujitsu-Siemens Phoenix",
            .salt = 65,
            .dictionary = digitsOnly,
            .pattern = ".*"
        });
        solvers[count++] = phoenixFSI.base;

        PhoenixSolver phoenixFSIModelL = makePhoenixSolver(&(PhoenixBios){
            .name = "phoenixFSIModelL",
            .description = "Fujitsu-Siemens (model L) Phoenix",
            .shift = 1,
            .salt = 'L',
            .dictionary = digitsOnly,
            .pattern = ".*"
        });
        solvers[count++] = phoenixFSIModelL.base;

        PhoenixSolver phoenixFSIModelP = makePhoenixSolver(&(PhoenixBios){
            .name = "phoenixFSIModelP",
            .description = "Fujitsu-Siemens (model P) Phoenix",
            .shift = 1,
            .salt = 'P',
            .dictionary = digitsOnly,
            .pattern = ".*"
        });
        solvers[count++] = phoenixFSIModelP.base;

        PhoenixSolver phoenixFSIModelS = makePhoenixSolver(&(PhoenixBios){
            .name = "phoenixFSIModelS",
            .description = "Fujitsu-Siemens (model S) Phoenix",
            .shift = 1,
            .salt = 'S',
            .dictionary = digitsOnly,
            .pattern = ".*"
        });
        solvers[count++] = phoenixFSIModelS.base;

        PhoenixSolver phoenixFSIModelX = makePhoenixSolver(&(PhoenixBios){
            .name = "phoenixFSIModelX",
            .description = "Fujitsu-Siemens (model S) Phoenix",
            .shift = 1,
            .salt = 'X',
            .dictionary = digitsOnly,
            .pattern = ".*"
        });
        solvers[count++] = phoenixFSIModelX.base;
    #endif

    #ifdef ENABLE_HPAMI
        solvers[count++] = makeSolver("hpami", "HP AMI", "^[0-9ABCDEF]{8}$", hpAmiKeygen);
    #endif

    solvers[count++] = (Solver){ NULL, NULL, NULL, NULL };

    if (argc != 3) {
        fprintf(stderr, "Usage: %s VENDOR SERIAL\n", argv[0]);
        printf("Keygens: \n");
        list_solvers(solvers);
        return 1;
    }

    const char *vendor = argv[1];
    const char *serial = argv[2];
    char *code = NULL;

    for (int i = 0; solvers[i].name; ++i) {
        if (strcmp(solvers[i].name, vendor) == 0) {
            code = runSolver(solvers[i], serial);
            break;
        }
    }

    if (!code) {
        fprintf(stderr, "Unknown vendor or failed to generate code.\n");
        return 1;
    }

    printf("Unlock code(s): %s\n", code);
    free(code);
    return 0;
}
