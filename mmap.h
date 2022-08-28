#ifndef MMAP_H
#define MMAP_H
#include <stdbool.h>
#include "map.h"

typedef void* MMAP;

bool mmapInsert(MMAP* hNode, char* key);
bool mmapSetValue(MMAP* hNode, char* key, MAP hMap);
bool mmapRemove(MMAP* hNode, char* key);
MAP mmapFind(MMAP hNode, char* key);
void mmapDestroy(MMAP* phNode);

//      Output Functions        //
void mmapPreOrderTraversal(MMAP hNode);
void mmapInOrderTraversal(MMAP hNode);
void mmapPostOrderTraversal(MMAP hNode);

#endif