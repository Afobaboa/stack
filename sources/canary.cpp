#include <stdlib.h>
#include <string.h>

#include "../headers/canary.h"


//----------------------------------------------------------------------------------------


static const canary_t CANARY = (uint64_t) 0xFEEDFACECAFEBEEF;


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


bool CanaryDelete(canary_t* canaryPtr)
{
    if (canaryPtr == NULL)
        return false;

    memset(canaryPtr, 0, sizeof(canary_t));
    return true;
}
