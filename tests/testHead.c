/** @file testHead.c

    Uses CUnit to perform unit tests on the program.
*/
#include "testHead.h"
#include <CUnit/CUError.h>
#include <CUnit/TestDB.h>
#include "../acnfl/acnfl_math.h"
#include <CUnit/CUnit.h>
#include <math.h>
#include <CUnit/Automated.h>
#include <CUnit/Console.h>
#include <stdbool.h>
#include "../acnfl/acnfl_derivative.h"
#include <stdio.h>
#include <stdlib.h>


/** TEST FILES */
#include "tests.h"

CU_TestInfo tests_appx_db[] = {
    {"Equality_inequality", test_appx_equality_inequality},
    {"Addition_subtraction", test_appx_addition},
    {"Multiplication", test_appx_multiplication},
    {"Division", test_appx_division},
    CU_TEST_INFO_NULL
};

CU_TestInfo derivatives_db[] = {
    {"First round of derivative tests", derivative_first_round},
    CU_TEST_INFO_NULL
};

CU_TestInfo neuronlayer_tests_db[] = {
    {"Feedforward tests", outputErrorTests_A},
    {"Hidden error tests", calculateHiddenErrorTests_A},
    CU_TEST_INFO_NULL
};

CU_SuiteInfo suite_db[] = {
    {"Approximate number tests", 0, 0, 0, 0, tests_appx_db},
    {"Derivative tests", 0,0,0,0 ,derivatives_db},
    {"Neuron layer tests", 0, 0, neuronLayer_setupTests, neuronLayer_teardownTests, neuronlayer_tests_db},
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
    

    CU_ErrorCode toReturn = CU_get_error();
    CU_cleanup_registry();
    return toReturn;

}   
