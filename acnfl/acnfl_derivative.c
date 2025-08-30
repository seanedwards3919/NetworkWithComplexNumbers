/**
 * @file acnfl_derivative.c
 * Provides tools for the calcultion of numerical derivatives that integrate with acnfl_math.c.
 **/


/**TODO:
    Implement simple finite differnces first
    Implement interpolation through points
    Implement ways to differentiate on the interpolation
 */

#include "acnfl_math.h"
#include <stdlib.h>
#include <math.h>
#include "../reporting/reporting_3.h"

typedef struct acnfl_genericFunctionResult {
    long long int resultsAmount;
    acnfl_NumberObject* results;
} acnfl_GenericFunctionResult;

/**TODO: Is parameterCount needed? */
typedef acnfl_GenericFunctionResult (*acnfl_GenericFunctionDefinition)(
    long long int parameterCount, 
    acnfl_NumberObject *parameterList) ;

/**Prints a single function result. Assumes all information is accurate .*/
void acnfl_printFunctionResult(acnfl_GenericFunctionResult *indexPointer)
{
        for (int i = 0; i < indexPointer->resultsAmount; i++) {
            printf("Result %i: ", i);
            acnfl_printValue(indexPointer->results[i]);
            printf("\n");

        }


}
/***
* Prints a list of function results.
* Lists are expected to be terminated by a genericFunctionResults with resultsAmount == 0; 
*/
void acnfl_printListOfFunctionResults(acnfl_GenericFunctionResult *list) {
        acnfl_GenericFunctionResult *indexPointer = list;
    while (indexPointer->resultsAmount != 0) {
        printf("Equation result at %p:\n", (void*)indexPointer);
        acnfl_printFunctionResult(indexPointer);
        indexPointer++;
    }
    #ifdef REPORTING_1
    printf("Zero element founds at index %p, quitting\n", (void*)indexPointer);
    #endif
    return;

}

/** Frees the pointeres held by GenericFunctionResults in a list.*/
void acnfl_freeListOfFunctionResults(acnfl_GenericFunctionResult *list, int lengthOfList) {
        for (int i = 0; i < lengthOfList; i++ ) {
        if (list[i].results != NULL) {
            free(list[i].results);
        }
        list[i].results = NULL;
        list[i].resultsAmount = 0;
        
    }
    return;
}