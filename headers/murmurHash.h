#ifndef MURMUR_HASH_H
#define MURMUR_HASH_H


//----------------------------------------------------------------------------------------


#include <stddef.h>
#include <stdint.h>


//----------------------------------------------------------------------------------------


/**
 * 
 */
typedef uint32_t hashData_t;


//----------------------------------------------------------------------------------------


/**
 * 
 */
void MurmurHash(hashData_t* hashBuffer,
                hashData_t* dataPtr, const size_t dataByteCount);


/**
 * 
 */
bool CheckMurmurHash(hashData_t* hashBuffer, 
                     hashData_t* dataPtr, const size_t dataByteCount);


//----------------------------------------------------------------------------------------


#endif // MURMUR_HASH_H
