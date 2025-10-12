/**
 * @file neuronLayer.c
 * 
 * Defines the neuron layer object and operations that act upon it.
 **/

#include "neuronLayer.h"
#include "../acnfl/acnfl_math.h"
#include "../acnfl/acnfl_derivative.h"
#include "datasetTypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/************************************************
 * 
 * DATA MANAGEMENT FUNCTIONS 
 *
 ************************************************/

/**
 * Initializes the memory for a neuronLayer_RegularLayer object.
 * All pointers are set to NULL at this stage and 
 * lengths are set to zero.
 *
 * DOES NOT INITIALIZE MEMORY FOR neuronLayer_RegularLayer objeect.
 *
 * @returns Error code (0 is normal). Only gives back one if *toBeInitialized is zero.
 */
int neuronLayer_initialize(neuronLayer_RegularLayer *toBeInitialized) {
    if (toBeInitialized == NULL) return 1;

    toBeInitialized->biasVector_pointer = NULL;
    toBeInitialized->weightMatrix_pointer = NULL;
    toBeInitialized->weightedInput_pointer = NULL;
    toBeInitialized->outputVector_pointer = NULL;

    toBeInitialized->weightMatrix_columns = 0;
    toBeInitialized->weightMatrix_rows = 0;

    return 0;
}


///TODO:   FIGURE OUT WHAT PARAMETERS THIS FUNCTION ACTUALLY NEEDS

/**
 * Initializes the correct amount of memory and dynamically allocates 
 * memory for neuronLayer_regular object object.
 *
 * @returns error code. 0 if finished successfuly, 1 if failed to allocate memory.
 **/
int neuronLayer_configure(neuronLayer_RegularLayer *toBeConfigured, long long int columns, long long int rows) {
    /** TODO: Optimization idea? Implement all of the seperate dynamically-allocated memory blocks here as one contigous block, where pointers simply reference offsets inside the block? Well, make sure that functions using neuronLayer_regularLayer actually work before trying this… */
    toBeConfigured->weightMatrix_columns = columns;
    toBeConfigured->weightMatrix_rows = rows;

    toBeConfigured->weightMatrix_pointer = malloc(sizeof(acnfl_NumberObject) * columns * rows);
    if (toBeConfigured->weightMatrix_pointer == NULL) {
        return 1;
    } 
    toBeConfigured->weightedInput_pointer = malloc(sizeof(acnfl_NumberObject) * toBeConfigured->WEIGHTEDINPUT_LENGTH);
    if (toBeConfigured->weightedInput_pointer == NULL) {
        return 1;
    }

    toBeConfigured->biasVector_pointer = malloc(sizeof(acnfl_NumberObject) * toBeConfigured->BIASVECTOR_LENGTH);
    if (toBeConfigured->biasVector_pointer == NULL) {
        return 1;
    }

    toBeConfigured->outputVector_pointer = malloc(sizeof(acnfl_NumberObject) * toBeConfigured->OUTPUTVECTOR_LENGTH );
    if (toBeConfigured->outputVector_pointer == NULL) {
        return 1;
    }


    return 0;
} 

/**
 * Deallocates memory for all pointers inside
 * neuronLayer_RegularLayer object.
 *
 * DOES NOT DEALLOCATE MEMORY FOR REGULARLAYER OBJECT ITSELF.
 * 
 * @returns Error code. 0 is normal
 **/
int  neuronLayer_destroy(neuronLayer_RegularLayer *toBeDestroyed) {
    free(toBeDestroyed->biasVector_pointer);
    free(toBeDestroyed->weightedInput_pointer);
    free(toBeDestroyed->weightMatrix_pointer);
    free(toBeDestroyed->outputVector_pointer);

    
    (toBeDestroyed->biasVector_pointer) = NULL;
    (toBeDestroyed->weightMatrix_pointer) = NULL;
    (toBeDestroyed->weightedInput_pointer)= NULL;
    (toBeDestroyed->outputVector_pointer) = NULL;

    toBeDestroyed->weightMatrix_columns=0;
    toBeDestroyed->weightMatrix_rows=0;

    return 0;
};



/************************************************
 * 
 * FEEDFORWARD (RegularLayer)
 *
 ************************************************/

/***
 * Calculates the linear combination of the Weight matrix, previous layer outpus
 * and bias vector for two neuronLayers. weightMatrix_pointer
 * The result is equivalent to
 * (aheadLayer.WeightMatrix * behindLayer.outputVector) +  
 * 
 * @param aheadLayer The regular layer whose linear combination is being calculated
 * @param behindLayer The regular laryer whose results are being used in the linear combination.
 * @returns A dynamically allocated array of acnfl_NumberObjects, count aheadLayer->weightMatrix. Don't forget to 
 * clean it up afterward. Returns NULL if error occured;
 ***/
acnfl_NumberObject* neuronLayer_linearCombinationCaluclate(neuronLayer_RegularLayer *behindLayer, neuronLayer_RegularLayer *aheadLayer) {
    if (aheadLayer == NULL || behindLayer == NULL) return NULL;

    //Validate that aheadLayer and behindLayer have the correct dimensions.
    if (aheadLayer->weightMatrix_columns != behindLayer->OUTPUTVECTOR_LENGTH) {
        return NULL; 
    }

    // Creates an array the size of aheadLayer->weightMatrix_rows
    acnfl_NumberObject *linearCombination = malloc(aheadLayer->BIASVECTOR_LENGTH * sizeof(acnfl_NumberObject));

    // Calculate value of aheadLayer->weightMatrix_pointer * behindLayer->outputVector_pointer
    /// TODO: Test out way to optimize this? This is a fairly common operation, so.
    for (long long int weightMatrix_rowIndex = 0; weightMatrix_rowIndex < aheadLayer->weightMatrix_rows; weightMatrix_rowIndex ++) {
        acnfl_NumberObject elementTotal = acnfl_generateApx(0, 0); 
        for (long long int outputVector_elementIndex = 0; outputVector_elementIndex < behindLayer->OUTPUTVECTOR_LENGTH; outputVector_elementIndex ++) {
            acnfl_add(elementTotal, 
                      acnfl_multiply(
                            neuronLayer_matrix_elementSelect(aheadLayer->weightMatrix_pointer, aheadLayer->weightMatrix_columns, weightMatrix_rowIndex, outputVector_elementIndex),
                          behindLayer->outputVector_pointer[outputVector_elementIndex]
                )     
            );
        }
        linearCombination[weightMatrix_rowIndex] = elementTotal;
    }

    // Calculate value of the prior product plus bias vector.
    for (long long int biasVector_index = 0; biasVector_index < aheadLayer->BIASVECTOR_LENGTH; biasVector_index++) {
        acnfl_add(linearCombination[biasVector_index], 
                    aheadLayer->biasVector_pointer[biasVector_index]);
    }

    return linearCombination;


}

/**
 * Feeds forward the results from one layer to the next in a neural network
 * @param neuronLayer_back Neuron layer that will be "feeding forward" into the next layer. The results for this layer should be calculated already
 * @param neuronLayer_forward Neuron layer that will be fed into. The weight matrix and bias vector should be initialized for this vector
 * @returns 0 if all good, 1 if did not complete, 2 if completed with error.
 */
 int layers_feedForward (neuronLayer_RegularLayer *neuronLayer_back,
    neuronLayer_RegularLayer *neuronLayer_forward) {
        if ((neuronLayer_back==NULL) || (neuronLayer_forward==NULL)) {
            return 1;
        }
        //Calculate linear combinations 
        // Apply activation function to weighted combinations
        {
            acnfl_NumberObject *combinations = neuronLayer_linearCombinationCaluclate(neuronLayer_back, neuronLayer_forward);
            if (combinations == NULL) {
                return 1;
            }
            for (long long i = 0; 
                 i < neuronLayer_forward->WEIGHTEDINPUT_LENGTH;
                 i++) {
                    // Apply linear combination
                    (neuronLayer_forward->weightedInput_pointer)[i] = combinations[i];
                    // Apply activation function  
                    acnfl_GenericFunctionResult activationResult = {.0, NULL}; 
                    if (neuronLayer_forward->activationFunction_pointer) {
                    activationResult = neuronLayer_forward->activationFunction_pointer(1, ((neuronLayer_forward->weightedInput_pointer)+i));}
                    
                    if (activationResult.results) {
                    (neuronLayer_forward->outputVector_pointer)[i]= activationResult.results[0];
                    ;
                    acnfl_freeHeldResults(&activationResult);
                    }
                 } 
            free(combinations);
        }
        return 0; 
    }

/**
 * Feeds forward the results for each layer in the neural network.
 * @param networkLayers Pointer to a list of network layers 
 * @param networkLayersLength How long networkLayers is 
 * @param inputdata Input data to the neural network
 * @return 0 if all good, 1 if couldn't complete operation, 2 if completed with errors
 **/
int layers_feedForwardNetwork(
    neuronLayer_RegularLayer *networkLayers,
    long long int networkLayersLength,
    layers_DataSetObject inputdata
) {
    if (networkLayersLength < 1) return 2;
    if (inputdata.dataSetType == LAYERS_DATASET_SINGULAR) {
        layers_DataSetSingular *redfinedInputdata = (layers_DataSetSingular*) inputdata.dataSet;
    //Hook up inputData to the neural network.
        neuronLayer_RegularLayer mockLayer = {
            .OUTPUTVECTOR_LENGTH = redfinedInputdata->length,
            .outputVector_pointer = redfinedInputdata->dataList
        };
        int result = layers_feedForward(&mockLayer, networkLayers);
        if (result == 1) return result;
    // Do mock first 
    } else {
        #ifdef REPORTING_3
            printf("FEEDFORWARD NETWORK: NOT COMPATABLE DATASET TYPE: %d", inputdata.dataSetType);
        #endif
        return 1;
    }
    //Go through loop 
    for (long long int i = 0; 0 < (networkLayersLength-1); i++) {
        int result = layers_feedForward(networkLayers+i, networkLayers+i+1);
        if (result == 1) return result;
    }

    
    return 0;
}

/************************************************
 * 
 * FEEDFORWARD (RegularLayer)
 *
 ************************************************/


/**********************
 * Calculates the error based on 
 * the output of a neural network
 * and the intended results.
 * @param outputLayer The final layer in a regular neural network.
 * @param intendedResult The "correct" result of the inputs from the training data
 * @param costFunction A pointer to a cost function used to evaluate the differecen between the outputLayer and intendedResult. A cost function, generally, should satisfy two conditions: 
 - It should be able to take in an arbitrary number of inputs, as long as that number is even. For a neural network that outputs n results, inputs 1..n to the function should be the actual results from the neural network, and inputs n+1..2n are the intended results of the neural network. The function should essentially take the first half of the inputs as the actual outputs of the neural network and the second half as the intended output of the neural network. The function should reject any calls with an odd number of functions.
 - It should return one output.
 TODO: MAKE SURE THIS WORKS. TESTING
 Costfunctions that don't satisfy these requirements may cause behavior or results you did not intend.
 
 * @returns A pointer to an array of acnfl_NumberObjects of length outputLayer.OUTPUTVECTOR_LENGTH. Dynamically allocated, so remember to clean up. Returns NULL if error occurs.
 **********************/
 acnfl_NumberObject* neuronLayer_calculateOutputError(
    neuronLayer_RegularLayer outputLayer,
    layers_DataSetObject intendedResult,
    acnfl_GenericFunctionDefinition costFunction
 ) {
    acnfl_NumberObject *toReturn;
    //Type of dataSetObject check...
    if (intendedResult.dataSetType == LAYERS_DATASET_SINGULAR) { 
        #define intendedResultAsSingular ((layers_DataSetSingular*)intendedResult.dataSet)
        //Make sure outputLayer.output and intendedResult are the same length
        if ((outputLayer.OUTPUTVECTOR_LENGTH == intendedResultAsSingular->length) == outputLayer.WEIGHTEDINPUT_LENGTH ) {
             toReturn = malloc(sizeof(acnfl_NumberObject)*outputLayer.OUTPUTVECTOR_LENGTH);
             if (toReturn == NULL) return toReturn;
            {
                acnfl_NumberObject combinedList[
                    outputLayer.WEIGHTEDINPUT_LENGTH +
                    outputLayer.OUTPUTVECTOR_LENGTH
                ];
                long long index =0;
                for (;
                     index < outputLayer.WEIGHTEDINPUT_LENGTH;
                     index++) {
                        combinedList[index]=outputLayer.weightedInput_pointer[index];
                }
                for (long long loopIndex; 
                     loopIndex < (outputLayer.OUTPUTVECTOR_LENGTH);
                     loopIndex++, index++){
                    combinedList[index]=outputLayer.outputVector_pointer[loopIndex];
                 }


                for (long long int index = 0;
                index < outputLayer.OUTPUTVECTOR_LENGTH; 
                index++){
                    // d_i ← f(a_i)
                    acnfl_GenericFunctionResult activationfunction_derivative = acnfl_derivative_default(
                        combinedList + index,
                        1, 1,  
                        outputLayer.activationFunction_pointer
                    ); /**  TODO: Change  to general acnfl_derivative function and add ways to adjust parameters globally.*/
                    // r_i = derivative of activation function by paramteter index
                    acnfl_GenericFunctionResult costfunction_derivative = acnfl_derivative_default(
                        combinedList,
                        outputLayer.WEIGHTEDINPUT_LENGTH + outputLayer.OUTPUTVECTOR_LENGTH,
                        index,
                        costFunction
                    ); /** TODO: Same as above, but also, given how most of the derivative results for activationFunction_derivative might be calculating the same things over and over, it's worth looking into this as a potential optimization route, if things come to that. */

                    // r_1 ← derivative of cost function * derivative of activation function
                    toReturn[index] = acnfl_multiply(activationfunction_derivative.results[0], costfunction_derivative.results[0]);

                    // Clean up memory for d_i
                    acnfl_freeHeldResults(&activationfunction_derivative);

                    // Clean up memory for r_i 
                    acnfl_freeHeldResults(&costfunction_derivative);
                }
                
            }
        } else {
            #ifdef REPORTING_3 
                printf("The outputLayer.output and intendedResult are not the same length: %lld!=%lld.", outputLayer.OUTPUTVECTOR_LENGTH, intendedResultAsSingular->length);
            #endif
            return NULL;
            
        }
    } else {
        #ifdef REPORTING_3 
            printf("Type of intendedResult is not specified. Type is %d.", intendedResult.dataSetType);
        #endif
        return NULL;
    }
    return toReturn;
 }
