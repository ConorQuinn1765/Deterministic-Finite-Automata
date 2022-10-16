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
    bool verbose;
}Dfa;

/*****************************************
 *     Helper Function Declarations     *
*****************************************/
Dfa* readFile(Dfa* pDfa, char* filename);
VECTOR tokenizeString(char* string, VECTOR alphabet);
int indexSubStr(char* haystack, char* needle, int startIndex);
bool checkInvalidStrings(VECTOR alphabet, char* str);

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
        
        dfa->verbose = verbose;
    }
    
    dfa = readFile(dfa, filename);
   
    if(verbose)
    {
        dfaPrint(dfa);
        printf("\n");
    }
    
    return (DFA)dfa;
}

bool dfaAccepts(DFA hDfa, char* string)
{
    Dfa* pDfa = (Dfa*)hDfa;
    if(pDfa)
    {
        if(!checkInvalidStrings(pDfa->alphabet, string))
        {
            printf("%s contains invalid characters --> Not Accepts\n", string);
            return false;
        }
        
        char currState[DFA_MAX_STRING];
        memset(currState, 0, DFA_MAX_STRING);
        strncpy(currState, pDfa->startState, strlen(pDfa->startState));
        
        VECTOR tokens = tokenizeString(string, pDfa->alphabet);
        
        for(int i = 0; i < vectorGetSize(tokens); i++)
        {
            MAP hMap = mmapFind(pDfa->transitions, currState);
            char* newState = mapFind(hMap, vectorAt(tokens, i));
            
            if(pDfa->verbose)
                printf("Curr State: %s\tSymbol: %s\tNew State: %s\n", currState, vectorAt(tokens, i), newState);
            
            memset(currState, 0, strlen(currState));
            strncpy(currState, newState, strlen(newState));
        }
        
        vectorDestroy(&tokens);
        
        for(int i = 0; i < vectorGetSize(pDfa->finalStates); i++)
        {
            if(strncmp(currState, vectorAt(pDfa->finalStates, i), strlen(currState)) == 0)
            {
                printf("%s --> Accepts\n", string);
                if(pDfa->verbose)
                    printf("\n");
                return true;
            }
        }
    }
    
    printf("%s --> Not Accepts\n", string);
    if(pDfa->verbose)
        printf("\n");
    return false;
}

void dfaPrint(DFA hDfa)
{
    Dfa* pDfa = (Dfa*)hDfa;
    if(pDfa)
    {
        printf("================ BEGIN DFA DEFINITION ================\n");
        printf("States: ");
        vectorPrint(pDfa->states);
        
        printf("\n\nAlphabet: ");
        vectorPrint(pDfa->alphabet);
        
        printf("\n\nStart State: %s\n", pDfa->startState);
        
        printf("\nFinal States: ");
        vectorPrint(pDfa->finalStates);
        
        printf("\n\nTransitions:\n");
        mmapInOrderTraversal(pDfa->transitions);
        printf("================= END DFA DEFINITION =================\n");
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

VECTOR tokenizeString(char* string, VECTOR alphabet)
{
    VECTOR hVector = vectorInit();
    if(!hVector)
        return NULL;
        
    char* tokens[DFA_MAX_STRING];
    memset(tokens, 0, sizeof(char*) * DFA_MAX_STRING);
        
    for(int i = 0; i < vectorGetSize(alphabet); i++)
    {
        int index = indexSubStr(string, vectorAt(alphabet, i), 0);
        
        while(index >= 0)
        {
            tokens[index] = vectorAt(alphabet, i);
            index = indexSubStr(string, vectorAt(alphabet, i), index + 1);
        }
    }
    
    for(int i = 0; i < DFA_MAX_STRING; i++)
    {
        if(tokens[i] != NULL)
        {
            vectorInsert(hVector, tokens[i]);
        }
    }
    
    return hVector;
}

int indexSubStr(char* haystack, char* needle, int startIndex)
{
    for(int i = startIndex; i < strlen(haystack); i++)
    {   
        for(int j = 0; j < strlen(needle); j++)
        {
            if(needle[j] != haystack[i+j])
                break;
            if(j + 1 == strlen(needle))
                return i;
        }
    }
    
    return -1;
}

bool checkInvalidStrings(VECTOR alphabet, char* str)
{
    if(!alphabet)
    {
        fprintf(stderr, "checkInvalidStrings - Invalid alphabet vector\n");
        return false;
    }
    
    char string[DFA_MAX_STRING];
    memset(string, 0, DFA_MAX_STRING);
    
    strncpy(string, str, DFA_MAX_STRING);
    
    for(int i = 0; i < vectorGetSize(alphabet); i++)
    {
        int index = indexSubStr(string, vectorAt(alphabet, i), 0);
        
        while(index >= 0)
        {
            for(int j = index + strlen(vectorAt(alphabet, i)); j < strlen(string); j++)
                string[j - strlen(vectorAt(alphabet, i))] = string[j];
                
            for(int j = strlen(string) - strlen(vectorAt(alphabet, i)); j < strlen(string); j++)
                string[j] = '\0';
            
            index = indexSubStr(string, vectorAt(alphabet, i), index);
        }
    }

    if(strncmp(string, "", strlen(string)) == 0)
        return true;
        
    return false;
}
