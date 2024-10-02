#include "../headers/murmurHash.h"


//----------------------------------------------------------------------------------------


/**
 * 
 */
static hashData_t CycleRightShift(hashData_t hashData, const size_t shift);


//----------------------------------------------------------------------------------------


void MurmurHash(hashData_t* hashBuffer,
                hashData_t* dataPtr, const size_t byteChanksCount)
{
    const hashData_t hashSeed = 0xDA52DA52;

    const hashData_t c1 = 0xCC9E2D51;
    const hashData_t c2 = 0x1B873593;
    const hashData_t r1 = 15;
    const hashData_t r2 = 13;
    const hashData_t m  = 5;
    const hashData_t n  = 0xE6546B64;

    hashData_t hash = hashSeed;

    for (size_t byteChankNum = 0; byteChankNum < byteChanksCount; byteChankNum++)
    {
        hashData_t byteChank = dataPtr[byteChankNum];

        byteChank = byteChank * c1;
        byteChank = CycleRightShift(byteChank, r1);
        byteChank = byteChank * c2;

        hash = hash ^ byteChank;
        hash = CycleRightShift(hash, r2);
        hash = (hash * m) + n;
    }

    hash = hash ^ byteChanksCount;
    hash = hash ^ (hash >> 16);
    hash = hash * 0x85EBCA6B;
    hash = hash ^ (hash >> 13);
    hash = hash * 0xc2b2ae35;
    hash = hash ^ (hash >> 16);

    *hashBuffer = hash;
}


bool CheckMurmurHash(hashData_t* hashBuffer, 
                     hashData_t* dataPtr, const size_t byteChanksCount)
{
    hashData_t hashCopy    = *hashBuffer;
    *hashBuffer = 0;
    hashData_t controlHash = 0;
    MurmurHash(&controlHash, dataPtr, byteChanksCount);
    *hashBuffer = hashCopy;

    if (hashCopy != controlHash)
        return false;
    
    return true;
}


bool GetByteChanksCount(const size_t byteCount, size_t* chanksCountBuffer)
{
    if (byteCount % sizeof(hashData_t) != 0)
        return false;

    *chanksCountBuffer = byteCount / sizeof(hashData_t);
    return true;
}


//----------------------------------------------------------------------------------------


static hashData_t CycleRightShift(hashData_t hashData, const size_t shift)
{
    return (hashData >> shift) | (hashData << (sizeof(hashData) - shift));
}
