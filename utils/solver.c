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
    int ret;
    
    ret = regcomp(&regex, solver.pattern, REG_EXTENDED | REG_ICASE);
    if (ret != 0) {
        char errbuf[128];
        regerror(ret, &regex, errbuf, sizeof(errbuf));
        fprintf(stderr, "Regex compilation failed: %s\n", errbuf);
        return NULL;
    }

    ret = regexec(&regex, serial, 0, NULL, 0);
    regfree(&regex);

    if (ret == 0) {
        return solver.keygen(serial);
    } else if (ret == REG_NOMATCH) {
        fprintf(stderr, "Invalid serial for %s: '%s'\n", solver.name, serial);
        return NULL;
    } else {
        char errbuf[128];
        regerror(ret, &regex, errbuf, sizeof(errbuf));
        fprintf(stderr, "Regex match error: %s\n", errbuf);
        return NULL;
    }
}
