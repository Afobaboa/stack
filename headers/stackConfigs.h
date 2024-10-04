/**
 * @file 
 * This is config file. 
 * Comment or uncomment defines to swith
 * on or swich of some feetures of program.
 */

#ifndef STACK_CONFIGS_H
#define STACK_CONFIGS_H


//----------------------------------------------------------------------------------------


/**
 * This definition switch off hash defence of stack's data.
 */
// #define HASH_SWITCH_OFF

#ifndef HASH_SWITCH_OFF

/**
 * This definition set CRC32 like hash alghorithm.
 * If you switch off this define, hash alghorithm 
 * will be murmur32.
 */
#define CRC32

#ifndef CRC32
/**
 * DONT'T COMMENT THIS DEFINITION
 * This is murmur32 hash alghorithm. 
 */
#define MURMUR32
#endif // CRC32

#endif // HASH_SWITCH_OFF


/**
 * This definition switch off canary defence of stack's data.
 */
// #define CANARY_SWITCH_OFF


/**
 * This definition switch off debug lines and speed up program. 
 * In release version it's alread switched off by default.
 */
// #define DEBUG_SWITCH_OFF


/**
 * You can use this defines for convinient way of
 * useing conditional compilation in the body of lines.
 */
#ifndef HASH_SWITCH_OFF
    #define HASH(...) __VA_ARGS__
#else 
    #define HASH(...) 
#endif


#ifndef CANARY_SWITCH_OFF
    #define CANARY(...) __VA_ARGS__
#else
    #define CANARY(...) 
#endif


#ifndef DEBUG_SWITCH_OFF
    #define ON_DEBUG(...) __VA_ARGS__
#else
    #define ON_DEBUG(...) 
#endif


//----------------------------------------------------------------------------------------


#endif // STACK_CONFIGS_H