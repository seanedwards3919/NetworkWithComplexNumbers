/** @file: acnfl_derivative.h*/
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

/// Provides a generic container for mathmatical functions that need to be handled.
typedef acnfl_GenericFunctionResult (*acnfl_GenericFunctionDefinition)(
    int_functionParameter parameterCount, 
    acnfl_NumberObject *parameterList) ;

/***
 * Provides a way to store information about an acnfl_GenericFunctionDefinition.
 * @param expected_inputs_minimum The expected number of inputs to the function. Should be set to -1 if any number of inputs are acceptable.
 * @param expected_inputs_maximum The expected number of inputs to the function. Should be set to -1 if any number of inputs are acceptable
 * @param expected_inputs_difference Whether the minimum and maximum number of expected inputs differ.
 * @param expected_outputs_minimum The expected number of inputs to the function. Should be set to -1 if any number of inputs are possible.
 * @param expected_outputs_maximum The expected number of inputs to the function. Should be set to -1 if any number of outputs are possible.
 * @param expected_outputs_difference Whether the minimum and maximum number of expected outputs differ.
 * @param functionPointer Pointer to function.

 */
typedef struct acnfl_genericFunctionContainer {
    int_functionParameter expected_inputs_minimum;
    int_functionParameter expected_inputs_maximum;
    bool expected_inputs_difference;

    int_functionParameter expected_outputs_minimum;
    int_functionParameter expected_outputs_maximum;
    bool expected_outputs_difference; 

    acnfl_GenericFunctionDefinition functionPointer;
} acnfl_GenericFunctionContainer;

void acnfl_printFunctionResult(acnfl_GenericFunctionResult *indexPointer);

void acnfl_printListOfFunctionResults(acnfl_GenericFunctionResult *list) ;

void acnfl_freeHeldResults(acnfl_GenericFunctionResult *results);
void acnfl_freeListOfFunctionResults(acnfl_GenericFunctionResult *list, int_functionParameter length);
acnfl_GenericFunctionResult acnfl_derivative(acnfl_NumberObject *locationToDifferentiate, int_functionParameter numberOfLocations, int_functionParameter indexToDerive, int_functionParameter numberOfTests, acnfl_GenericFunctionDefinition functionPointer, acnfl_NumberObject delta);

acnfl_GenericFunctionResult acnfl_derivative_default(acnfl_NumberObject *locationToDifferentiate, int_functionParameter numberOfLocations, int_functionParameter indexToDerive,  acnfl_GenericFunctionDefinition functionPointer) ;
void print_derivative(acnfl_NumberObject *locationA, int_functionParameter locationLength, acnfl_GenericFunctionResult derivativeA) ;



#endif