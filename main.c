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

int version(void) {
    return 1;
}