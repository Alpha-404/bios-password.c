// solver.h
#ifndef SOLVER_H
#define SOLVER_H

typedef struct {
    const char *name;
    const char *description;
    char *(*keygen)(const char *serial);
    const char *pattern;
} Solver;

Solver makeSolver(const char *name, const char *description, const char *pattern, char *(*keygen)(const char *));
char *runSolver(Solver solver, const char *serial);

#endif