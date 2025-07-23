/** @file testHead.c

    Uses CUnit to perform unit tests on the program.
*/
#include "testHead.h"
#include <CUnit/CUError.h>
#include <CUnit/TestDB.h>
#include "../acnfl/acnfl_math.h"
#include <CUnit/CUnit.h>
#include <CUnit/TestDB.h>
#include <math.h>
#include <CUnit/Automated.h>
#include <CUnit/Console.h>


#define COMMON_NUMBERS_LENGTH 200
acnfl_NumberObject commonNumbers[COMMON_NUMBERS_LENGTH];
int position, length_1st;

#define LOOP_NUMBERS_LENGTH 8

/**
    Initializes numbers used for testing  numbers.
 */
int tests_initialize_numbers(){
    
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



void test_appx_equality_inequality(void) {
    char methods[] = {'d', 'i', 'a'};
    
    // default method. 
    CU_ASSERT_EQUAL(acnfl_comparison(commonNumbers[0], commonNumbers[0], 0, methods), 0)
    CU_ASSERT_EQUAL(acnfl_comparison(commonNumbers[0], commonNumbers[3], 0, methods), 0);
    CU_ASSERT_EQUAL(acnfl_comparison(commonNumbers[46], commonNumbers[25], 0, methods), 1)
    CU_ASSERT_EQUAL(acnfl_comparison(commonNumbers[25], commonNumbers[46], 0, methods), -1)

    //  imaginary method 
    CU_ASSERT_EQUAL( acnfl_comparison(commonNumbers[2], commonNumbers[2], 0, (methods+1)) ,0);
    CU_ASSERT_EQUAL(  acnfl_comparison(commonNumbers[37], commonNumbers[28], 0, (methods+1)) ,1);
    CU_ASSERT_EQUAL(  acnfl_comparison(commonNumbers[50], commonNumbers[22], 0, (methods+1)) ,1);
    CU_ASSERT_EQUAL(  acnfl_comparison(commonNumbers[48], commonNumbers[28], 0, (methods+1)) ,1);
    CU_ASSERT_EQUAL(  acnfl_comparison(commonNumbers[28], commonNumbers[2], 0, (methods+1)),-1);
    CU_ASSERT_EQUAL( acnfl_comparison(commonNumbers[0], commonNumbers[2], 0, (methods+1)), -1);

    // Absolute method
    CU_ASSERT_EQUAL( acnfl_comparison(commonNumbers[3], commonNumbers[24], 0, (methods+2)) ,0);
    CU_ASSERT_EQUAL(  acnfl_comparison(commonNumbers[29], commonNumbers[37], 0, (methods+2)) ,0);

}

CU_TestInfo tests_appx_db[] = {
    {"Equality_inequality", test_appx_equality_inequality},
    CU_TEST_INFO_NULL
};

CU_SuiteInfo suite_db[] = {
    {"Approximate number tests", 0, 0, 0, 0, tests_appx_db},
    CU_SUITE_INFO_NULL,
};

CU_ErrorCode test_HeadProcess(void) {
    /*Initialize numbers*/
    tests_initialize_numbers();

    /*Initialize CUnit registry.*/
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    /* Register cunit suites */
    CU_ErrorCode allSuites  =  CU_register_suites(suite_db);
    if (allSuites != CUE_SUCCESS) return CU_get_error();

    CU_automated_run_tests();
    //CU_console_run_tests();
    

    ending:
    CU_ErrorCode toReturn = CU_get_error();
    CU_cleanup_registry();
    return toReturn;

}   