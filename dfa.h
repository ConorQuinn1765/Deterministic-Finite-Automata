#ifndef DFA_H
#define DFA_H
#include <stdbool.h>

typedef void* DFA;

DFA dfaInit(char* filename, bool verbose);
bool dfaAccepts(DFA hDfa, char* string);

#endif