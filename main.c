#include "reporting/reporting_1.h"

#include <stdlib.h>
#include <stdio.h>
#include "acnfl/acnfl_math.h"
#include "tests/testHead.h"
#include <math.h>

#ifdef REPORTING_1
    #include <time.h>
#endif

#include "acnfl/acnfl_derivative.c"

/**
 * OVERALL PRIORITIES!!!!!!
 * 
 * Reduce ideas of objects having to "manage" everything
 * inside of them (esp. with memory management), make that easier 
 * and freeform. We want to be able to use the stack more when it comes
 * to memory.
 * - FUnctions are a big sore point. Allow the user to pass a pointer 
 *   in that allows the function to simply place the results where 
 *   the user pleases. Also pass how much memory is left.
 */

/** ----------------------- **/
/**genericFunctionResults
 * This is the main function for this program. We're just putting this here to set up Doxygen.
 */
int main() {
    #ifdef REPORTING_1
        clock_t startingTime_wholeProgram = clock();
    #endif

    /** Run tests */
    test_HeadProcess();



    #ifdef REPORTING_1
        clock_t endingTime_wholeProgram = clock();
        printf("\n");
        printf("This program took %Lf seconds and %ld ticks to run\n",
             ((long double) (endingTime_wholeProgram-startingTime_wholeProgram))/CLOCKS_PER_SEC,
            (endingTime_wholeProgram-startingTime_wholeProgram));
    #endif

   }