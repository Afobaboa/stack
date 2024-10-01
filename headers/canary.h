#ifndef CANARY_H
#define CANARY_H


//----------------------------------------------------------------------------------------


/**
 * 
 */
typedef signed long long canary_t;


//----------------------------------------------------------------------------------------


/**
 * 
 */
bool CanarySet(canary_t* canaryPtr);


/**
 * 
 */
bool CanaryCheck(canary_t* canaryPtr);


/**
 * 
 */
void CanaryDelete(canary_t* canaryPtr);


//----------------------------------------------------------------------------------------


#endif // CANARY_H