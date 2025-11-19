
#include "../acnfl/acnfl_math.h"
#include "../acnfl/acnfl_derivative.h"
#include <bits/types/locale_t.h>
#include <stdlib.h>
#include "testHead.h"

#include "../layers/neuronLayer.h"
#include "../layers/datasetTypes.h"
#include "tests.h"

#define REGULARLAYER_NUMBER 5
neuronLayer_RegularLayer neuronLayers[REGULARLAYER_NUMBER];
#define DATASETOBJECT_NUMBER 1
layers_DataSetObject datasets[DATASETOBJECT_NUMBER];
#define DATASETSINGULAR_NUMBER 1
layers_DataSetSingular datasetsSingular[DATASETSINGULAR_NUMBER];

acnfl_GenericFunctionResult returnSumOfGiven(long long parameterCount, acnfl_NumberObject *parameterList) {
    acnfl_function_preliminaryCheck(parameterCount, 0, parameterList, 1);
        resultPointer[0] = acnfl_generateApx(0, 0);
        for (int i = 0; i < parameterCount; i++) {
            resultPointer[0] = acnfl_add(resultPointer[0], parameterList[i]);            
        }
 
        return toReturn;
}

acnfl_GenericFunctionResult giveInverse(long long parameterCount, acnfl_NumberObject *parameterList) {
    acnfl_function_preliminaryCheck(parameterCount, 0, parameterList, parameterCount);
    for (int i = 0; i < parameterCount; i++) resultPointer[i]=acnfl_multiply(acnfl_generateApx(-1, 0), parameterList[i]);
    return toReturn;
}


acnfl_GenericFunctionResult giveHalf(long long parameterCount, acnfl_NumberObject *parameterList) {
    acnfl_function_preliminaryCheck(parameterCount, 0, parameterList, parameterCount);
    for (int i = 0; i < parameterCount; i++) resultPointer[i]=acnfl_multiply(acnfl_generateApx(0.5, 0), parameterList[i]);
    return toReturn;
}

void neuronLayer_setupTests(void) {
    #define initializeNeuronLayerObject(location, columns, rows, pointer) neuronLayer_initialize(neuronLayers+location); \
    neuronLayer_configure(neuronLayers+location, columns, rows); \
    neuronLayer_regularLayer_zeroOut(neuronLayers[location]); \
    neuronLayers[location].activationFunction_pointer = pointer

    printf("Beginning neuronlayer tests.");
    // NETWORK ONE!!! 
    initializeNeuronLayerObject(0, 2, 2, returnSumOfGiven);
    for (int i=0; i < (neuronLayers[0].weightMatrix_columns * neuronLayers[0].weightMatrix_rows); i++) { // Weight matrix values;
        neuronLayers[0].weightMatrix_pointer[i] = acnfl_generateApx(1, 0);
    }

    //NETWORK TWO!!! 
    initializeNeuronLayerObject(1, 2, 3, returnSumOfGiven);
    for (int i=0; i < (neuronLayers[1].weightMatrix_columns * neuronLayers[1].weightMatrix_rows); i++) { // Weight matrix values;
        neuronLayers[1].weightMatrix_pointer[i] = acnfl_generateApx(1, 0);
    }

    //Network three!!!
    initializeNeuronLayerObject(2, 3, 2, returnSumOfGiven);
    neuronLayer_matrix_elementSelect(neuronLayers[2].weightMatrix_pointer, neuronLayers[2].weightMatrix_columns, 0, 0) = acnfl_generateApx(0.25, 0);
    neuronLayer_matrix_elementSelect(neuronLayers[2].weightMatrix_pointer, neuronLayers[2].weightMatrix_columns, 0, 1) = acnfl_generateApx(0.25, 0);
    neuronLayer_matrix_elementSelect(neuronLayers[2].weightMatrix_pointer, neuronLayers[2].weightMatrix_columns, 0, 2) = acnfl_generateApx(0.25, 0);
    neuronLayer_matrix_elementSelect(neuronLayers[2].weightMatrix_pointer, neuronLayers[2].weightMatrix_columns, 1, 0) = acnfl_generateApx(-1, 0);
    neuronLayer_matrix_elementSelect(neuronLayers[2].weightMatrix_pointer, neuronLayers[2].weightMatrix_columns, 1, 1) = acnfl_generateApx(0.5, 0);
    neuronLayer_matrix_elementSelect(neuronLayers[2].weightMatrix_pointer, neuronLayers[2].weightMatrix_columns, 1, 2) = acnfl_generateApx(0, 0);

    //Network four!!!
    initializeNeuronLayerObject(3, 2, 2, giveInverse);
    neuronLayer_matrix_elementSelect(neuronLayers[3].weightMatrix_pointer, neuronLayers[3].weightMatrix_columns, 0, 0) = acnfl_generateApx(1, 0);
    neuronLayer_matrix_elementSelect(neuronLayers[3].weightMatrix_pointer, neuronLayers[3].weightMatrix_columns, 0, 1) = acnfl_generateApx(0, 0);
    neuronLayer_matrix_elementSelect(neuronLayers[3].weightMatrix_pointer, neuronLayers[3].weightMatrix_columns, 1, 0) = acnfl_generateApx(0, 0);
    neuronLayer_matrix_elementSelect(neuronLayers[3].weightMatrix_pointer, neuronLayers[3].weightMatrix_columns, 1, 1) = acnfl_generateApx(1, 0);

    //Network five!!!!
    initializeNeuronLayerObject(4, 2, 4, giveHalf);
    neuronLayer_matrix_elementSelect(neuronLayers[4].weightMatrix_pointer, neuronLayers[4].weightMatrix_columns,
                                        0, 0) = acnfl_generateApx(2, 0);
    neuronLayer_matrix_elementSelect(neuronLayers[4].weightMatrix_pointer, neuronLayers[4].weightMatrix_columns,
                                        0, 1) = acnfl_generateApx(5, 0);
                                        
    neuronLayer_matrix_elementSelect(neuronLayers[4].weightMatrix_pointer, neuronLayers[4].weightMatrix_columns,
                                        1, 0) = acnfl_generateApx(1, 0);
    neuronLayer_matrix_elementSelect(neuronLayers[4].weightMatrix_pointer, neuronLayers[4].weightMatrix_columns,
                                        1, 1) = acnfl_generateApx(-0.5, 0);

    neuronLayer_matrix_elementSelect(neuronLayers[4].weightMatrix_pointer, neuronLayers[4].weightMatrix_columns,
                                        2, 0) = acnfl_generateApx(6, 0);
    neuronLayer_matrix_elementSelect(neuronLayers[4].weightMatrix_pointer, neuronLayers[4].weightMatrix_columns,
                                        2, 1) = acnfl_generateApx(2, 0);

    neuronLayer_matrix_elementSelect(neuronLayers[4].weightMatrix_pointer, neuronLayers[4].weightMatrix_columns,
                                        3, 0) = acnfl_generateApx(7, 0);
    neuronLayer_matrix_elementSelect(neuronLayers[4].weightMatrix_pointer, neuronLayers[4].weightMatrix_columns,
                                        3, 1) = acnfl_generateApx(10, 0);
    // DATASET ONE !!!
    datasets[0].dataSetType = LAYERS_DATASET_SINGULAR;
    datasets[0].dataSet = datasetsSingular+0;
    layers_dataSetSingular_initializeMemory(datasetsSingular+0);
    layers_dataSetSingular_pushListOfDataPoints(
        (acnfl_NumberObject[]) {
            acnfl_generateApx(-1, 0), acnfl_generateApx(5, 0)
        }, 2, datasetsSingular+0);


}

void neuronLayer_teardownTests(void) {
    neuronLayer_destroy(neuronLayers+0);
    free(datasetsSingular[0].dataList);

}

void outputErrorTests_A(void) {
    
    printf("Beginning neuronlayer tests A.");
    layers_feedForwardNetwork(neuronLayers+0, 5, datasets[0]);
    // Set up needed variables

    //Network one results
    ACNFL_testing_equal(neuronLayers[0].outputVector_pointer[0],acnfl_generateApx(4, 0));
    ACNFL_testing_equal(neuronLayers[0].outputVector_pointer[1],acnfl_generateApx(4, 0));
    //Network two results
    
    ACNFL_testing_equal(neuronLayers[1 ].outputVector_pointer[0],acnfl_generateApx(8, 0));
    ACNFL_testing_equal(neuronLayers[1].outputVector_pointer[1],acnfl_generateApx(8, 0));
    ACNFL_testing_equal(neuronLayers[1].outputVector_pointer[2],acnfl_generateApx(8, 0));
    //Network three
    ACNFL_testing_equal(neuronLayers[2].outputVector_pointer[0],acnfl_generateApx(6, 0));
    ACNFL_testing_equal(neuronLayers[2].outputVector_pointer[1],acnfl_generateApx(-4, 0));
    //Network four 
    ACNFL_testing_equal(neuronLayers[3].outputVector_pointer[0],acnfl_generateApx(-6, 0));
    ACNFL_testing_equal(neuronLayers[3].outputVector_pointer[1],acnfl_generateApx(4, 0));
    //Network five
    ACNFL_testing_equal(neuronLayers[4].outputVector_pointer[0],acnfl_generateApx(4, 0));
    ACNFL_testing_equal(neuronLayers[4].outputVector_pointer[1],acnfl_generateApx(-4, 0));
    ACNFL_testing_equal(neuronLayers[4].outputVector_pointer[2],acnfl_generateApx(14, 0));
    ACNFL_testing_equal(neuronLayers[4].outputVector_pointer[3],acnfl_generateApx(-1, 0));

    return;
}

void calculateHiddenErrorTests_A(void) {
    /**
    * Test 1 
    * 
    * Activation function is just 
    * f(x) = x^2
    *
    * Weighted inputs for backLayer are [1, 2]T, so derivative
    * should be [2, 4]T
    *
    * forwardLayer weight matrix is 
    *   [2, 3]
    *   [4, 5]
    * So transposed, it should be
    *   [2, 4]
    *   [3, 5]
    * And the error for forwardLayer should be 
    *   [-2, 0]T
    * Therefore the matrix multiplication, wT * error should be 
    *   [-4, -6]T
    * Final product from the Hadamard product should be
    *   [-8, -24]
    **/

    neuronLayer_RegularLayer 
        backLayer = {
            .weightedInput_pointer = (acnfl_NumberObject[])
                {acnfl_generateApx(2, 0),
                 acnfl_generateApx(4, 0)},
            .WEIGHTEDINPUT_LENGTH = 2,
            .activationFunction_pointer = x_squared
        },
        forwardLayer = {
            .weightMatrix_pointer = (acnfl_NumberObject[])
                {acnfl_generateApx(2, 0), acnfl_generateApx(3,0),
                acnfl_generateApx(4, 0), acnfl_generateApx(5, 0)},
            .weightMatrix_rows = 2,
            .weightMatrix_columns = 2, 

            .errorVector_pointer = (acnfl_NumberObject[])
                {acnfl_generateApx(-2, 0),
                 acnfl_generateApx(0, 0)},
        };
    acnfl_NumberObject intendedResult[] = 
        {acnfl_generateApx(-8, 0),
         acnfl_generateApx(-24, 0)};
    int intendedResultLength = 2;

    acnfl_NumberObject *result = neuronLayer_calculateHiddenError(backLayer, forwardLayer);
    if(!result){
        for (int i = 0; i < intendedResultLength; i++) {
            ACNFL_testing_equal(result[i], intendedResult[i])
        }
        free(result);
    }

    /**
    * Test 2 
    *
    * Activation function is just 
    * f(x)= x*(-1), so all derivatives, are -1
    * 
    * Weighted inputs for backLayer are 3 long, and can be anything 
    * but the derivative is always going to be [-1, -1, -1]T.
    *
    * forwardLayer weight matrix is
    *  [1, -5, 9]
    *  [-2, 6, -10]
    *  [3, -7, 11]
    *  [-4, 8, -12]
    * 
    * So transposed, it should be 
    * [1, -2, 3, -4]
    * [-5, 6, -7, 8]
    * [9, -10, 11, -12]
    *
    * And the error for frwardLayer should be 
    * [-1, -100, 20.5, 2]T
    * 
    * Therefore the matrix multiplication, wT * error should be 
    * [252.5, -722.5, 1192.5]T
    *
    * And final product from the Hadamard product should be 
    * [-252.5, 722.5, -1192.5]T
    **/

    /**
 * Test 3
 * 
 * Activation function is just 
 * f(x) = sqrt(x), so the derivative is f'(x) = 1 / (2 * sqrt(x))
 * 
 * Weighted inputs for backLayer are [4, 16, 25]T, so the derivatives
 * should be [1/4, 1/8, 1/10]T, or approximately [0.25, 0.125, 0.1]T
 * 
 * forwardLayer weight matrix is
 *   [0.5, -1.5, 2.5]
 *   [-2.5, 3.5, -4.5]
 * So transposed, it should be
 *   [0.5, -2.5]
 *   [-1.5, 3.5]
 *   [2.5, -4.5]
 * 
 * And the error for forwardLayer should be
 *   [1, -10]T
 * 
 * Therefore the matrix multiplication, wT * error should be
 *   [25.5, -36.5, 47.5]
 * 
 * Final product from the Hadamard product should be
 *   [6.375, -4.5625, 4.75]
 **/

 /**
 * Test 4
 * 
 * Activation function is just 
 * f(x) = log(x), so the derivative is f'(x) = 1/x
 * 
 * Weighted inputs for backLayer are [1, 10]T, so the derivatives
 * should be [1/1, 1/10]T, or [1, 0.1]T
 * 
 * forwardLayer weight matrix is
 *   [0.5, 1]
 *   [1.5, 2]
 * So transposed, it should be
 *   [0.5, 1.5]
 *   [1, 2]
 * 
 * And the error for forwardLayer should be
 *   [2, 2]T
 * 
 * Therefore the matrix multiplication, wT * error should be
 *   [0.5 * 2 + 1.5 * 2, 
 *    1 * 2 + 2 * 2]T
 *   = [1 + 3, 2 + 4]T
 *   = [4, 6]T
 * 
 * Final product from the Hadamard product should be
 *   [4 * 1, 6 * 0.1]T
 *   = [4, 0.6]T
 * 
 * To make the final Hadamard product result equal to [2, 2]T, we need to
 * scale the matrix multiplication result or modify the error vector accordingly.
 **/

 /**
  * No. 5 
  * Activation function is just f(x)= x^2, so derivative is 
  * f'(x)=2x. So weighted inputs for BackLayer are [1+2i, 10-3i]T, 
  * so derivatives should be [2+4i, 20-6i].

  * Forwardlayer weight matrix is 
 *   [0.5 + 1i, 1 - 2i]
 *   [1.5 + 0.5i, 2 + 3i]
 * So transposed, it should be
 *   [0.5 + 1i, 1.5 + 0.5i]
 *   [1 - 2i, 2 + 3i]
   * 
 * And the error for forwardLayer should be
 *   [2 + 3i, -1 + 4i]T
 * The matrix multiplication wT * error should be:
 *   [-5.5+9i, -6+4i]
 * Therefore the hadamard product is 
 *   [-47-4i, -48+58i]
  **/
}