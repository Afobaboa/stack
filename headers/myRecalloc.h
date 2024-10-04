#ifndef MY_RECALLOC_H
#define MY_RECALLOC_H


//----------------------------------------------------------------------------------------


#include <stddef.h>


//----------------------------------------------------------------------------------------


/**
 * This is analogue of realloc() with calloc() args style.
 * New data buffer has new address, this function can't use old address.
 * This function free oldData. Other features is like in realloc().
 * 
 * @param oldData         Pointer to old buffer.
 * @param oldDataCapacity Capacity of old buffer.
 * @param newDataCapacity Capacity of new buffer.
 * @param elemSize        Size of each elem in bytes.
 * 
 * @return Ptr to new data buffer or NULL if reallocation failed or if new data size is 0.
 */
void* MyRecalloc(void* oldData, const size_t oldDataCapacity,
                 const size_t newDataCapacity, const size_t elemSize);


//----------------------------------------------------------------------------------------


#endif // MY_RECALLOC_H