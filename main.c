/** @file main.c
 *  
 * Reporitng:
 * 
 * There are three levels of reporting. Level 1 is for routine events, such as regular program functioning.
 * Level 2 is for events that are signifigant but only truly useful for a developer or contributor.
 * Level 3 is VERY RARE, monumentious events that require immediete reporting, like undefined behavior 
 * or errors.
 * The standard macros used to activate these levels are:
 * 
 *  - REPORTING_1
 * 
 *  - REPORTING_2
 *
 *  - REPORTING_3
 *
 * There is also a safety macro for more safety. Please figure out something to do with this; 
 */

#include "reporting/reporting_3.h"

#include <stdlib.h>
#include <stdio.h>
#include "acnfl/acnfl_math.h"

#ifdef REPORTING_1
    #include <time.h>
#endif

/**
 * This is the main function for this program. We're just putting this here to set up Doxygen.
 */
int main() {
    #ifdef REPORTING_1
        clock_t startingTime_wholeProgram = clock();
    #endif

    int x =5;
    printf("Compilation test.\n");
    printf("%d\n", x);

    printf(" ");

    printf("Numbers are %ld bytes wide\n", sizeof(acnfl_valueType_apx));

    acnfl_NumberObject *numbers = calloc(3, sizeof(acnfl_NumberObject));
    numbers[0] = acnfl_generateApx(0,2);
    numbers[1] = acnfl_generateApx(124, 0);
    numbers[2] = acnfl_generateApx(2534, 2.2);
/*
    acnfl_errorReporting(numbers);
    acnfl_errorReporting(numbers +1);
    acnfl_errorReporting(numbers +2);
*/
    acnfl_NumberObject sum = acnfl_add(numbers[0], 
        acnfl_add(numbers[1], numbers[2]));
    acnfl_NumberObject product =  acnfl_multiply(numbers[0], numbers[2]);

    printf("The sum of these three numbers " 
    "is %Lf + %Lf*i\n", sum.realNumberValue_apx, sum.imaginaryNumberValue_apx);
    printf("The product of numbers 1 and 3 is %Lf + %Lf*i\n", 
        product.realNumberValue_apx, product.imaginaryNumberValue_apx);
    
    printf("Something went wrong with the multiplication, me. Set up testing framework now.");




    free(numbers);

    #ifdef REPORTING_1
        clock_t endingTime_wholeProgram = clock();
        printf("\n");
        printf("This program took %Lf seconds and %ld ticks to run",
             ((long double) (endingTime_wholeProgram-startingTime_wholeProgram))/CLOCKS_PER_SEC,
            (endingTime_wholeProgram-startingTime_wholeProgram));
    #endif

   }