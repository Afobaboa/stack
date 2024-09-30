#include <stdlib.h>
#include <string.h>

#include "../headers/myRecalloc.h"


//----------------------------------------------------------------------------------------


void* MyRecalloc(void* oldData, const size_t oldDataСapacity,
                 const size_t newDataCapacity, const size_t elemSize)
{
    void* newData = calloc(newDataCapacity, elemSize);
    if (newData == NULL)
    {
        free(oldData);
        return NULL;
    }

    if (newDataCapacity < oldDataСapacity)
    {
        memmove(newData, oldData, newDataCapacity * elemSize);
    }
    else 
    {
        memmove(newData, oldData, oldDataСapacity * elemSize);
        memset((char*) newData + oldDataСapacity * elemSize, 0, 
                                        (newDataCapacity - oldDataСapacity) * elemSize);
    }

    free(oldData);
    return newData;
}


//----------------------------------------------------------------------------------------