/** Todo: Optimization idea, allow functions that return a acnfl_genericFunctionResult to have user specify where they want data stored BE AWARE THIS COULD BE INTENSELY ERROR-PRONE */

#ifndef ACNFL_DERIVATIVE_H
#define ACNFL_DERIVATIVE_H

#include "acnfl_math.h"

/** Want to keep this consistent in case it needs to be changed in the future.*/
typedef long long int int_functionParameter ;

/**
 * Provides a container for a function returned by a function with type signature 
 * acnfl_GenericFunctionResult.
 * @param resultsAmount The length of the list pointed to by the results pointer.
 * @param results A pointer to a list of results.
 **/
typedef struct acnfl_genericFunctionResult {
    int_functionParameter resultsAmount;
    acnfl_NumberObject* results;
} acnfl_GenericFunctionResult;

/// Provides a generic colong long intntainer for mathmatical functions that need to be handled.
typedef acnfl_GenericFunctionResult (*acnfl_GenericFunctionDefinition)(
    int_functionParameter parameterCount, 
    acnfl_NumberObject *parameterList) ;

void acnfl_printFunctionResult(acnfl_GenericFunctionResult *indexPointer);

void acnfl_printListOfFunctionResults(acnfl_GenericFunctionResult *list) ;

void acnfl_freeHeldResults(acnfl_GenericFunctionResult *results);
void acnfl_freeListOfFunctionResults(acnfl_GenericFunctionResult *list, int_functionParameter length);
acnfl_GenericFunctionResult acnfl_derivative(acnfl_NumberObject *locationToDifferentiate, int_functionParameter numberOfLocations, int_functionParameter indexToDerive, int_functionParameter numberOfTests, acnfl_GenericFunctionDefinition functionPointer, acnfl_NumberObject delta);

acnfl_GenericFunctionResult acnfl_derivative_default(acnfl_NumberObject *locationToDifferentiate, int_functionParameter numberOfLocations, int_functionParameter indexToDerive,  acnfl_GenericFunctionDefinition functionPointer) ;
void print_derivative(acnfl_NumberObject *locationA, int_functionParameter locationLength, acnfl_GenericFunctionResult derivativeA) ;



#endif