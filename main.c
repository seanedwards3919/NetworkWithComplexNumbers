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

#include <stdio.h>
#include "acnfl/acnfl_math.h"


/**
 * This is the main function for this program. We're just putting this here to set up Doxygen.
 */
int main() {
    int x =5;
    printf("Compilation test.\n");
    printf("%d\n", x);

    printf(" ");

    acnfl_NumberObject a = acnfl_generateApx(2.4, 0), 
    b = acnfl_generateApx(0, 1);

    acnfl_errorReporting(&a);
    acnfl_errorReporting(&b);


    

   }