#include <stdlib.h>

#include "../headers/canary.h"


//----------------------------------------------------------------------------------------


static const canary_t CANARY = 0xFEEDFACECAFEBEEF;


//----------------------------------------------------------------------------------------


bool CanarySet(canary_t* canaryPtr)
{
    if (canaryPtr == NULL)
        return false;

    *canaryPtr = CANARY;
    return true;
}


bool CanaryCheck(canary_t* canaryPtr)
{
    if (canaryPtr == NULL)
        return false;

    if (*canaryPtr != CANARY)
        return false;
    
    return true;
}