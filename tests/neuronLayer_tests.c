
#include "../acnfl/acnfl_math.h"
#include "../acnfl/acnfl_derivative.h"
#include <stdlib.h>
#include "testHead.h"

#include "../layers/neuronLayer.h"
#include "../layers/datasetTypes.h"

#define REGULARLAYER_NUMBER 1
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



void neuronLayer_setupTests(void) {
    printf("Beginning neuronlayer tests.");
    // NETWORK ONE!!! 
    neuronLayer_initialize(neuronLayers+0);
    neuronLayer_configure(neuronLayers+0, 2, 2);
    neuronLayer_regularLayer_zeroOut(neuronLayers[0]);
    for (int i=0; i < (neuronLayers[0].weightMatrix_columns * neuronLayers[0].weightMatrix_rows); i++) {
        neuronLayers[0].weightMatrix_pointer[i] = acnfl_generateApx(1, 0);
    }
    neuronLayers[0].activationFunction_pointer = returnSumOfGiven;
    

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
    layers_feedForwardNetwork(neuronLayers+0, 1, datasets[0]);
    // Set up needed variables


    return;
}