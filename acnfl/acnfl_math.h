
#ifndef acnfl_MATH_H
#define acnfl_MATH_H

#include <stdbool.h>
#include <stdio.h>

/** The type with which the values are stored. Made modular like this so 
 */
typedef long double valueType_apx;

/**
 * Possible values for nType (indicates the type of number used):
 *
 * 'r' - Indicates that acnfl_NumberObject holds a real number. If this is the case, then 
 *       complexNumberValue can be assumed to be zero and irrelavant.
 *
 * 'c' - Indicates that acnfl_NumberObject holds a complex number. If this is the case, then 
 *       complexNumberValue can be assumed to be relevant.
 * 
 * 'e' - Indicates there is an error with the acnfl_NumberObject's number format.
 *
 * The only current valid values for vType (indicates format for number) is 'a' (approximate) or 'e' 
 * (indicates an error with the acnfl_NumberObject's value sorage).
 * 
 * TODO: Determine type priority if there is more than one vType in play.
 */
typedef struct acnfl_numberObject {
    char nType; 
    char vType;
    struct {
        valueType_apx realNumberValue_apx;
        valueType_apx complexNumberValue_apx; // If nType is 'r', this is empty.
    };
} acnfl_NumberObject;


#endif