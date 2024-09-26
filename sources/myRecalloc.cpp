#include <stdlib.h>
#include <string.h>

#include "../headers/myRecalloc.h"


//----------------------------------------------------------------------------------------


void* MyRecalloc(void* oldData, const size_t oldDataSize,
                 const size_t newDataSize, const size_t elemSize)
{
    void* newData = calloc(newDataSize, elemSize);
    if (newData == NULL)
        return NULL;

    if (newDataSize < oldDataSize)
    {
        memmove(newData, oldData, newDataSize);
    }
    else 
    {
        memmove(newData, oldData, oldDataSize);
        memset((char*) newData + oldDataSize, 0, newDataSize - oldDataSize);
    }

    free(oldData);
    return newData;
}


//----------------------------------------------------------------------------------------