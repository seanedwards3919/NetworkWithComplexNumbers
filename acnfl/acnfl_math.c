
/** @file acnfl_math.c
 * ACNFL stands for: Automatically Compatable Numbertypes and Functions Library. 
 */
#include "acnfl_math.h"
#include "../reporting/reporting_3.h"

#include <stdbool.h>
#include <stdio.h>
#include <math.h>

/****************************************************************************8
 * RIGHTNESS CHECKING & ERROR HANDLING
 *****************************************************************************/

/**
 * Checks a acnfl_NumberObject for an error and returns a boolean if true.
 * Only checks for error codes right now. In the future, if an error is detected,
  *  and not already flagged, then this funciton
 * modifies the passed value and inserts an error code into the passed value .
 */
inline char acnfl_errorHandling(acnfl_NumberObject *a) {
    // If error detected

    if (a->nType == 'e') {
        #ifdef REPORTING_3
            printf("ERROR DETECTED: \n ntype is 'e' ");
        #endif
        return 'n';}
    if (a->vType == 'e') {
        #ifdef REPORTING_3
            printf("ERROR DETECTED: \n vtype is 'e' ");
        #endif
        return 'v';}
    return 'c';  
}


/**
 * Checks the acnfl_NumberObjects for errors in binary operations, and returns an 
 * appropriate acnfl_errorHandling object.
 * 
 * Checks for errors from a_error and b_error first, and then deterimines the correct 
 * nType and vType to return if error detected 
 * 
 * TODO: I don't think this is used, lol 
 */
inline acnfl_NumberObject acnfl_errorHandlingBinary (acnfl_NumberObject a , acnfl_NumberObject b,
        char a_error, char b_error ) {
    
    acnfl_NumberObject to_return ;
    if ((a_error == 'c') && (b_error == 'c') ) {
        to_return.nType = 'o';
        to_return.vType = 'k';
        return to_return;
    }

    // Determine correct nType to return. 
    if ((a.nType == 'r') || (b.nType == 'r'))  to_return.nType= 'r';
    if ((a.nType == 'c') || (b.nType == 'c'))  to_return.nType = 'c';

    if ((a.nType == 'e') || (b.nType == 'e'))  to_return.nType = 'e';

    // Determine correct vType to return 
    if ((a.vType == 'a') || (b.vType == 'a'))  to_return.vType = 'b';
    
    if ((a.vType == 'e') || (b.vType == 'e'))  to_return.vType = 'e';

   return to_return ;
}

/**
 * Check if a number marked as real has a complex component.
 * If so, marks the number as complex. 
 * If this fails, the errorFlag for number is set to true.
 */
void acnfl_checkComplexity(acnfl_NumberObject *number, bool *errorFlag) {
    if (number->vType == 'a') {
        if ( ( number->nType == 'r') && (number->imaginaryNumberValue_apx != 0) ) {
            #ifdef REPORTING_2
                printf("Something's gone wrong. "
                "Number a : %La + i*%La is marked as \n" 
                "a real number, but the complex number part"
                "is not zero. The  value will be\n"
                "marked as complex.", number->realNumberValue_apx , 
                number->imaginaryNumberValue_apx);
            #endif
            number->nType = 'c';
        }
    } else {
         #ifdef REPORTING_3
            printf("vType behavior not defined! Check impossible!"
                    "vType, %c\n", number->vType);
        #endif       
        (*errorFlag) = true;
    }
}


#ifdef REPORTING_3
/**
 * Dumps memory for acnfl_NumberObject.
 */
    void acnfl_errorReporting(acnfl_NumberObject *a) {
        printf("Dumping memory of number at %p.\n", a);
        printf("Parameters: nType %c, vType %c \n", a->nType, a->vType);
        printf("Memory dump:. ");
        
    short int counter = 0;
    short int length = sizeof(acnfl_NumberObject);

    unsigned char* beginning = (unsigned char*) a;


    printf("\n     ");
    for ( int i = 0 ; i < length ; i++ ) {
        
        printf("%02hhx ", *(beginning+i));
        counter++;
        if (counter == 15) {
            counter = 0;
            printf("\n     ");
        }
    }
    printf("\n");
        return; 
    }
#endif



/****************************************************************************8
 * OPERATIONS
 *****************************************************************************/

/*********
 * Common code shared between all operations
 *********/

/**
 * Operation common function format 
 * Check for error and if it exists, set the error flag and return.
 * Check the complex numbers and modify a and b if not consistent 
 */
void acnfl_unaryOperationCheck(acnfl_NumberObject *a, acnfl_NumberObject *returnValue, 
    bool *errorFlag) {
    // Check for error.
    char err = 'c';//acnfl_errorHandling(a); 
    ///TODO: Calling acnfl_errorHandling here was causing an error for some reason? What?
    // Return error results if there is a error
    if (err != 'c') {
        #ifdef REPORTING_3
            printf("Error code %c reported!\n", err);
        #endif
        (*errorFlag)  = true;
        return;
    }

    acnfl_checkComplexity(a, errorFlag);
}

/**
 * In addition to operations in unaryOpertionCommon, 
 * tries to determine the correct output value of the returnValue;
 */
void acnfl_binaryOperationCheck(acnfl_NumberObject *a, acnfl_NumberObject *b,  
    acnfl_NumberObject *returnValue, bool *errorFlag) {
    //For each vType 
        acnfl_unaryOperationCheck(a, returnValue, errorFlag);
        acnfl_unaryOperationCheck(b, returnValue, errorFlag);

        // For each nType 
        // When both numbers are real
        if ( (a->nType == 'r') && (b->nType == 'r')) {
            #ifdef REPORTING_1
                printf("Real number.\n");
            #endif
            returnValue->nType = 'r';
        // When one number is complex.
        } else if ( (a->nType == 'c') || (b->nType == 'c') ) {
            #ifdef REPORTING_1
                printf("Complex number\n");
            #endif
            returnValue->nType = 'c';
        }
        else {
            #ifdef REPORTING_2
                  printf("nType combination not defined! Setting to complex as default."
                    "a nType: %c .. b nType %c", a->nType, b->nType);
            #endif
            returnValue->nType = 'c';
        }
    
}
/**
 * Common code for binary function 
 */
acnfl_NumberObject acnfl_binaryOperationCommon(acnfl_NumberObject a, acnfl_NumberObject b,
            acnfl_NumberObject (*functionList[])(acnfl_NumberObject, acnfl_NumberObject, void*)) {
    acnfl_NumberObject returnValue;
    bool errorFlag = false;
    acnfl_binaryOperationCheck(&a, &b, 
        &returnValue, &errorFlag);
    if (errorFlag) {
        #ifdef REPORTING_3 
            printf("Error detected in binaryOperationCheck");
           acnfl_errorReporting(&b);
           acnfl_errorReporting(&a);
            goto errorHandling;
        #endif 
    }

        // When both numbers are 'a':
    if ( (a.vType == 'a') && (b.vType == 'a') ) {
        #ifdef REPORTING_1
            printf("vType is 'a', 'a' : %c, %c", a.vType, b.vType);
        #endif
        returnValue.vType = 'a';
        acnfl_NumberObject result = (*functionList[0])(a,b,&returnValue);
         
        /// TODO: Perform sanity checks here. 
        if (returnValue.vType == result.vType) {
                ;
        } else {
            #ifdef REPORTING_3
            printf("returnValue and result have different vType. result will overwrite returnValue"
             "\n returnvalue %c, result %c\n Function at %p", returnValue.vType, result.vType, 
             (functionList[0]));
             acnfl_errorReporting(&a);
             acnfl_errorReporting(&b);

            #endif
            #ifdef SAFETY
                printf("Safety mode active! Erasing value!")
                goto errorHandling;
            #endif
            
        }
        returnValue = result;

        }
    else {
        #ifdef REPORTING_3
            printf("vType combination not defined! Returning error value!\n"
                    "a vType: %c .. b vType %c", a.vType, b.vType);
                    
            acnfl_errorReporting(&a); acnfl_errorReporting(&b);
        #endif
        goto  errorHandling; 
    }

    return returnValue;



    errorHandling:
    #ifdef REPORTING_3
        printf("Error reported. Returning blank value!\n");
        returnValue = (acnfl_NumberObject) {0};
        returnValue.nType = 'c';
        returnValue.nType = 'a';
    #endif 
    return  returnValue;
}

/*********
 * Binary operations code and top-level functions 
 *********/

/** The total number of functions needed per operation. Best 
  * to hard-code this in order to avoid any sort of discrepancies 
  * (i.e. one operation having one less function than needed) at 
  * compile-time. */
#define NUMBEROF_FUNCTION_OPERATIONS 1

 /**~~~~~~
    ~  Addition/subtraction.
    ~~~~~~*/

/***
 * Add two approximate-format numbers together.
 */
acnfl_NumberObject acnfl_binary_a_a_add(acnfl_NumberObject a, acnfl_NumberObject b, void *other) { /** TODO: clean *other up in the same way as the comparison functions?*/ 
    acnfl_NumberObject toReturn;
    toReturn.vType = 'a';
    toReturn.nType = ((acnfl_NumberObject*) other)->nType;
    toReturn.imaginaryNumberValue_apx = (a.imaginaryNumberValue_apx + b.imaginaryNumberValue_apx);
    toReturn.realNumberValue_apx = (a.realNumberValue_apx + b.realNumberValue_apx);

    return toReturn;
    /*        (acnfl_NumberObject) {.vType = 'a', .nType = ((acnfl_NumberObject*) other)->nType,
                .imaginaryNumberValue_apx = (a.imaginaryNumberValue_apx + b.imaginaryNumberValue_apx),
                .realNumberValue_apx = (a.realNumberValue_apx + b.realNumberValue_apx)};*/
}

/**
* List of internal addition functions.`
 */
acnfl_NumberObject (*acnfl_addFunctions[NUMBEROF_FUNCTION_OPERATIONS])(acnfl_NumberObject, acnfl_NumberObject, void*) = 
        {acnfl_binary_a_a_add};

/**
 * Top-level addition function. 
 * 
 * THIS IS A USER FUNCTION.
 */

 acnfl_NumberObject acnfl_add(acnfl_NumberObject a, acnfl_NumberObject b) {
    return acnfl_binaryOperationCommon(a, b, acnfl_addFunctions);
        
 }

/**
 * Top level subtraction function.
 *
 * This is implemented by reversing b and simply calling the addition function.
 * This is functionally a - b.
 *
 * THIS IS A USER FUNCTION
 */
acnfl_NumberObject ancfl_subtract(acnfl_NumberObject a, acnfl_NumberObject b) {
    // Reverse value of b
    if (b.vType == 'a') {
        b.imaginaryNumberValue_apx = (-1) * b.imaginaryNumberValue_apx;
        b.realNumberValue_apx = (-1) * b.realNumberValue_apx;
    } else {
        #ifdef REPORTING_3
            printf("vType not supported. vType: %c", b.vType);
            printf("Zeroing out values.");
        #endif
        char vType_temp = b.vType;
        char nType_temp = b.nType; 
        b = (acnfl_NumberObject) {0};
        b.vType = vType_temp;
        b.nType = nType_temp;
    }

    return acnfl_add(a, b);
}


 /**~~~~~~
    ~  Multiplication
    ~~~~~~*/
    

/**
 * Multiply two approximate-format numbers together;
 */
 acnfl_NumberObject acnfl_binary_a_a_multiply(acnfl_NumberObject a, acnfl_NumberObject b, void *extra){
        acnfl_NumberObject toReturn;
        toReturn.nType = ((acnfl_NumberObject*) extra )->nType;
        toReturn.vType = 'a';
        toReturn.realNumberValue_apx = ( (a.realNumberValue_apx * b.realNumberValue_apx)
                                         - (a.imaginaryNumberValue_apx * b.imaginaryNumberValue_apx));
        toReturn.imaginaryNumberValue_apx = ( (a.realNumberValue_apx * b.imaginaryNumberValue_apx)
                                         + (a.imaginaryNumberValue_apx * b.realNumberValue_apx));


        return toReturn;
            /*(acnfl_NumberObject) {((acnfl_NumberObject*) extra )->nType, 'a', 
                .realNumberValue_apx = ( (a.realNumberValue_apx * b.realNumberValue_apx)
                                         - (a.imaginaryNumberValue_apx * b.imaginaryNumberValue_apx)),
                .imaginaryNumberValue_apx = ( (a.realNumberValue_apx * b.imaginaryNumberValue_apx)
                                         + (a.imaginaryNumberValue_apx * b.realNumberValue_apx)) };*/
    }

/**
 * List of internal multiplication functions.
 */

 acnfl_NumberObject (*acnfl_multiplyFunctions[NUMBEROF_FUNCTION_OPERATIONS])(acnfl_NumberObject, acnfl_NumberObject, void*) = 
 {acnfl_binary_a_a_multiply};

 /**
  * Top level multiplication function.
  * 
  * THIS IS A USER FUNCTION 
  */
 
acnfl_NumberObject acnfl_multiply(acnfl_NumberObject a, acnfl_NumberObject b) {
    return acnfl_binaryOperationCommon(a, b, acnfl_multiplyFunctions);
}

 /**~~~~~~
    ~  Division
    ~~~~~~*/
/**
 * Divides two approximate format numbers.
 */
acnfl_NumberObject acnfl_binary_a_a_divide(acnfl_NumberObject a, acnfl_NumberObject b, void *extra) {
    acnfl_NumberObject toReturn;
    toReturn.nType = ((acnfl_NumberObject *) extra)->nType;
    toReturn.vType = 'a';
    toReturn.realNumberValue_apx = ( 
                                ( (a.realNumberValue_apx * b.realNumberValue_apx) + (a.imaginaryNumberValue_apx*b.imaginaryNumberValue_apx) ) 
                                    / 
                                 ( (b.realNumberValue_apx*b.realNumberValue_apx) + (b.imaginaryNumberValue_apx*b.imaginaryNumberValue_apx)));
        toReturn.imaginaryNumberValue_apx = (
                                    ( (a.imaginaryNumberValue_apx*b.realNumberValue_apx) - (a.realNumberValue_apx*b.imaginaryNumberValue_apx) ) 
                                        / 
                                     ( (b.realNumberValue_apx*b.realNumberValue_apx) + (b.imaginaryNumberValue_apx*b.imaginaryNumberValue_apx) ))   ;

    return toReturn; /*(acnfl_NumberObject) {
        ((acnfl_NumberObject *) extra)->nType, 'a',
        .realNumberValue_apx = ( ( (a.realNumberValue_apx * b.realNumberValue_apx) + (a.imaginaryNumberValue_apx+b.imaginaryNumberValue_apx) ) 
                                    / 
                                 ( (b.realNumberValue_apx*b.realNumberValue_apx) + (b.imaginaryNumberValue_apx*b.imaginaryNumberValue_apx))),
        .imaginaryNumberValue_apx = (( (a.imaginaryNumberValue_apx*b.realNumberValue_apx) - (a.realNumberValue_apx*b.imaginaryNumberValue_apx) ) 
                                        / 
                                     ( (b.realNumberValue_apx*b.realNumberValue_apx) + (b.imaginaryNumberValue_apx*b.imaginaryNumberValue_apx) ))
    };*/
}

/** List of internal division functions. */
acnfl_NumberObject (*acnfl_divideFunctions[NUMBEROF_FUNCTION_OPERATIONS])(acnfl_NumberObject, acnfl_NumberObject, void*) = 
{acnfl_binary_a_a_divide};

/**
 * Divides two nuumbers Top level division function . a/b
 * Note that this does NOT CHECK whether b is non-zero. Like
 * with the C operation itself, you'll have to check that yourself.
 * 
 * THIS IS A USER FUNCTION.
 */
acnfl_NumberObject acnfl_divide(acnfl_NumberObject a, acnfl_NumberObject b) {
    /// TODO: Make sure this isn't dividing by zero.
    return acnfl_binaryOperationCommon(a, b, acnfl_divideFunctions);
}

/****************************************************************************8
 * INEQUALITIES . EQUALITIES 
 *****************************************************************************/

/********* 
 * General comparison function
 *********/
/**
 * This is the default comparison function for acnfl_NumberObjects.
 * This is written to return results in the same way comp is for bsearch;
 * defined as <1 for a < b, 0 for a == b, and >1 
 * for a > b
 * 
 * As there is no intuitive way to compare complex numbers, the exact
 * opreation that will be performed is decided by the information passed through
 * acnfl_defaultComparisonInformation.
 * 
* 
 */

float acnfl_defaultComparison(acnfl_NumberObject a, acnfl_NumberObject b,
    acnfl_defaultComparisonInformation parameter) {
        #ifdef NAN
            float returnValue = NAN;
        #elif 
            float returnValue = 0;
        #endif
        
        
        if ( (a.vType == 'a') && (b.vType == 'a') ) {
            
        acnfl_valueType_apx  a_com, b_com;

            if (parameter.opType == 'a') {
            #ifdef REPORTING_1
                printf("Passed parameter is %c\n"
                        , parameter.opType);
                printf("Proceed with 'o'");
            #endif

               a_com = ((a.imaginaryNumberValue_apx)*(a.imaginaryNumberValue_apx))+((a.realNumberValue_apx)*(a.realNumberValue_apx));
                
               b_com = ((b.imaginaryNumberValue_apx)*(b.imaginaryNumberValue_apx))+((b.realNumberValue_apx)*(b.realNumberValue_apx));
                

            } else if(parameter.opType == 'i') {
                #ifdef REPORTING_1

                printf("Passed parameter is %c\n"
                        , parameter.opType);
                printf("Proceed with 'i'");
                #endif

                a_com = a.imaginaryNumberValue_apx;
                b_com = b.imaginaryNumberValue_apx;

    
            } else /*parameter.opType == 'd' or other*/ {
               #ifdef REPORTING_2
                printf("Passed parameter is %c\n"
                        , parameter.opType);
                printf("Proceed with 'o'");
            #endif
                
                a_com = a.realNumberValue_apx; 
                b_com = b.realNumberValue_apx;

                
            }

            // Check for a == b 
            if (parameter.leeway_apx) {
                // Check for exact equality.
                if (a_com == b_com)  return 0;
                acnfl_valueType_apx diff  = fabsl(a_com-b_com),
                                    den  = fabsl(a_com) + fabsl(b_com),
                                    comparison = diff/den;
                if (comparison < parameter.epsilon_apx)  return 0; 
                
            }
            else {
                // Check for exact equality
                if (a_com == b_com) return 0;
            }

            // Check for a > b
            if   (a_com > b_com) return 1;

            // Return a < b
            else /*a_com < b_com*/ return -1;

        }

        else {
            #ifdef REPORTING_3
                printf("vType combination not defined."
                "a: %c, b: %c\n", a.vType, b.vType);
                printf("Returning NAN, "
                    "if possible.\n");
            #endif
        }

        ending: 
        return returnValue;
    }; 

/**
 * This is the top-level comparison function for acnfl_NumberObjects. 
 * If NULL is passed into alternateFunction, then it will used the defined 
 * alternative function. For the rest of this program, these will be expected
 * to be defined in the same style as  bsearch; <1 for a < b, 0 for a == b, and >1 
 * for a > b. This is what will be returned, as well.
 *
 * extraData can be user-defined and cast within alternateFunction. Within 
 * acnfl_defaultComparison, it is cast to char.
 *
 * Returns NAN if error occurs
 */
float acnfl_comparison(acnfl_NumberObject a, acnfl_NumberObject b, 
        float (*alternateFunction)(acnfl_NumberObject, acnfl_NumberObject, void*),
        void* extraData) {
            if (alternateFunction != NULL) {
                return alternateFunction(a,b, extraData); 
            }
        return acnfl_defaultComparison(a, b, 
            *((acnfl_defaultComparisonInformation*) extraData));
        
        };

/****************************************************************************8
 * DATA MANAGEMENT
 *****************************************************************************/

/*********
 * Generate objects
 *********/

/**
 * generates an acnfl_NumberObject using the approximate format
 *
 * THIS IS A USER FUNCTION.
 */
acnfl_NumberObject acnfl_generateApx(long double real, long double imaginary) {
    acnfl_NumberObject toReturn = {.vType = 'a'};
    if (imaginary != 0) toReturn.nType = 'c';
    else toReturn.nType = 'r';

    toReturn.imaginaryNumberValue_apx = imaginary;
    toReturn.realNumberValue_apx = real;

    return toReturn;
}