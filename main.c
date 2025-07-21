#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "keygens/sony.h"
#include "keygens/hpmini.h"
#include "keygens/phoenix.h"
#include "keygens/hpami.h"
#include "keygens/insyde.h"
#include "keygens/asus.h"

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

int main(int argc, char *argv[]) {
    Solver solvers[MAX_SOLVERS];
    int count = 0;
    
    #ifdef ENABLE_SONY
        solvers[count++] = makeSolver("sony", "Sony 7-digit numeric keygen", "1234567", "^[0-9]{7}$", sonyKeygen);
    #endif

    #ifdef ENABLE_HPMINI
        solvers[count++] = makeSolver("hpmini", "HP Mini 10-character alphanumeric keygen", "CNU1234ABC", "^[0-9A-Z]{10}$", hpminiKeygen);
    #endif

    #ifdef ENABLE_PHOENIX
        PhoenixSolver phoenix = makePhoenixSolver(&(PhoenixBios){
            .name = "phoenix",
            .description = "Generic Phoenix",
            .pattern = "^[0-9]{5}$",
            .example = "12345"
        });
        solvers[count++] = phoenix.base;

        PhoenixSolver phoenixHPCompaq = makePhoenixSolver(&(PhoenixBios){
            .name = "phoenixHP",
            .description = "HP/Compaq Phoenix BIOS",
            .salt = 17232,
            .pattern = "^[0-9]{5}$",
            .example = "12345"
        });
        solvers[count++] = phoenixHPCompaq.base;

        PhoenixSolver phoenixFSI = makePhoenixSolver(&(PhoenixBios){
            .name = "phoenixFSI",
            .description = "Fujitsu-Siemens Phoenix",
            .salt = 65,
            .dictionary = digitsOnly,
            .pattern = "^[0-9]{5}$",
            .example = "12345"
        });
        solvers[count++] = phoenixFSI.base;

        PhoenixSolver phoenixFSIModelL = makePhoenixSolver(&(PhoenixBios){
            .name = "phoenixFSIModelL",
            .description = "Fujitsu-Siemens (model L) Phoenix",
            .shift = 1,
            .salt = 'L',
            .dictionary = digitsOnly,
            .pattern = "^[0-9]{5}$",
            .example = "12345"
        });
        solvers[count++] = phoenixFSIModelL.base;

        PhoenixSolver phoenixFSIModelP = makePhoenixSolver(&(PhoenixBios){
            .name = "phoenixFSIModelP",
            .description = "Fujitsu-Siemens (model P) Phoenix",
            .shift = 1,
            .salt = 'P',
            .dictionary = digitsOnly,
            .pattern = "^[0-9]{5}$",
            .example = "12345"
        });
        solvers[count++] = phoenixFSIModelP.base;

        PhoenixSolver phoenixFSIModelS = makePhoenixSolver(&(PhoenixBios){
            .name = "phoenixFSIModelS",
            .description = "Fujitsu-Siemens (model S) Phoenix",
            .shift = 1,
            .salt = 'S',
            .dictionary = digitsOnly,
            .pattern = "^[0-9]{5}$",
            .example = "12345"
        });
        solvers[count++] = phoenixFSIModelS.base;

        PhoenixSolver phoenixFSIModelX = makePhoenixSolver(&(PhoenixBios){
            .name = "phoenixFSIModelX",
            .description = "Fujitsu-Siemens (model S) Phoenix",
            .shift = 1,
            .salt = 'X',
            .dictionary = digitsOnly,
            .pattern = "^[0-9]{5}$",
            .example = "12345"
        });
        solvers[count++] = phoenixFSIModelX.base;
    #endif

    #ifdef ENABLE_HPAMI
        solvers[count++] = makeSolver("hpami", "HP AMI", "CNU1234ABX", "^[0-9ABCDEF]{8}$", hpAmiKeygen);
    #endif

    #ifdef ENABLE_INSYDE
        solvers[count++] = makeSolver("acerInsydeKeygen", "Some Acer, HP  laptops. 8 digits", "0173549286", "^[0-9]{10}$", acerInsydeKeygen);
        solvers[count++] = makeSolver("insydeKeygen", "Insyde H20 (generic)", "03133610", "^[0-9]{8}$", insydeKeygen);
        solvers[count++] = makeSolver("hpInsydeKeygen", "HP Insyde H2O", "i 70412809", "[i \\d{8}]", hpInsydeKeygen);
    #endif

    #ifdef ENABLE_ASUS
        solvers[count++] = makeSolver("asus", "ASUS (alt+r in bios pw to use)", "2010-02-03", "^[0-9]{4}-[0-9]{2}-[0-9]{2}$", asusKeygen);
    #endif

    solvers[count++] = (Solver){ NULL, NULL, NULL, NULL };

    if (argc != 2) {
        fprintf(stderr, "Usage: %s SERIAL\n", argv[0]);
        printf("\nAvailible keygens:\n");
        printf("%-18s | %-42s | %-21s| %-12s\n", "Solver Name", "Description", "Example", "Regex");
        printf("-------------------+--------------------------------------------+----------------------+---------------\n");
        for (int i = 0; solvers[i].name; ++i) {
            printf("%-18s | %-42s | %-21s| %-12s\n", solvers[i].name, solvers[i].description, solvers[i].example, solvers[i].pattern);
        }
        return 1;
    }

    //const char *vendor = argv[1];
    const char *serial = argv[1];// argv[2];
    char *code = NULL;
    int success = 0;

    srand(time(NULL));

    printf("Try one of the following codes:\n");
    printf("%-18s | %-15s\n", "Solver Name", "Unlock Code(s)");
    printf("-------------------+---------------\n");
    for (int i = 0; solvers[i].name; ++i) {
        regex_t regex;
        int reti;
        reti = regcomp(&regex, solvers[i].pattern, REG_EXTENDED | REG_NOSUB);
        reti = regexec(&regex, serial, 0, NULL, 0);
        if (!reti) {
            code = runSolver(solvers[i], serial);
            if (code) {
                success = 1;
                //printf("%s : %s\n", solvers[i].name, code);
                printf("%-18s | %-15s\n", solvers[i].name, code);
                free(code);
            }
            //printf("MATCH ON %s\n", solvers[i].name);
        } else {
            //printf("NO MATCH ON %s ON PATTERN %s\n", solvers[i].name, solvers[i].pattern);
        }
        //break;
    }

    if (!success) {
        fprintf(stderr, "Unknown vendor or failed to generate code.\n");
        return 1;
    }

    //printf("Unlock code(s): %s\n", code);
    //free(code);
    return 0;
}
