

#ifndef ACNFL_DERIVATIVE_H
#define ACNFL_DERIVATIVE_H

#include "acnfl_math.h"

/**
 * Provides a container for a function returned by a function with type signature 
 * acnfl_GenericFunctionResult.
 * @param resultsAmount The length of the list pointed to by the results pointer.
 * @param results A pointer to a list of results.
 **/
typedef struct acnfl_genericFunctionResult {
    long long int resultsAmount;
    acnfl_NumberObject* results;
} acnfl_GenericFunctionResult;

/// Provides a generic container for mathmatical functions that need to be handled.
typedef acnfl_GenericFunctionResult (*acnfl_GenericFunctionDefinition)(
    long long int parameterCount, 
    acnfl_NumberObject *parameterList) ;

void acnfl_printFunctionResult(acnfl_GenericFunctionResult *indexPointer);

void acnfl_printListOfFunctionResults(acnfl_GenericFunctionResult *list) ;

void acnfl_freeHeldResults(acnfl_GenericFunctionResult *results);
void acnfl_freeListOfFunctionResults(acnfl_GenericFunctionResult *list, int length);
acnfl_GenericFunctionResult acnfl_derivative(acnfl_NumberObject *locationToDifferentiate, int numberOfLocations, int indexToDerive, int numberOfTests, acnfl_GenericFunctionDefinition functionPointer, acnfl_NumberObject delta);
acnfl_GenericFunctionResult acnfl_derivative_default(acnfl_NumberObject *locationToDifferentiate, int numberOfLocations, int indexToDerive, int numberOfTests, acnfl_GenericFunctionDefinition functionPointer) ;

void print_derivative(acnfl_NumberObject *locationA, int locationLength, acnfl_GenericFunctionResult derivativeA) ;



#endif