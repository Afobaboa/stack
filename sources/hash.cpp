#include <immintrin.h>
#include <string.h>

#include "../headers/hash.h"


//----------------------------------------------------------------------------------------


/**
 * 
 */
static hash32_t CycleRightShift(hash32_t hash, const size_t shift);


//----------------------------------------------------------------------------------------


void MURMUR32_Hash(hash32_t* hashBuffer,
                hash32_t* dataPtr, const size_t dataByteCount)
{
    const hash32_t hashSeed = 0xDA52DA52;

    const hash32_t c1 = 0xCC9E2D51;
    const hash32_t c2 = 0x1B873593;
    const hash32_t r1 = 15;
    const hash32_t r2 = 13;
    const hash32_t m  = 5;
    const hash32_t n  = 0xE6546B64;

    hash32_t hash = hashSeed;

    for (size_t byteChankNum = 0; byteChankNum < dataByteCount / sizeof(hash32_t); 
                                                                        byteChankNum++)
    {
        hash32_t byteChank = dataPtr[byteChankNum];

        byteChank = byteChank * c1;
        byteChank = CycleRightShift(byteChank, r1);
        byteChank = byteChank * c2;

        hash = hash ^ byteChank;
        hash = CycleRightShift(hash, r2);
        hash = (hash * m) + n;
    }

    size_t hashedByteCount = sizeof(hash32_t) * (dataByteCount / sizeof(hash32_t));
    hash32_t remainigBytesChank = 0;
    memmove(&remainigBytesChank, 
            (char*) dataPtr + hashedByteCount, 
            dataByteCount - hashedByteCount);

    remainigBytesChank = remainigBytesChank * c1;
    remainigBytesChank = CycleRightShift(remainigBytesChank, r1);
    remainigBytesChank = remainigBytesChank * c2;

    hash = hash ^ remainigBytesChank;

    hash = hash ^ dataByteCount;
    hash = hash ^ (hash >> 16);
    hash = hash * 0x85EBCA6B;
    hash = hash ^ (hash >> 13);
    hash = hash * 0xc2b2ae35;
    hash = hash ^ (hash >> 16);

    *hashBuffer = hash;
}


void CRC32_Hash(hash32_t* hashBuffer,
                hash32_t* dataPtr, const size_t dataByteCount)
{
    hash32_t crc = 0xFFFFFFFF;

    for (size_t byteNum = 0; byteNum < dataByteCount ; byteNum++)
    {
        crc = _mm_crc32_u8(crc, *((unsigned char*) dataPtr + byteNum));
    }

    *hashBuffer = ~crc; 
}


//----------------------------------------------------------------------------------------


static hash32_t CycleRightShift(hash32_t hash, const size_t shift)
{
    return (hash >> shift) | (hash << (sizeof(hash)*8 - shift));
}
