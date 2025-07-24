#include <CUnit/CUError.h>
#include <CUnit/CUnit.h>

CU_ErrorCode test_HeadProcess(void);


/**Probably should've  defined these earlier.*/
#define ACNFL_testing_equal(a, b) CU_ASSERT_EQUAL(acnfl_defaultComparison(a, b, (acnfl_defaultComparisonInformation) {.opType='a', .leeway_apx = true, .epsilon_apx = 0.0001}), 0)
#define gApx(a, b) acnfl_generateApx(a,b)