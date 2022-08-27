#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dfa.h"
#include "map.h"
#include "vector.h"

typedef struct
{
    VECTOR states;          // Array of strings
    VECTOR alphabet;        // Array of strings
    MAP    transitions;     // Hash-table / Map of strings
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
        dfa->states = vectorInit();
        dfa->alphabet = vectorInit();
        dfa->transitions = mapInit();
        dfa->startState = NULL;
        dfa->finalStates = vectorInit();
    }
    
    return (DFA)readFile(dfa, filename);
}

bool dfaAccepts(DFA hDfa, char* string)
{
    return false;
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
            {
                break;
            }
        }
        else if(strncmp(type, "alphabet:", strlen(type)) == 0)
        {
            break;
        }
        else if(strncmp(type, "start-state:", strlen(type)) == 0)
        {
            break;
        }
        else if(strncmp(type, "final-states:", strlen(type)) == 0)
        {
            break;
        }
        else if(strncmp(type, "transition:", strlen(type)) == 0)
        {
            break;
        }
        else
        {
            break;
        }
    }
    
    fclose(fp);
    
    return pDfa;
}