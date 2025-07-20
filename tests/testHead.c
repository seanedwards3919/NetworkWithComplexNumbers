/** @file testHead.c

    Uses CUnit to perform unit tests on the program.
*/
#import "testHead.h"
#include <CUnit/CUError.h>
#include <CUnit/TestDB.h>
#import "../acnfl/acnfl_math.h"
#include <CUnit/CUnit.h>
#include <math.h>


#define COMMON_NUMBERS_LENGTH 200
acnfl_NumberObject commonNumbers[COMMON_NUMBERS_LENGTH];
int position, length_1st;

#define LOOP_NUMBERS_LENGTH 8

/**
    Initializes numbers used for testing.
 */
int initialize_numbers_1st(){
    
    position = 0;

    long double loopNumbers[LOOP_NUMBERS_LENGTH] = {0.00, NAN, INFINITY, 1.0, -1.0, 5.5, M_PI, M_E} ;
    int loopNumbers_length = LOOP_NUMBERS_LENGTH;
    
    for (int k = 0; k < loopNumbers_length; k++) {
        for (int j = 0; j <loopNumbers_length; j++) {
            commonNumbers[position] = acnfl_generateApx(loopNumbers[k], loopNumbers[j]);
            position++;
            if (position > 200) {goto end;}
        }
    }

    end:
    length_1st = position;
    return 0;

}

void tests_1st_addition(void) {
    acnfl_NumberObject add_result = acnfl_add(commonNumbers[0], commonNumbers[4]);
    acnfl_NumberObject originalResult = ((struct acnfl_numberObject) commonNumbers[4]);
    //add_result == originalResult;
    /// TODO: Write function to check for equality;

}


CU_ErrorCode test_HeadProcess(void) {
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    CU_pSuite firstSuite = CU_add_suite("operation_add_1", initialize_numbers_1st, 0);
    if (CU_get_error() != CUE_SUCCESS) goto ending;

    CU_add_test(firstSuite, "Addition Tests", tests_1st_addition);



    ending:
    CU_ErrorCode toReturn = CU_get_error();
    CU_cleanup_registry();
    return toReturn;

}