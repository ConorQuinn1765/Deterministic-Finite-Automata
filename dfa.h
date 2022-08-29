#ifndef DFA_H
#define DFA_H
#include <stdbool.h>
#include "vector.h"

typedef void* DFA;

DFA dfaInit(char* filename, bool verbose);
bool dfaAccepts(DFA hDfa, char* string);
void dfaPrint(DFA hDfa);
void dfaDestroy(DFA* phDfa);

#endif