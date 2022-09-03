#include <stdio.h>
#include <string.h>
#include "dfa.h"
#include "vector.h"

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        fprintf(stderr, "main - Program takes two command line inputs.\n"
                "1) The path to the file with the dfa description.\n"
                "2) The input file with strings to test against the dfa.\n");
        return 1;
    }
    
    DFA dfa = dfaInit(argv[1], true);
    
    FILE* fp = fopen(argv[2], "r");
    if(!fp)
    {
        fprintf(stderr, "main - Failed to open \"%s\" for reading\n", argv[2]);
        return 2;
    }
    
    char line[16384] = {0};
    while(fgets(line, 16384, fp))
    {
        char* c = strrchr(line, '\n');
        if(c)
            *c = '\0';
        
        dfaAccepts(dfa, line);
        memset(line, 0, 16384);
    }
    
    fclose(fp);
    
    dfaDestroy(&dfa);
    return 0;
}
