#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dfa.h"
#include "mmap.h"
#include "map.h"
#include "vector.h"

const int DFA_MAX_STRING = 1024;

typedef struct
{
    VECTOR states;          // Array of strings
    VECTOR alphabet;        // Array of strings
    MMAP   transitions;     // Map of Map of strings
    char*  startState;
    VECTOR finalStates;     // Array of strings
}Dfa;

/*****************************************
 *     Helper Function Declarations     *
*****************************************/
Dfa* readFile(Dfa* pDfa, char* filename);

/*****************************************
 *       DFA Function Definitions       *
*****************************************/
DFA dfaInit(char* filename, bool verbose)
{
    Dfa* dfa = malloc(sizeof(Dfa));
    
    if(dfa)
    {
        dfa->startState = malloc(sizeof(char) * DFA_MAX_STRING);
        if(!dfa->startState)
        {
            fprintf(stderr, "dfaInit - Failed to create room for start state\n");
            return NULL;
        }
        
        dfa->states = vectorInit();
        if(!dfa->states)
        {
            fprintf(stderr, "dfaInit - Failed to create states vector\n");
            free(dfa->startState);
            return NULL;
        }
        
        dfa->alphabet = vectorInit();
        if(!dfa->alphabet)
        {
            fprintf(stderr, "dfaInit - Failed to create alphabet vector\n");
            free(dfa->startState);
            vectorDestroy(&dfa->states);
            return NULL;
        }
        
        dfa->finalStates = vectorInit();
        if(!dfa->finalStates)
        {
            fprintf(stderr, "dfaInit - Failed to create states vector\n");
            free(dfa->startState);
            vectorDestroy(&dfa->states);
            vectorDestroy(&dfa->alphabet);
            return NULL;
        }
        
        dfa->transitions = NULL;
    }
    
    return (DFA)readFile(dfa, filename);
}

bool dfaAccepts(DFA hDfa, char* string)
{
    return false;
}

void dfaPrint(DFA hDfa)
{
    Dfa* pDfa = (Dfa*)hDfa;
    if(pDfa)
    {
        printf("States: ");
        vectorPrint(pDfa->states);
        
        printf("\n\nAlphabet: ");
        vectorPrint(pDfa->alphabet);
        
        printf("\n\nStart State: %s\n", pDfa->startState);
        
        printf("\nFinal States: ");
        vectorPrint(pDfa->finalStates);
        
        printf("\n\nTransitions:\n");
        mmapInOrderTraversal(pDfa->transitions);   
    }
}

void dfaDestroy(DFA* phDfa)
{
    if(phDfa)
    {
        Dfa* pDfa = (Dfa*)*phDfa;
        if(pDfa)
        {
            free(pDfa->startState);
            vectorDestroy(&pDfa->states);
            vectorDestroy(&pDfa->alphabet);
            vectorDestroy(&pDfa->finalStates);
            mmapDestroy(&pDfa->transitions);
            free(pDfa);
        }
        *phDfa = NULL;
    }
}

/*****************************************
 *      Helper Function Definitions     *
*****************************************/
Dfa* readFile(Dfa* pDfa, char* filename)
{
    FILE* fp = fopen(filename, "r");
    if(!fp)
    {
        fprintf(stderr, "readFile - Failed to open file \"%s\" for reading\n", filename);
        return pDfa;
    }
    
    char line[256];
    while(fgets(line, 256, fp))
    {
        char* type = strtok(line, " ");
        
        if(strncmp(type, "states:", strlen(type)) == 0)
        {            
            char* data;
            while((data = strtok(NULL, " \n")))
                vectorInsert(pDfa->states, data);
        }
        else if(strncmp(type, "alphabet:", strlen(type)) == 0)
        {
            char* data;
            while((data = strtok(NULL, " \n")))
                vectorInsert(pDfa->alphabet, data);
        }
        else if(strncmp(type, "start-state:", strlen(type)) == 0)
        {
            strncpy(pDfa->startState, strtok(NULL, " \n"), DFA_MAX_STRING);
        }
        else if(strncmp(type, "final-states:", strlen(type)) == 0)
        {
            char* data;
            while((data = strtok(NULL, " \n")))
                vectorInsert(pDfa->finalStates, data);
        }
        else if(strncmp(type, "transition:", strlen(type)) == 0)
        {
            char* data = strtok(NULL, " ");
            MAP hMap = mmapFind(pDfa->transitions, data);
            
            if(!hMap)
            {
                if(!mmapInsert(&pDfa->transitions, data))
                {
                    fprintf(stderr, "readFile - Failed to create MMAP for transitions\n");
                    continue;
                }
                
                hMap = mmapFind(pDfa->transitions, data);
            }
            
            char* key = strtok(NULL, " ");
            char* value = strtok(NULL, " \n");
            
            mapInsert(&hMap, key, value);
            
            mmapSetValue(&pDfa->transitions, data, hMap);
        }
        else
        {
            fprintf(stderr, "readFile - Invalid token %s. Ignoring line.\n", type);
        }
    }
    
    fclose(fp);
    
    return pDfa;
}