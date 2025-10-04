/**
 * @file neuronLayer.c
 * 
 * Defines the neuron layer object and operations that act upon it.
 **/

#include "../acnfl/acnfl_math.h"
#include "../acnfl/acnfl_derivative.h"
#include "datasetTypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * Struct that holds all of the parameters for  one 
 * neural network layer.
 *
 * ---- WEIGHT MATRIX PARAMETERS
 *
 * @param weightMatrix_pointer
 * Pointer to a matrix of weight values. Upon initialization
 * and deinitialization this pointer should be NULL. For an  m x n
 * matrix (m rows, n columns), this is implemented as a pointer into
 * an dynamically allocated array of m*n count acnfl_NumberObjects.
 * @param weightMatrix_rows
 * the Number of rows in weightMatrix_pointer.
 * @param weightMatrix_columns 
 * the Number of columns in weightMatrix_pointer.
 *
 * ---- BIAS VECTOR PARAMETERS
 * @param biasVector_pointer
 * Pointer to a vector of bias values. Upon initialziation and 
 * deinitialization this pointer should be NULL. For an m-legnth 
 * vector, this is implemented as a poitner into a dynamically allocated
 * array of m acnfl_NumberObjects
 *
 * ---- WEIGHTED INPUT PARAMETERS
 * @param weightedInput_pointer
 * Pointer to a vector of bias values. upon initialization and deinitalization this pointer should be null. For an m-length vector, this is implimented as a pointer into a dynamically allocated array of m acnfl_NumberObjects
 *
 * ---- OUTPUT VECTOR PARAMETERS
 * @param outputVector_pointer
 * Pinter to a vector of output values. Upon initalization 
 * and deinitialization this pointer should be NULL. For an 
 * m-length vector, this is implemented as a poitner into a dynamically allocated
 * array of m acnfl_NumberObjects
 *
 * ---- ACTIVATION FUNCTION PARAMETERS
 * @param activationFunction_pointer Pointer to this layer's activation function.
 * Both the expected inputs and expected outputs should be one. TODO: Does this make sense...? Review math.
 *
 **/
typedef struct neuronLayer_regularLayer {

    //Weight matrix 
    acnfl_NumberObject *weightMatrix_pointer;
    long long int weightMatrix_rows;
    long long int weightMatrix_columns;

    //Bias vectors
    acnfl_NumberObject *biasVector_pointer;
    /** The length of the bias vector is the same as the number of rows in the weight matrix.*/
    #define BIASVECTOR_LENGTH weightMatrix_rows

    //Bias vectors
    acnfl_NumberObject *weightedInput_pointer;
    /** The length of the bias vector is the same as the number of rows in the weight matrix. */
    #define WEIGHTEDINPUT_LENGTH weightMatrix_rows


    //Output vectors
    acnfl_NumberObject *outputVector_pointer;
    /** The length of the output vector is the same as the number of rows in the weight matrix. */
    #define OUTPUTVECTOR_LENGTH weightMatrix_rows

    //Activation function
    acnfl_GenericFunctionDefinition activationFunction_pointer;

    
} neuronLayer_RegularLayer;


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
};


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
}; 

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


/***
 * Provides a way to let the user get a spesific element from a 
 * matrix at compile time.
 *
 * Unlike a normal mathematical matrix, the 
 * element of a matrix you want to return is are numbered starting 
 * at zero. However, you count the number of rows 
 * and columns are like normal still.
 *
 * @param matrix Pointer to a matrix array.
 * @param totalColums total columns in the matrix array.
 * @param targetRow Row of the element you want to retrieve.
 * @param targetColumn Column of the element you want to retrieve.
 ***/
#define neuronLayer_matrix_elementSelect(matrix, totalColumns, targetRow, targetColumn) *(matrix + ((targetRow * totalColumns) + targetColumn) ) 


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
                printf("The outputLayer.output and intendedResult are not the same length: %d!=%d.", outputLayer.OUTPUTVECTOR_LENGTH, intendedResultAsSingular->length);
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
