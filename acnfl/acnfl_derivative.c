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
///TODO: Add check if list is NULL
void acnfl_freeHeldResults(acnfl_GenericFunctionResult *results) {
    if (results->results != NULL /** TODO: and if ResultsAmount != 0 ... ?*/) {
        free(results->results);
    }
    results->results=NULL;
    results->resultsAmount =0; 

}

/** Frees the pointeres held by GenericFunctionResults in a list.*/
void acnfl_freeListOfFunctionResults(acnfl_GenericFunctionResult *list) { 
    int i = 0;
    while(list[i].resultsAmount != 0) {
    //for (int i = 0; i < lengthOfList; i++ ) {
         

        acnfl_freeHeldResults(list+i);
        
    }
    return;
}


/***
 * Returns the instantanious rate of change for a function defined by acnfl_GenericFunctionDefinition at location, with specificity determined by numberOfTests
 * @param location The location of the instantanious rate of change on the function.
 * @param numberOfTests The number of times acnfl_derivative gets closer to location when calculating the difference quotient. In general, a higher value means a more accurate result.
 *
 * TODO: Pathalogical behavior at high numberOfTests? Inserting 3*i for x_squared gives 3+3*i instead of 6*i
 **/
acnfl_GenericFunctionResult acnfl_derivative(acnfl_NumberObject location,
    int numberOfTests, acnfl_GenericFunctionDefinition functionPointer) {
 
    //Delta. Value to use in the difference quotient
    acnfl_NumberObject delta    = acnfl_generateApx(0.001, 0.001);
    acnfl_GenericFunctionResult results[numberOfTests+1],
                                resultsBank[(numberOfTests*2)+1];
    
    // Set up end buffer. Other functions use this to know when the list ends.
    resultsBank[(numberOfTests)] = (acnfl_GenericFunctionResult) {.resultsAmount =0};
    resultsBank[(numberOfTests*2)] = (acnfl_GenericFunctionResult) {.resultsAmount =0};

    // For as many times as numberOfTests
    for (int i = 0; i < numberOfTests; i++) {
        // Get first part of finite difference for test i 
        resultsBank[(i*2)] = functionPointer(1,  (acnfl_NumberObject[]) { acnfl_add(location, delta)});
        // Get second part of finite difference for test i
        resultsBank[(i*2)+1] = functionPointer(1,  (acnfl_NumberObject[]) { location});

        #ifdef REPORTING_1
            printf("\nDiff: \n");
            acnfl_printFunctionResult(resultsBank + (i*2));
            printf("...\nto ...\n");
            acnfl_printFunctionResult(resultsBank + (i*2) +1);
        #endif

        // Make sure both parts have the same number of returned parameters.
        if (resultsBank[i*2].resultsAmount != resultsBank[(i*2)+1].resultsAmount) printf("Different results gotten for two differnt locations. Quitting"),exit(1);

        // Create bank to hold differences between difference quotients. This should be the number of returned parameters 
        results[i].results = malloc(sizeof(acnfl_NumberObject)*resultsBank[i*2].resultsAmount);
        results[i].resultsAmount = resultsBank[i*2].resultsAmount;


        //Get full difference quotient.
        //Does the calculation for each of the returned paramteters.
        for (int locationIndex = 0; locationIndex < results[i].resultsAmount; locationIndex++) {
            results[i].results[locationIndex] = acnfl_divide(ancfl_subtract( resultsBank[i*2].results[locationIndex], resultsBank[(i*2)+1].results[locationIndex] ), delta);

        }

        //Decrease delta and have it be closer to location 
        delta = acnfl_divide(delta, acnfl_generateApx(2, 0));
    } 

    // Check that all of the amounts for the resultsAmount are the same 
    {
        if (numberOfTests > 1)  {
            for (int i = 1; i < numberOfTests; i++) {
                if (results[i-1].resultsAmount != results[i].resultsAmount) {
                    #ifdef  REPORTING_3 
                        printf("The resultsAmount is inconsistent among different results entries.");
                    #endif
                    exit(1);
                }
            }
        }
    }

        // Create container full of results to return 
        int finalResult_length = results[0].resultsAmount;

        acnfl_GenericFunctionResult container_finalResult = {.resultsAmount = finalResult_length,
                .results = malloc(sizeof(acnfl_NumberObject)*finalResult_length)};
        if (container_finalResult.results == NULL) {
            exit(1);
        }
        
        //Initialze container to return/
        acnfl_NumberObject *finalResult = container_finalResult.results;
        #define FINALRESULT_LENGTH results[0].resultsAmount
        {
            for (int i = 0; i < FINALRESULT_LENGTH; i++) {
                finalResult[i] = acnfl_generateApx(0, 0);
            }
        }

    // Average together all of the calculations. 
    // Bias conclusion torwards the results closer to location
    
    // Calculate "total space" for all of the conclusions
    #define RATIO_TOTAL_FORMULA(number)  3 * pow(6.0/5, number)
    double total = 0; 
    for (int currentNumber = 0; currentNumber < numberOfTests; currentNumber++) {
        total += RATIO_TOTAL_FORMULA(currentNumber);
    }
    #ifdef REPORTING_1
        printf("Total is %f", total);
    #endif
    // Average out and add together results
    for (int i = 0; i < numberOfTests ; i++){

        long double ratio = 0.0;
        {
            // Figure out the percentage of the total that this result is going to make up.
            #ifdef REPORTING_1

                printf("\n%d is %f, %f/%f is %f %% \n\n", 
                        i+1, RATIO_TOTAL_FORMULA(i), RATIO_TOTAL_FORMULA(i), total, (RATIO_TOTAL_FORMULA(i))/total);
            #endif

            ratio  = (RATIO_TOTAL_FORMULA(i))/total;
        }
        // Add this to the result
        {
            for (int j = 0; j < results->resultsAmount ; j ++) {
                acnfl_NumberObject product = acnfl_multiply(acnfl_generateApx(ratio, 0), results[i].results[j]);
                finalResult[j] = acnfl_add(finalResult[j], product);
            }
       }

    }
    

    // Memory cleanup. 
    {
        {
            acnfl_freeListOfFunctionResults(results);
            acnfl_freeListOfFunctionResults(resultsBank);
        }
            }

    return container_finalResult;
}

/**
 * Prints a derivative. Mostly used for testing. 
 */
void print_derivative(acnfl_NumberObject locationA, acnfl_GenericFunctionResult derivativeA) {
    {
        printf("\nThe derivative at ");
        acnfl_printValue(locationA);
        printf(" is ");
        for (int i = 0; i < derivativeA.resultsAmount; i++) {
            acnfl_printValue(derivativeA.results[i]);
            printf(" ");
        }
    }
}

