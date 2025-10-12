
#include "../acnfl/acnfl_math.h"
#include "../acnfl/acnfl_derivative.h"
#include <stdlib.h>
#include "testHead.h"



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
    acnfl_NumberObject //a = acnfl_generateApx(NAN, 0), ///TODO: Write tests using NAN
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

