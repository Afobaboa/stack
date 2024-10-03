#include <string.h>

#include "../headers/hash.h"


//----------------------------------------------------------------------------------------


/**
 * 
 */
static hashData_t CycleRightShift(hashData_t hashData, const size_t shift);


//----------------------------------------------------------------------------------------


void MURMUR32_Hash(hashData_t* hashBuffer,
                hashData_t* dataPtr, const size_t dataByteCount)
{
    const hashData_t hashSeed = 0xDA52DA52;

    const hashData_t c1 = 0xCC9E2D51;
    const hashData_t c2 = 0x1B873593;
    const hashData_t r1 = 15;
    const hashData_t r2 = 13;
    const hashData_t m  = 5;
    const hashData_t n  = 0xE6546B64;

    hashData_t hash = hashSeed;

    for (size_t byteChankNum = 0; byteChankNum < dataByteCount / sizeof(hashData_t); 
                                                                        byteChankNum++)
    {
        hashData_t byteChank = dataPtr[byteChankNum];

        byteChank = byteChank * c1;
        byteChank = CycleRightShift(byteChank, r1);
        byteChank = byteChank * c2;

        hash = hash ^ byteChank;
        hash = CycleRightShift(hash, r2);
        hash = (hash * m) + n;
    }

    size_t hashedByteCount = sizeof(hashData_t) * (dataByteCount / sizeof(hashData_t));
    hashData_t remainigBytesChank = 0;
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


void CRC32_Hash(hashData_t* hashBuffer,
                hashData_t* dataPtr, const size_t dataByteCount)
{
    unsigned int crc = 0; // Начальное значение CRC

    asm (
        "xor %%eax, %%eax;"      // Обнулить EAX (начальное значение CRC)
        "test %length, %length;" // Проверить, равна ли длина нулю
        "jz done;"               // Если длина 0, перейти к завершению
        "1:;"
        "crc32 %%eax, byte [%[data]];" // Вычислить CRC32 для текущего байта
        "inc %[data];"           // Перейти к следующему байту
        "dec %length;"         // Уменьшить счетчик длины
        "jnz 1b;"                // Повторять, пока не достигнута длина 0
        "done:;"
        : "=a"(crc)              // Выход: CRC в EAX
        : "[data]"(dataPtr), "length"(dataByteCount) // Вход: указатель на данные и длина
        : "%eax"
    );

    *hashBuffer = crc; // Вернуть результат CRC
}


//----------------------------------------------------------------------------------------


static hashData_t CycleRightShift(hashData_t hashData, const size_t shift)
{
    return (hashData >> shift) | (hashData << (sizeof(hashData)*8 - shift));
}
