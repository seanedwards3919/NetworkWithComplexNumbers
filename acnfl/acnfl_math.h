
/** @file acnfl_math.c
 * ACNFL stands for: Automatically Compatable Numbertypes and Functions Library. 
 * 
 * This is the set of functions and datatypes that allows imaginary and real numbers
 * to work in tandem during this program.
 * It is also designed to be potentiallye extensible in the future.
 */
#ifndef acnfl_MATH_H
#define acnfl_MATH_H

#include <stdbool.h>
#include <stdio.h>


/** The type with which the values are stored. Made modular like this so it may be changed in future.
 */
typedef long double acnfl_valueType_apx;

/**
 * The general number data type for acnfl
 * 
 * @param nType Gives the type of the number in a mathematical sense. 
 *
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
 * @param vType vType determines the 
 *  format used for storing numbers in the struct.
 * The only current valid values for vType (indicates format for number) is 'a' (approximate), 'e' 
 * (indicates an error with the acnfl_NumberObject's value sorage), or 'd' (deliberate dummy value with no information stored.).
 * 'a' is simply a long double.
 *
 * @param realNumberValue_apx Real value of the number if approx data type is used.
 * @param imaginaryNumberValue_apx imaginary value of the number if approximate data type is used.
 * 
 * TODO: Determine type priority if there is more than one vType in play.
 * The rule should be that if a hiher-precision vType is used in the same operation 
 * as a lower-precision vType, then everything ends up converted to the higher-precision vType
 */
typedef struct acnfl_numberObject {
    char nType; 
    char vType;
    struct {
        acnfl_valueType_apx realNumberValue_apx;
        acnfl_valueType_apx imaginaryNumberValue_apx; // If nType is 'r', this is empty.
    };
} acnfl_NumberObject;

/**
 * Information passed into acnfl_defaultComparison.
 * 
 * @param opType Determines the type of operation performed to determine equality.
 * 
 * if opType is 'd' or an undefined character,('default'), even for  complex 
 * numbers it will only compare the real components.
 *
 * If opType is 'i', only the imaginary value of each number will be compared.
 *
 * If opType is 'a', absolute value will be used. For each 
 * imaginary number, a + bi, we use the comparison value a^2 + b^2 
 * (which measures the distance from an "origin", 0 + 0i)  
 *
 * @param leeway_apx Tells defaultComparison whether or not to check for near-equality
 * when using .
 * 
 * @param epsilon_apx If leeway is true, this is the epsilon value that determines our 
 * leeway for near-equality when leeway_apx is true.
 *
 * TODO: Change default comparison operation so that it takes the imaginary 
 * part into account without wrecking operations for "normal" non-complex
 * numbers.
 */
typedef struct {
    char opType;
    bool leeway_apx;
    acnfl_valueType_apx epsilon_apx;
} acnfl_defaultComparisonInformation;



 acnfl_NumberObject acnfl_add(acnfl_NumberObject a, acnfl_NumberObject b);
acnfl_NumberObject acnfl_generateApx(long double real, long double imaginary);

acnfl_NumberObject ancfl_subtract(acnfl_NumberObject a, acnfl_NumberObject b);
acnfl_NumberObject acnfl_multiply(acnfl_NumberObject a, acnfl_NumberObject b);
acnfl_NumberObject acnfl_divide(acnfl_NumberObject a, acnfl_NumberObject b);
float acnfl_comparison(acnfl_NumberObject a, acnfl_NumberObject b, 
        float (*alternateFunction)(acnfl_NumberObject, acnfl_NumberObject, void*),
        void* extraData);
float acnfl_defaultComparison 	( 	acnfl_NumberObject  	,
		acnfl_NumberObject  	,
		acnfl_defaultComparisonInformation  	) 		;

void acnfl_printValue(acnfl_NumberObject number);

#ifdef REPORTING_3
    void acnfl_errorReporting(acnfl_NumberObject *a) ;
    //Testing?
#endif

#endif