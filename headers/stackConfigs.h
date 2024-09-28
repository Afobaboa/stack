#ifndef STACK_CONFIG_H
#define STACK_CONFIG_H


//----------------------------------------------------------------------------------------


/**
 * 
 */
// #define HASH_SWITCH_OFF


/**
 * 
 */
// #define CANARY_SWITCH_OFF


/**
 * 
 */
// #define DEBUG_SWITCH_OFF


#ifndef HASH_SWITCH_OFF
    #define HASH(code) code
#else 
    #define HASH(code) 
#endif


/**
 * 
 */
#ifndef CANARY_SWITCH_OFF
    #define CANARY(code) code
#else
    #define CANARY(code) 
#endif


/**
 * 
 */
#ifndef DEBUG_SWITCH_OFF
    #define ON_DEBUG(code) code
#else
    #define ON_DEBUG(code) 
#endif


//----------------------------------------------------------------------------------------


#endif // STACK_CONFIG_H