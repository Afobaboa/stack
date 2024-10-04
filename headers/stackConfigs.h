#ifndef STACK_CONFIGS_H
#define STACK_CONFIGS_H


//----------------------------------------------------------------------------------------


/**
 * 
 */
// #define HASH_SWITCH_OFF

#ifndef HASH_SWITCH_OFF

/**
 * 
 */
#define CRC32

#ifndef CRC32
/**
 * 
 */
#define MURMUR32
#endif // CRC32

#endif // HASH_SWITCH_OFF


/**
 * 
 */
// #define CANARY_SWITCH_OFF


/**
 * 
 */
// #define DEBUG_SWITCH_OFF


#ifndef HASH_SWITCH_OFF
    #define HASH(...) __VA_ARGS__
#else 
    #define HASH(...) 
#endif


/**
 * 
 */
#ifndef CANARY_SWITCH_OFF
    #define CANARY(...) __VA_ARGS__
#else
    #define CANARY(...) 
#endif


/**
 * 
 */
#ifndef DEBUG_SWITCH_OFF
    #define ON_DEBUG(...) __VA_ARGS__
#else
    #define ON_DEBUG(...) 
#endif


//----------------------------------------------------------------------------------------


#endif // STACK_CONFIGS_H