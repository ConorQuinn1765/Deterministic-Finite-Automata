#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

const int VECTOR_MAX_STRING = 1024;

typedef struct vector
{
	int size;
	int capacity;
	char** arr;

}Vector;

bool vectorExpand(Vector* pVector);
bool vectorShrink(Vector* pVector);
bool vectorShift(Vector* pVector, int index);

VECTOR vectorInit(void)
{
	Vector* pVector = malloc(sizeof(Vector));

	if(pVector)
	{
		pVector->size = 0;
		pVector->capacity = 5;
		pVector->arr = malloc(sizeof(char*) * pVector->capacity);
		if(!pVector->arr)
		{
			fprintf(stderr, "vectorInit - Failed to allocate space for vector array\n");
			free(pVector);
			return NULL;
		}
		
		for(int i = 0; i < pVector->capacity; i++)
			pVector->arr[i] = NULL;
	}
	return (VECTOR)pVector;
}

bool vectorInsert(VECTOR hVector, char* data)
{
	Vector* pVector = (Vector*)hVector;
	if(!pVector)
	{
		fprintf(stderr, "vectorInsert - Invalid hVector passed to vectorInsert\n");
		return false;
	}
	
	if(pVector->size >= pVector->capacity)
		vectorExpand(pVector);
	
	pVector->arr[pVector->size] = malloc(sizeof(char) * VECTOR_MAX_STRING);
	if(!pVector->arr[pVector->size])
	{
		fprintf(stderr, "vectorInsert - Failed to allocate space for string\n");
		return false;
	}
	
	memset(pVector->arr[pVector->size], 0, VECTOR_MAX_STRING);
	strncpy(pVector->arr[pVector->size], data, strlen(data));
	pVector->size++;
	return true;
}

bool vectorInsertPos(VECTOR hVector, char* data, int index)
{
	Vector* pVector = (Vector*)hVector;
	if(!pVector)
	{
		fprintf(stderr, "vectorInsertPos - Invalid hVector passed to vectorInsert\n");
		return false;
	}
	
	if(index < 0 || index >= pVector->capacity)
	{
		fprintf(stderr, "vectorInsertPos - Invalid index %d\n", index);
		return false;	
	}
	
	if(pVector->size >= pVector->capacity)
		vectorExpand(pVector);
	
	pVector->arr[index] = malloc(sizeof(char) * VECTOR_MAX_STRING);
	if(!pVector->arr[index])
	{
		fprintf(stderr, "vectorInsertPos - Failed to allocate space for string\n");
		return false;
	}
	
	strncpy(pVector->arr[index], data, strlen(data));
	pVector->size++;
	return true;
}

bool vectorRemove(VECTOR hVector, char* data)
{
	Vector* pVector = (Vector*)hVector;
	
	if(pVector)
	{
		for(int i = 0; i < pVector->size; i++)
		{
			if(strncmp(pVector->arr[i], data, strlen(data)) == 0)
			{
				free(pVector->arr[i]);
				
				vectorShift(pVector, i);
				if(pVector->size < pVector->capacity / 2)
					vectorShrink(pVector);

				return true;
			}
		}
	}
	return false;
}

char* vectorAt(VECTOR hVector, int index)
{
	Vector* pVector = (Vector*)hVector;
	if(pVector)
		if(index >= 0 && index < pVector->size)
			return pVector->arr[index];

	return NULL;
}

int  vectorGetSize(VECTOR hVector)
{
	Vector* pVector = (Vector*)hVector;
	if(pVector)
		return pVector->size;

	return -1;
}

int  vectorGetCapacity(VECTOR hVector)
{
	Vector* pVector = (Vector*)hVector;
	if(pVector)
		return pVector->capacity;

	return -1;
}

void vectorPrint(VECTOR hVector)
{
	Vector* pVector = (Vector*)hVector;

	if(pVector)
	{
		for(int i = 0; i < pVector->size; i++)
			printf("%s\n", pVector->arr[i]);
	}
}

void vectorDestroy(VECTOR* phVector)
{
	Vector* pVector = (Vector*)*phVector;
	if(pVector)
	{
		for(int i = 0; i < pVector->capacity; i++)
			if(pVector->arr[i])
				free(pVector->arr[i]);

		free(pVector->arr);
		free(pVector);
	}
	*phVector = NULL;
}

/****************************************************************************************************/
/**
 * @brief Expand the vector capacity by a factor of 2
 * 
 * @param pVector a pointer to the vector object
 */
bool vectorExpand(Vector* pVector)
{
	char** temp;

	if(!pVector)
		return false;
	
	temp = realloc(pVector->arr, sizeof(char*) * pVector->capacity * 2);
	if(!temp)
	{
		fprintf(stderr, "Failed to increase vector size\n");
		return false;
	}
	
	for(int i = pVector->size; i < pVector->capacity * 2; i++)
		temp[i] = NULL;
	
	pVector->arr = temp;
	pVector->capacity *= 2;
	
	return true;
}

/**
 * @brief Shrink the capacity of the vector by a factor of 2
 * 
 * @param pVector a pointer to the vector object
 */
bool vectorShrink(Vector* pVector)
{
	char** temp;
	if(pVector && pVector->size <= pVector->capacity / 2 && pVector->size > 1)
	{
		for(int i = pVector->capacity / 2; i < pVector->capacity; i++)
			free(pVector->arr[i]);
		
		temp = realloc(pVector->arr, sizeof(char*) * (pVector->capacity / 2));
		if(!temp)
		{
			fprintf(stderr, "Failed to shrink the vector\n");
			
			for(int i = pVector->capacity / 2; i < pVector->capacity; i++)
				pVector->arr[i] = NULL;
			
			return false;
		}
		
		pVector->arr = temp;
		pVector->capacity /= 2;
		
		return true;
	}
	
	return false;
}

/**
 * @brief Remove item from given index and shift all items after it to fill in the gap
 * 
 * @param pVector pointer to the vector object
 * @param index index to remove the item from
 */
bool vectorShift(Vector* pVector, int index)
{
	if(pVector && index > 0 && index < pVector->size)
	{	
		for(; index < pVector->size - 1; index++)
			pVector->arr[index] = pVector->arr[index + 1];

		pVector->arr[pVector->size - 1] = NULL;
		pVector->size--;
		
		return true;
	}
	
	return false;
}