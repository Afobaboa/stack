#ifndef MURMUR_HASH_H
#define MURMUR_HASH_H


//----------------------------------------------------------------------------------------


#include <stddef.h>
#include <stdint.h>


//----------------------------------------------------------------------------------------


/**
 * 
 */
typedef uint32_t hash32_t;


//----------------------------------------------------------------------------------------


/**
 * 
 */
void MURMUR32_Hash(hash32_t* hashBuffer,
                   hash32_t* dataPtr, const size_t dataByteCount);


/**
 * 
 */
void CRC32_Hash(hash32_t* hashBuffer,
                hash32_t* dataPtr, const size_t dataByteCount);


//----------------------------------------------------------------------------------------


#endif // MURMUR_HASH_H
