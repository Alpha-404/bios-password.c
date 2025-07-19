#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include "solver.h"

Solver makeSolver(const char *name, const char *description, const char *pattern, char *(*keygen)(const char *)) {
    Solver s;
    s.name = name;
    s.description = description;
    s.pattern = pattern;
    s.keygen = keygen;
    return s;
}

char *runSolver(Solver solver, const char *serial) {
    regex_t regex;

    if (regcomp(&regex, solver.pattern, REG_EXTENDED | REG_ICASE) != 0) {
        fprintf(stderr, "Regex compilation failed\n");
        return NULL;
    }

    int valid = regexec(&regex, serial, 0, NULL, 0);
    regfree(&regex);

    if (valid == 0) {
        return solver.keygen(serial);
    } else {
        fprintf(stderr, "Invalid serial for %s: %s\n", solver.name, serial);
        return NULL;
    }
}