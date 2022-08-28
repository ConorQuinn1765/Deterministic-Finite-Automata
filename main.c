#include <stdio.h>
#include "dfa.h"

int main(void)
{
    DFA dfa = dfaInit("dfa-files/m1.dfa", false);
    
    dfaPrint(dfa);
    
    dfaDestroy(&dfa);
    return 0;
}
