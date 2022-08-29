#ifndef MAP_H
#define MAP_H
#include <stdbool.h>
#include "vector.h"

typedef void* MAP;

bool mapInsert(MAP* hNode, char* key, char* value);
bool mapRemove(MAP* hNode, char* key);
void mapKeys(MAP hNode, VECTOR keys);
void mapValues(MAP hNode, VECTOR values);
char* mapFind(MAP hNode, char* key);
void mapDestroy(MAP* phNode);

//      Output Functions        //
void mapPreOrderTraversal(MAP hNode);
void mapInOrderTraversal(MAP hNode);
void mapPostOrderTraversal(MAP hNode);


#endif
