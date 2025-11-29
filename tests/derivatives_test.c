
#include "../acnfl/acnfl_math.h"
#include "../acnfl/acnfl_derivative.h"
#include <stdlib.h>
#include "testHead.h"


/** ----------------------- **/
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
    acnfl_function_preliminaryCheck(parameterCount, 0, parameterList, parameterCount);
    for (int i = 0; i < parameterCount; i++) resultPointer[i]=acnfl_multiply(parameterList[i], parameterList[i]);
    return toReturn;
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

