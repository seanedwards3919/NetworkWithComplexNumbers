
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

void neuronLayer_setup(void) {
    // NETWORK ONE!!! 
    neuronLayer_initialize(neuronLayers+0);
    neuronLayer_configure(neuronLayers+0, 2, 2);


}

void neuronLayer_teardown(void) {
    neuronLayer_destroy(neuronLayers+0);

}

void outputErrorTests_A(void) {
    
    // Set up needed variables


    return;
}