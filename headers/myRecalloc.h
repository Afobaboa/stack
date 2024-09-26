#ifndef MY_RECALLOC_H
#define MY_RECALLOC_H


//----------------------------------------------------------------------------------------


#include <stddef.h>


//----------------------------------------------------------------------------------------


/**
 * 
 */
void* MyRecalloc(void* oldData, const size_t oldDataSize,
                 const size_t newElemCount, const size_t elemSize);


//----------------------------------------------------------------------------------------


#endif // MY_RECALLOC_H