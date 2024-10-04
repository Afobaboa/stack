/**
 * @file 
 * This header can be useful to defence your data by canaries.
 */

#ifndef CANARY_H
#define CANARY_H


//----------------------------------------------------------------------------------------


#include <stdint.h>


//----------------------------------------------------------------------------------------


/**
 * Type of canaries.
 */
typedef uint64_t canary_t;


//----------------------------------------------------------------------------------------


/**
 * This function set special value to canaryPtr data.
 * 
 * @param canaryPtr Ptr to canary.
 * 
 * @return false if setting failed,
 * @return true if it's OK.
 */
bool CanarySet(canary_t* canaryPtr);


/**
 * This function check if canary is alive.
 * Canary is alive if her value isn't changed.
 * 
 * @param canaryPtr Ptr to canary.
 * 
 * @return true if canary is alive.
 * @return false if canary died or canaryPtr is NULL.
 */
bool CanaryCheck(canary_t* canaryPtr);


/**
 * This function 
 */
bool CanaryDelete(canary_t* canaryPtr);


//----------------------------------------------------------------------------------------


#endif // CANARY_H