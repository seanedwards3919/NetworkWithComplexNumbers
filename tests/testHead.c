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


#define COMMON_NUMBERS_LENGTH 200
/**Okay, so don't touch this, but we're not using this to generate tests 
  *any more. Just unnececarily clunky. */
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
    acnfl_defaultComparisonInformation methods[] = {
                            (acnfl_defaultComparisonInformation) {
                               'd', true, 0.001
                            }
                        , 
                            (acnfl_defaultComparisonInformation) {
                                'i', true, 0.001
                            }
                        , 
                            (acnfl_defaultComparisonInformation) {
                                'a', true, 0.001
                            }};
    
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

void test_appx_addition(void){
    acnfl_NumberObject a = acnfl_generateApx(NAN, 0),
                       b = acnfl_generateApx(5, 5),
                       c = acnfl_generateApx(10, 10),
                       d = acnfl_generateApx(-5, 5),
                       e = acnfl_generateApx(1.56, 0),
                       f = acnfl_generateApx(0, 10),
                       g = acnfl_generateApx(5, 15);
    
    CU_ASSERT_EQUAL(acnfl_defaultComparison(b, b, (acnfl_defaultComparisonInformation) {.opType='d'}), 0);
    CU_ASSERT_EQUAL(
        acnfl_defaultComparison(
            acnfl_add(b, b), 
            c, 
            (acnfl_defaultComparisonInformation) {.opType='a'}), 
        0)
    CU_ASSERT_EQUAL(  
        acnfl_defaultComparison(
            acnfl_add(
                b, 
                d), 
            ancfl_subtract(
                g, b), 
            (acnfl_defaultComparisonInformation) {.opType='a'}), 
        0);
    CU_ASSERT_EQUAL(
        acnfl_defaultComparison(
            acnfl_add(
                e, f),  
            acnfl_generateApx(1.56, 10), 
            (acnfl_defaultComparisonInformation) {.opType='a'}), 
        0);

}


void test_appx_multiplication(void) {
    
    ACNFL_testing_equal( 
        acnfl_multiply(gApx(1.2,0), gApx(0, 1.4)), 
        gApx(0, 1.68));

    ACNFL_testing_equal( 
        acnfl_multiply(gApx(1.2,1), gApx(0, 1.4)), 
        gApx(-1.4, 1.68));
    
    ACNFL_testing_equal(
        acnfl_multiply(gApx(-1,2),gApx(0,1) ), 
        gApx(-2,-1))

    ACNFL_testing_equal( 
        acnfl_multiply(gApx(1.2,0), gApx(0, 0)), 
        gApx(0, 0));

    ACNFL_testing_equal( 
        acnfl_multiply(gApx(1.2,0), gApx(0, 0)), 
        gApx(0, 0));

    ACNFL_testing_equal( 
        acnfl_multiply(gApx(26.77,1245.12), gApx(2, 0)), 
        gApx(53.54, 2490.24));
        
    ACNFL_testing_equal( 
        acnfl_multiply(gApx(2,3), gApx(0, 1)), 
        gApx(-3,2));

    ACNFL_testing_equal( 
        acnfl_multiply(gApx(2,3), gApx(0, -1)), 
        gApx(3,-2));

}

void test_appx_division(void) {
 ACNFL_testing_equal( 
     acnfl_divide(gApx(1,2), gApx(3, 4)), 
    gApx((11.0/25.0),(2.0/25.0)));
 ACNFL_testing_equal( 
     acnfl_divide(gApx(1,2), gApx(-3, -4)), 
    gApx((-11.0/25.0),(-2.0/25.0)));

 ACNFL_testing_equal( 
     acnfl_divide(gApx(1241.124,12.4), gApx(-645, 25)), 
    gApx((-1.92059),(-0.09366)));




}

/** ----------------------- **/
/** ONLY FOR USE WITHIN FUNCTIONS THAT RETURN acnfl_GenericFunctionResult */
#define acnfl_function_preliminaryCheck(parameterCount, minumumParameter, parameterList, allocatedResult) \
    if (parameterCount < minumumParameter) return (acnfl_GenericFunctionResult) {.resultsAmount = 0, .results = NULL};\
    acnfl_NumberObject* resultPointer = malloc(sizeof(acnfl_NumberObject)*allocatedResult);\
    if (resultPointer == NULL) return (acnfl_GenericFunctionResult) {.resultsAmount = 0, .results = NULL}; \
    acnfl_GenericFunctionResult toReturn = {.resultsAmount = allocatedResult, .results = resultPointer};

acnfl_GenericFunctionResult multipleOutputs(long long parameterCount, acnfl_NumberObject *parameterList) {
    acnfl_function_preliminaryCheck(parameterCount, 2, parameterList, 2);
        resultPointer[0] =
            // Actual equation, only reads first
                        acnfl_multiply(parameterList[0], parameterList[0]);
            resultPointer[1] =
        // x*(x^2+2y^-1), x = parameterList[0], y = parameterList[1]
            acnfl_multiply(parameterList[0], acnfl_add(
                acnfl_multiply(
                    parameterList[0], parameterList[0]), 
                acnfl_multiply(
                    acnfl_generateApx(2, 0), acnfl_divide(
                        acnfl_generateApx(2, 0), parameterList[1])
                    )
                )
            );
 
        return toReturn;
}

acnfl_GenericFunctionResult summation_function(long long parameterCount, acnfl_NumberObject *parameterList) {
    acnfl_function_preliminaryCheck(parameterCount, parameterCount, parameterList, 1);
    resultPointer[0] = acnfl_generateApx(0,0);
    for (int counter = 0; counter < parameterCount; counter++) { 
        resultPointer[0] = acnfl_add(resultPointer[0], acnfl_multiply(parameterList[counter], acnfl_generateApx(counter+1, 0)) );
        ; 
    }
    return toReturn;
}

acnfl_GenericFunctionResult multipleInputs2(long long parameterCount, acnfl_NumberObject *parameterList) {
    acnfl_function_preliminaryCheck(parameterCount, 3, parameterList, 1);
    resultPointer[0] = 
        //x(y+2i(c))
            acnfl_multiply(parameterList[0], acnfl_add(parameterList[1], acnfl_multiply(parameterList[2], acnfl_generateApx(0, 2))));
    return toReturn;
}

acnfl_GenericFunctionResult multipleInputs(long long parameterCount, acnfl_NumberObject *parameterList) {
    acnfl_function_preliminaryCheck(parameterCount, 2, parameterList, 1);
    resultPointer[0] =
        // x*(x^2+2y^-1), x = parameterList[0], y = parameterList[1]
            acnfl_multiply(parameterList[0], acnfl_add(
                acnfl_multiply(
                    parameterList[0], parameterList[0]), 
                acnfl_multiply(
                    acnfl_generateApx(2, 0), acnfl_divide(
                        acnfl_generateApx(2, 0), parameterList[1])
                    )
                )
            );
    return toReturn;
}


acnfl_GenericFunctionResult x_squared(long long parameterCount, acnfl_NumberObject *parameterList) {
    // Only takes the first parameter.
   acnfl_function_preliminaryCheck(parameterCount, 1, parameterList, 1);
    resultPointer[0] =
            // Actual equation, only reads first
                        acnfl_multiply(parameterList[0], parameterList[0]);

    return  toReturn;
}

acnfl_GenericFunctionResult randomPolynomial(long long parameterCount, acnfl_NumberObject *parameterList) {
    // Only takes the first parameter.
    
   acnfl_function_preliminaryCheck(parameterCount, 1, parameterList, 1);    // Only takes the first parameter.

    acnfl_NumberObject result = acnfl_generateApx(.28, 0);
    result = acnfl_multiply(result, acnfl_multiply(parameterList[0], acnfl_multiply(parameterList[0], parameterList[0])));
    result = ancfl_subtract(result, acnfl_multiply(acnfl_generateApx(2, 0), acnfl_multiply(parameterList[0], parameterList[0])));
    result = acnfl_add(result, acnfl_multiply(acnfl_generateApx(2, 0), parameterList[0]));
    result = ancfl_subtract(result, acnfl_generateApx(2, 0));
    resultPointer[0] = result;
            
            
                        
    return  toReturn;
    
}

void derivative_first_round(void) {
    //acnfl_GenericFunctionDefinition test;


    acnfl_NumberObject locationA = acnfl_generateApx(2.5, 0),
                        locationB = acnfl_generateApx(362.5555, 0), 
                        locationC[2] = {acnfl_generateApx(2.0, 0.0), acnfl_generateApx(2.0, 0.0)},
                        locationD[3] = {
                            acnfl_generateApx(0, 99),
                            acnfl_generateApx(5, 0),
                            acnfl_generateApx(0, 2)
                        },
                        locationE[] = {
                            acnfl_generateApx(2, 0),
                            acnfl_generateApx(4, 0),
                            acnfl_generateApx(0, 2),
                            acnfl_generateApx(5, 2),
                       };
    acnfl_GenericFunctionResult derivativeA = 
        acnfl_derivative_default(&locationA, 1, 0,  x_squared), 
                                derivativeB =
        acnfl_derivative_default(&locationB, 1, 0,  randomPolynomial),
                                derivativeC = 
        acnfl_derivative_default(locationC, 2, 1,  multipleInputs),
                                derivativeD_1 = 
        acnfl_derivative_default(locationD, 3, 0,  multipleInputs2),
                                derivativeD_2 = 
        acnfl_derivative_default(locationD, 3, 1,  multipleInputs2),
                                derivativeD_3 = 
        acnfl_derivative_default(locationD, 3, 2,  multipleInputs2),
                                derivativeE_1 = 
        acnfl_derivative_default(locationE, 4, 2,  summation_function),
                                derivativeE_2 = 
        acnfl_derivative_default(locationE, 4, 3,  summation_function),
                            derivativeF =
        acnfl_derivative_default(locationC, 2, 1,  multipleOutputs);

        


    ACNFL_testing_equal(derivativeA.results[0], acnfl_generateApx(5.0, 0));
    ACNFL_testing_equal(derivativeB.results[0], acnfl_generateApx(108966.701811, 0.136)); /** TODO: This is "Good enough," but is there any way to maybe entirely remove the derivative part here if it isn't relevant? */
    ACNFL_testing_equal(derivativeC.results[0], acnfl_generateApx(-2, 0));
    ACNFL_testing_equal(derivativeD_1.results[0], acnfl_generateApx(1, 0));
    ACNFL_testing_equal(derivativeD_2.results[0], acnfl_generateApx(0, -99));
    
    ACNFL_testing_equal(derivativeD_3.results[0], acnfl_generateApx(-198, 0));
    
    ACNFL_testing_equal(derivativeE_1.results[0], acnfl_generateApx(3, 0));
    
    ACNFL_testing_equal(derivativeE_2.results[0], acnfl_generateApx(4, 0));
    
    ACNFL_testing_equal(derivativeF.results[0], acnfl_generateApx(00, 0));
 
    ACNFL_testing_equal(derivativeF.results[1], acnfl_generateApx(-2, 0));
    



    acnfl_freeHeldResults(&derivativeA);
    acnfl_freeHeldResults(&derivativeB);
    acnfl_freeListOfFunctionResults(&derivativeC,2);
    acnfl_freeHeldResults(&derivativeD_1);
    acnfl_freeHeldResults(&derivativeD_2);
    acnfl_freeHeldResults(&derivativeD_3);
    acnfl_freeHeldResults(&derivativeE_1);
    acnfl_freeHeldResults(&derivativeE_2);
    acnfl_freeHeldResults(&derivativeF);
    printf("\n\n\n");
       return;
}



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

CU_SuiteInfo suite_db[] = {
    {"Approximate number tests", 0, 0, 0, 0, tests_appx_db},
    {"Derivative tests", 0,0,0,0 ,derivatives_db},
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