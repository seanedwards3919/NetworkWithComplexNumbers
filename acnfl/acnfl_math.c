
/** @file math.
 * ACNFL stands for: Automatically Compatable Numbertypes and Functions Library. 
 */
#include "acnfl_math.h"
#include "../reporting/reporting_3.h"

#include <stdbool.h>
#include <stdio.h>

/****************************************************************************8
 * RIGHTNESS CHECKING & ERROR HANDLING
 *****************************************************************************/

/**
 * Checks a acnfl_NumberObject for an error and returns a boolean if true.
 * Only checks for error codes right now. If error detected, and not already flagged, then this funciton
 * modifies a and inserts an error code into the data.
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
            printf("vType not defined! Check impossible!"
                    "vType, %c\n", number->vType);
        #endif       
        (*errorFlag) = true;
    }
}


#ifdef REPORTING_3
/**
 * Dump memory for acnfl_NumberObject
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
        
        printf("%2hhx ", *(beginning+i));
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
    char err = 't';//acnfl_errorHandling(a); 
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
        acnfl_NumberObject result = (*functionList[0])(a,b,&result);
         
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
        printf("Error reported. Returning blank value!");
        returnValue = (acnfl_NumberObject) {0};
        returnValue.nType = 'c';
        returnValue.nType = 'a';
    #endif 
    return  returnValue;
}

/*********
 * Binary operations code and top-level functions 
 *********/

/***
 * Add two approximate-format numbers together.
 */
acnfl_NumberObject acnfl_binary_a_a_add(acnfl_NumberObject a, acnfl_NumberObject b, void *other) {
    return 
        (acnfl_NumberObject) {.vType = 'a', .nType = ((acnfl_NumberObject*) other)->nType,
                .imaginaryNumberValue_apx = (a.imaginaryNumberValue_apx + b.imaginaryNumberValue_apx),
                .realNumberValue_apx = (a.realNumberValue_apx + b.realNumberValue_apx)};
}

/**
* List of internal addition functions.`
 */
acnfl_NumberObject (*acnfl_addFunctions[1])(acnfl_NumberObject, acnfl_NumberObject, void*) = 
        {acnfl_binary_a_a_add};

/**
 * Top-level addition function. 
 * 
 * THIS IS A USER FUNCTION.
 */

 acnfl_NumberObject acnfl_add(acnfl_NumberObject a, acnfl_NumberObject b) {
    return acnfl_binaryOperationCommon(a, b, acnfl_addFunctions);
        
 }


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