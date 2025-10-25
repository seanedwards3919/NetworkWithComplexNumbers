
#include "../acnfl/acnfl_math.h"
#include "../acnfl/acnfl_derivative.h"
#include <bits/types/locale_t.h>
#include <stdlib.h>
#include "testHead.h"

#include "../layers/neuronLayer.h"
#include "../layers/datasetTypes.h"

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