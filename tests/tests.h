#ifndef TESTS_H 
#define TESTS_H 

#include "../acnfl/acnfl_math.h"
#include "../acnfl/acnfl_derivative.h"

void tests_initialize_numbers();
void test_appx_equality_inequality(void) ;
void test_appx_addition(void);
 void test_appx_multiplication(void) ;
 
void test_appx_division(void) ;

void derivative_first_round(void) ;
 
void neuronLayer_setupTests(void) ;
 
    void neuronLayer_teardownTests(void) ;

void outputErrorTests_A(void) ;

void calculateHiddenErrorTests_A(void);

acnfl_GenericFunctionResult x_squared(long long parameterCount, acnfl_NumberObject *parameterList) ;

#endif