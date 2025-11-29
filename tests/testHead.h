
/**
 * @file testHead.h
 * 
 * Header for testHead.c
 **/
#include <CUnit/CUError.h>
#include <CUnit/CUnit.h>

CU_ErrorCode test_HeadProcess(void);


/**Probably should've  defined these earlier.*/
#define ACNFL_testing_equal(a, b) CU_ASSERT_EQUAL(acnfl_defaultComparison(a, b, (acnfl_defaultComparisonInformation) {.opType='a', .leeway_apx = true, .epsilon_apx = 0.001}), 0)
#define gApx(a, b) acnfl_generateApx(a,b)


/** ONLY FOR USE WITHIN FUNCTIONS THAT RETURN acnfl_GenericFunctionResult */
#define acnfl_function_preliminaryCheck(parameterCount, minumumParameter, parameterList, allocatedResult) \
    if (parameterCount < minumumParameter) return (acnfl_GenericFunctionResult) {.resultsAmount = 0, .results = NULL};\
    acnfl_NumberObject* resultPointer = malloc(sizeof(acnfl_NumberObject)*allocatedResult);\
    if (resultPointer == NULL) return (acnfl_GenericFunctionResult) {.resultsAmount = 0, .results = NULL}; \
    acnfl_GenericFunctionResult toReturn = {.resultsAmount = allocatedResult, .results = resultPointer};

