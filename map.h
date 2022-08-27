#ifndef MAP_H
#define MAP_H
#include <stdlib.h>
#include <stdbool.h>

typedef void* MAP;

bool mapInsert(MAP* hNode, char* key, char* value);
bool mapRemove(MAP* hNode, char* key);
char* mapFind(MAP hNode, char* key);
void mapDestroy(MAP* phNode);

//      Output Functions        //
void preOrderTraversal(MAP hNode);
void inOrderTraversal(MAP hNode);
void postOrderTraversal(MAP hNode);


#endif
