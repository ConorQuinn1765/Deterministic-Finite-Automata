#ifndef VECTOR_H
#define VECTOR_H
#include <stdbool.h>

typedef void* VECTOR;

VECTOR vectorInit(void);
bool vectorInsert(VECTOR hVector, char* data);
bool vectorInsertPos(VECTOR hVector, char* data, int index);
bool vectorRemove(VECTOR hVector, char* data);
char* vectorAt(VECTOR hVector, int index);
int  vectorGetSize(VECTOR hVector);
int  vectorGetCapacity(VECTOR hVector);
void vectorPrint(VECTOR hVector);
void vectorDestroy(VECTOR* phVector);

#endif
