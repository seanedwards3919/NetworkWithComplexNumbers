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
#include "../reporting/reporting_2.h"
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
    toBeInitialized->errorVector_pointer = NULL;
    
    toBeInitialized->activationFunction_pointer = NULL;

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

    toBeConfigured->biasVector_pointer = malloc(sizeof(acnfl_NumberObject) * toBeConfigured->BIASVECTOR_LENGTH);
    if (toBeConfigured->biasVector_pointer == NULL) {
        return 1;
    }
    toBeConfigured->weightMatrix_pointer = malloc(sizeof(acnfl_NumberObject) * columns * rows);
    if (toBeConfigured->weightMatrix_pointer == NULL) {
        return 1;
    } 
    toBeConfigured->weightedInput_pointer = malloc(sizeof(acnfl_NumberObject) * toBeConfigured->WEIGHTEDINPUT_LENGTH);
    if (toBeConfigured->weightedInput_pointer == NULL) {
        return 1;
    }
    toBeConfigured->outputVector_pointer = malloc(sizeof(acnfl_NumberObject) * toBeConfigured->OUTPUTVECTOR_LENGTH );
    if (toBeConfigured->outputVector_pointer == NULL) {
        return 1;
    }
    toBeConfigured->errorVector_pointer = malloc(sizeof(acnfl_NumberObject) * toBeConfigured->ERRORVECTOR_LENGTH);
    if (toBeConfigured->errorVector_pointer == NULL) {
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

    if (toBeDestroyed->biasVector_pointer != NULL){
    free(toBeDestroyed->biasVector_pointer);
    (toBeDestroyed->biasVector_pointer) = NULL;}

    if (toBeDestroyed->weightMatrix_pointer!= NULL){
    free(toBeDestroyed->weightMatrix_pointer);
    (toBeDestroyed->weightMatrix_pointer) = NULL;}

    
    if (toBeDestroyed->weightedInput_pointer!= NULL){
    free(toBeDestroyed->weightedInput_pointer);
    (toBeDestroyed->weightedInput_pointer)= NULL;}

    if (toBeDestroyed->outputVector_pointer!= NULL){
        free(toBeDestroyed->outputVector_pointer);
    (toBeDestroyed->outputVector_pointer)=NULL;}

    if (toBeDestroyed->errorVector_pointer!= NULL){
        free(toBeDestroyed->errorVector_pointer);
        (toBeDestroyed->errorVector_pointer) = NULL;
    }


    

    toBeDestroyed->weightMatrix_columns=0;
    toBeDestroyed->weightMatrix_rows=0;

    return 0;
}
/**
 * Sets all values to a valid zero value for the regularLayer object.
 **/
void neuronLayer_regularLayer_zeroOut(neuronLayer_RegularLayer zeroOutTarget) {
    if (zeroOutTarget.weightMatrix_pointer!=NULL) {
        for (long long i = 0; i < (zeroOutTarget.weightMatrix_columns*zeroOutTarget.weightMatrix_rows); i++) {
            zeroOutTarget.weightMatrix_pointer[i]=acnfl_generateApx(0, 0);
        }
    }
    if (zeroOutTarget.biasVector_pointer!=NULL) {
        for (long long i = 0; i < (zeroOutTarget.BIASVECTOR_LENGTH); i++) {
            zeroOutTarget.biasVector_pointer[i]=acnfl_generateApx(0, 0);
        }
    }
    if (zeroOutTarget.outputVector_pointer!=NULL) {
        for (long long i = 0; i < (zeroOutTarget.OUTPUTVECTOR_LENGTH); i++) {
            zeroOutTarget.outputVector_pointer[i]=acnfl_generateApx(0, 0);
        }
    }
    if (zeroOutTarget.weightedInput_pointer!=NULL) {
        for (long long i = 0; i < (zeroOutTarget.WEIGHTEDINPUT_LENGTH); i++) {
            zeroOutTarget.weightedInput_pointer[i]=acnfl_generateApx(0, 0);
        }
    }
    if (zeroOutTarget.errorVector_pointer!=NULL) {
        for (long long i = 0; i < (zeroOutTarget.ERRORVECTOR_LENGTH); i++) {
            zeroOutTarget.errorVector_pointer[i]=acnfl_generateApx(0, 0);
        }
    }
}


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
            elementTotal = acnfl_add(elementTotal, 
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
        linearCombination[biasVector_index]= acnfl_add(linearCombination[biasVector_index], 
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
    for (long long int i = 0; i < (networkLayersLength-1); i++) {
        int result = layers_feedForward(networkLayers+i, networkLayers+i+1);
        if (result == 1) return result;
    }

    
    return 0;
}

/************************************************
 * 
 * BACKPROPOGATION (RegularLayer)
 *
 ************************************************/


/**********************
 * Calculates the error based on 
 * the output of a neural network
 * and the intended results.
 * @param outputLayer The final layer in a regular neural network.
 * @param intendedResult The "correct" result of the inputs from the training data
 * @param costFunction A pointer to a cost function used to evaluate the differecen between the outputLayer and intendedResult. A cost function, generally, should satisfy two conditions: 
 - It should be able to take in an arbitrary high number of inputs, as long as that number is even. For a neural network that outputs n results, inputs 1..n to the function should be the actual results from the neural network, and inputs n+1..2n are the intended results of the neural network. The function should essentially take the first half of the inputs as the actual outputs of the neural network and the second half as the intended output of the neural network. The function should reject any calls with an odd number of functions.
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
        if ((outputLayer.OUTPUTVECTOR_LENGTH == intendedResultAsSingular->length) ) {
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
                for (long long loopIndex =0; 
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
                        1, 0,  
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

acnfl_NumberObject* neuronLayer_calculateHiddenError(neuronLayer_RegularLayer behindLayer, neuronLayer_RegularLayer forwardLayer) {
    // For convenience :
            // Transpose, so now number of rows is equal to number of columns
    long long int lengthOfA = forwardLayer.weightMatrix_columns;
    long long int lengthOfB = behindLayer.WEIGHTEDINPUT_LENGTH;
    if (lengthOfA != lengthOfB) {
        #ifdef REPORTING_3
            printf("\nNumber of columns in forwardLayer.weightMatrix != number of rows in behindLayer.weightMatrix. Returning null\n");
        #endif
        return NULL;
    }
    long long int lengthOfC = lengthOfA;
    long long int hiddenLayerAmount = lengthOfA+lengthOfB+lengthOfC;
    acnfl_NumberObject* toReturn = malloc(sizeof(acnfl_NumberObject) * hiddenLayerAmount);
    if (!toReturn) {
        #ifdef REPORTING_3
            printf("Couldn't allocate memory. Returning NULL\n");
        #endif
        return NULL;
    }
    
    acnfl_NumberObject *startOfA = toReturn,
                       *startOfB = toReturn+lengthOfA,
                       *startOfC = startOfB+lengthOfB;
    /** Calculate the product of (forwardLayer.weightMatrix)T * forwardLayer.errorVector = A */

    {
        acnfl_NumberObject sumHolder;
        for (long long columnCounter = 0; columnCounter < forwardLayer.weightMatrix_columns; columnCounter ++) {
            sumHolder = acnfl_generateApx(0, 0);
            for (long long rowCounter = 0; rowCounter < forwardLayer.weightMatrix_rows; rowCounter++) {
                sumHolder = acnfl_add(sumHolder,  acnfl_multiply(neuronLayer_matrix_elementSelect(forwardLayer.weightMatrix_pointer, forwardLayer.weightMatrix_columns, rowCounter, columnCounter), forwardLayer.errorVector_pointer[rowCounter]  ));
            }
            startOfA[columnCounter] = sumHolder;
        }
    }
    /** Calculate the result of the derivative of behindLayer's activation function on each element of behindLayer.weightedInput = B */
    {
        for (long long behindLayerIndex = 0; behindLayerIndex < behindLayer.WEIGHTEDINPUT_LENGTH; behindLayerIndex++) {
            acnfl_GenericFunctionResult results = acnfl_derivative_default((behindLayer.weightedInput_pointer+behindLayerIndex), 1, 0, behindLayer.activationFunction_pointer);
            if (results.results == NULL) {free(toReturn); return NULL;}
            startOfB[behindLayerIndex] = results.results[0];
            acnfl_freeHeldResults(&results);
        }
    }

    /** for a1, a2 ... in A and b1, b2, ... in B get c_i = a_i * b_i*/
    {
        for (long long index = 0; index < lengthOfA; index++) {
            startOfC[index] = acnfl_multiply(startOfA[index], startOfB[index]);
        }
        /// TODO: INCREDIBLY BAD WAY TO LAY OUT MEMORY
        /// REDUCE SIZE OF MALLOC
        for (long long index = 0; index < lengthOfA; index++) {
            toReturn[index] = startOfC[index];
        }
    }

    /** return */
    return toReturn;
}
/***
 * Deposits the new values for a neuronLayer_regularLayer's weight values into 
 * a NumberObject array.
 * @param main Layer whose new weight values will be calculated. Error values 
 * will be used in the algorithm. If there are null values within the 
 * main object, -1 will be returned
 *  


 * @param back Layer whose activations will be used in the algorithm. 
 * if Check that number of columns in main's column matrix is equal to number of elements in back's outputVector fails, returns -2
 * @param deposit Array to write into. If null, program returns 1
 * @param depositMaximum The length of deposit. If depositMaximum<1, returns 2. 
 * Function will write values up until it reaches the maximum specified by this 
 * variable. If it terminates before all values for new weight matrix have been 
 * written, returns 3.
 */
int neuronLayer_changeInWeights(neuronLayer_RegularLayer main, neuronLayer_RegularLayer back, acnfl_NumberObject *deposit, int depositMaximum, acnfl_NumberObject learningRate) 
{
    // Deposit  weights into deposit according to error times activation
    if (!deposit) 
        return 1;
    if ((!main.weightMatrix_pointer) || (!main.errorVector_pointer))
        return -1;
    if (depositMaximum<1)
        return 2;

    int indexCount = 0;
    if (main.weightMatrix_columns != back.OUTPUTVECTOR_LENGTH)
        return -2;
    acnfl_NumberObject 
        newValue[main.weightMatrix_rows * main.weightMatrix_columns],
        difference[main.weightMatrix_rows *main.weightMatrix_columns];

    for (int currentRow = 0; currentRow < main.weightMatrix_rows; currentRow ++) {
        for (int currentColumn; currentColumn < main.weightMatrix_columns; currentColumn++) {
            neuronLayer_matrix_elementSelect(difference, main.weightMatrix_columns, currentRow, currentColumn) = acnfl_multiply(back.outputVector_pointer[currentRow], main.errorVector_pointer[currentRow]);
            neuronLayer_matrix_elementSelect(newValue, main.weightMatrix_columns, currentRow, currentColumn) = acnfl_add(neuronLayer_matrix_elementSelect(difference, main.weightMatrix_columns, currentRow, currentColumn), neuronLayer_matrix_elementSelect(main.weightMatrix_pointer, main.weightMatrix_columns, currentRow, currentColumn));
            neuronLayer_matrix_elementSelect(deposit, main.weightMatrix_columns, currentRow, currentColumn) = neuronLayer_matrix_elementSelect(newValue, main.weightMatrix_columns, currentRow, currentColumn);
            neuronLayer_matrix_elementSelect(deposit, main.weightMatrix_columns, currentRow, currentColumn) =  acnfl_multiply(acnfl_divide(learningRate, acnfl_generateApx(main.ERRORVECTOR_LENGTH, 0)),neuronLayer_matrix_elementSelect(deposit, main.weightMatrix_columns, currentRow, currentColumn));
            indexCount++;
            if (indexCount >= depositMaximum)
                return 3;
        }
    }
    return 0;
}
/***
 * Deposits the new values for a neuronLayer_regularLayer's bias values into 
 * a NumberObject array.
 * @param main Layer whose new bias values will be calculated. Error values will be used in algorithm. 
 * If there are null values within the 
 * main object, -1 will be returned
 * @param deposit Array to write into. If null, program returns 1
 * @param depositMaximum The length of deposit. If depositMaximum<1, returns 2. 
 * Function will write values up until it reaches the maximum specified by this 
 * variable. If it terminates before all values for new weight matrix have been 
 * written, returns 3.
 **/
int neuronLayer_changeInBiases(neuronLayer_RegularLayer main, acnfl_NumberObject *deposit, int depositMaximum, acnfl_NumberObject learningRate){
    if (!deposit) 
        return 1;
    if (!main.biasVector_pointer) 
        return -1;
    if (depositMaximum<1)
        return 2;

    int indexCount = 0; 
    for (int elementIndex = 0; elementIndex < main.ERRORVECTOR_LENGTH; elementIndex++ ) {
        deposit[elementIndex] = acnfl_add(main.biasVector_pointer[elementIndex], main.biasVector_pointer[elementIndex]);
        deposit[elementIndex] = acnfl_multiply(acnfl_divide(learningRate, acnfl_generateApx(main.ERRORVECTOR_LENGTH, 0)),deposit[elementIndex]);
        indexCount ++;
        if (indexCount>=depositMaximum)
            return 3;
    }
    return 0;
} 
/**
 * Puts the new weight matrix and bias vector in a regularlayer object.
 *  
 * If this function reaches the maximum defined by newWeightMatrixLength or newBiasVectorLength without depositing all of the values for either, returns 1
 *
 * or needs to stop prematurely because of main length exceeds passed length, 
 * returns 2
 *
 * If both, returns 3
 *
 * if passed null, returns -1
 *
 * if null inside main object, returns -2
 * 
 **/
int neuronLayer_deposit(
    neuronLayer_RegularLayer main,
    acnfl_NumberObject *newWeightMatrix, int newWeightMatrixLength,
    acnfl_NumberObject *newBiasVector, int newBiasVectorLength
) {
    int returnValue = 0; 
    if ((!newWeightMatrix) || (!newBiasVector))
        return -1;
    if ((!main.weightMatrix_pointer) || (!main.biasVector_pointer))
        return -2;
     

    int index;
    for (index = 0; (index<(main.weightMatrix_columns*main.weightMatrix_rows))&&(index<newWeightMatrixLength); index++) {
        main.weightMatrix_pointer[index] = newWeightMatrix[index];
    }   
    if (index < (main.weightMatrix_columns*main.weightMatrix_rows))
        returnValue+=2;
    if (index < newWeightMatrixLength)
        returnValue +=1;
    

    for (index = 0; (index<main.BIASVECTOR_LENGTH)&&(index <newBiasVectorLength); index++) {
        main.biasVector_pointer[index] = newBiasVector[index];
    }

    if (index < newBiasVectorLength)
        {
            if ((returnValue != 1)&&(returnValue != 3))
                returnValue += 1;
        }
    if (index < main.BIASVECTOR_LENGTH)
    {
        if (returnValue < 2)
            returnValue +=2;
    }
    return returnValue;
}


/**
 * Backpropogates the results for each layer in the neural network, and 
 * deposits errors in error vectors.
 *  
 * @param networkLayers Pointer to a list of network layers 
 * @param networkLayersLength How long networkLayers is 
 * @param inputdata Input data to the neural network. Needed for changeInWeights
 * @param intendedResult The correct result of the inputs from the training data. For calculate output error.
 * @return 0 if all good, 1 if couldn't complete operation, 2 if completed with errors
 **/
int neuronLayer_backpropogateNetwork(
    neuronLayer_RegularLayer *network,
    long long int networkLayersLength,

    layers_DataSetObject intendedResult,
    acnfl_GenericFunctionDefinition costFunction
) {
    int returnValue = 0;
    if (networkLayersLength < 1) return 2;

    // perform backpropogation for last layer
    acnfl_NumberObject *errorResult = neuronLayer_calculateOutputError(network[networkLayersLength-1], intendedResult, costFunction);
    
    if (!errorResult) return 2;
    for (int i = 0; i < network[networkLayersLength-1].ERRORVECTOR_LENGTH; i++) {
        network[networkLayersLength-1].errorVector_pointer[i] = errorResult[i];
    }
    free(errorResult);

    // perform backpropogation for all other layers
    // for indecies networkLayersLength-2...0 : i
    for (long long index = networkLayersLength-2; index>=0; index--) {
        // result ← calculateHiddenError(i, i+1) 
        errorResult =  neuronLayer_calculateHiddenError(network[index], network[index+1]);
        // if (!result) return 2 
        if (!errorResult) return 2;
        // i.errorVector ← result
        for (int i = 0; i < network[index].ERRORVECTOR_LENGTH; i++)
            {network[index].errorVector_pointer[i] = errorResult[i];}
        // free(result)
        free(errorResult);
    }

    return returnValue;
}





int neuronLayer_updateNetwork(
    neuronLayer_RegularLayer *network,
    long long int networkLayersLength,

    layers_DataSetObject inputdata,
    acnfl_NumberObject learningRate
) {

    int returnValue = 0;

    if (networkLayersLength < 1) return 2;
    // Update on all other layers

    // for index networkLayersLength-1 ... 1 : i 
    for (int index = networkLayersLength-1; index>0; index-- ){
        // allocate buffer_a ; array w/ dimensions network[i].rows * network[i].columns
        acnfl_NumberObject buffer_a[network[index].weightMatrix_rows * network[index].weightMatrix_columns],
        // allocate buffer_b ; array w/ dimensions network[i].bias_length
            buffer_b[network[index].BIASVECTOR_LENGTH];
        // changeInWeights(network[i], network[i-1], buffer_a)
        #ifdef REPORTING_2
        int result=
        #endif
        neuronLayer_changeInWeights(network[index], network[index-1], buffer_a, network[index].weightMatrix_rows*network[index].weightMatrix_columns, learningRate);
        #ifdef REPORTING_2
        if (result!=0)
            printf("Result of changeInWeights was %d on index %d", result, index);
        #endif
        // changeInBiases(network[i], buffer_b)
        #ifdef REPORTING_2
        result =
        #endif
        neuronLayer_changeInBiases(network[index], buffer_b, network[index].BIASVECTOR_LENGTH, learningRate); 
        #ifdef REPORTING_2
        if (result!=0)
            printf("Result of changeInBiases was %d on index %d", result, index);
        #endif
        // neuronLayer_deposit( network[i], buffer_a, buffer_b)
        #ifdef REPORTING_2
        result =
        #endif
        neuronLayer_deposit(network[index], buffer_a, network[index].weightMatrix_rows*network[index].weightMatrix_columns, buffer_b, network[index].ERRORVECTOR_LENGTH);
        #ifdef REPORTING_2
        if (result!=0)
            printf("Result of network was %d on index %d", result, index);
        
        #endif
 
    }
    // Update on lastlayer

    if (inputdata.dataSetType == LAYERS_DATASET_SINGULAR) {
        layers_DataSetSingular *redfinedInputdata = (layers_DataSetSingular*) inputdata.dataSet;
    //Hook up inputData to the neural network.
        neuronLayer_RegularLayer mockLayer = {
            .OUTPUTVECTOR_LENGTH = redfinedInputdata->length,
            .outputVector_pointer = redfinedInputdata->dataList,
            .biasVector_pointer = NULL,
            .errorVector_pointer = NULL,
            .activationFunction_pointer = NULL,
            .weightedInput_pointer = NULL,
            .weightMatrix_pointer = NULL
        };

            // allocate buffer_a ; array w/ dimensions network[0].rows * network[0].columns
            // allocate buffer_b ; array w/ dimensions network[0].bias_length
            // changeInWeights(network[i], mockLayer, buffer_a)
            // changeInBiases(network[i], buffer_b)
            // neuronLayer_deposit( network[i], buffer_a, buffer_b)

        acnfl_NumberObject buffer_a[network[0].weightMatrix_rows * network[0].weightMatrix_columns],
        // allocate buffer_b ; array w/ dimensions network[i].bias_length
            buffer_b[network[0].BIASVECTOR_LENGTH];
        // changeInWeights(network[i], network[i-1], buffer_a)
        #ifdef REPORTING_2
        int result=
        #endif
        neuronLayer_changeInWeights(network[0], mockLayer, buffer_a, network[0].weightMatrix_rows*network[0].weightMatrix_columns, learningRate);
        #ifdef REPORTING_2
        if (result!=0)
            printf("Result of changeInWeights was %d on index %d", result, 0);
        #endif
        // changeInBiases(network[i], buffer_b)
        #ifdef REPORTING_2
        result =
        #endif
        neuronLayer_changeInBiases(network[0], buffer_b, network[0].BIASVECTOR_LENGTH, learningRate); 
        #ifdef REPORTING_2
        if (result!=0)
            printf("Result of changeInBiases was %d on index %d", result, 0);
        #endif
        // neuronLayer_deposit( network[i], buffer_a, buffer_b)
        #ifdef REPORTING_2
        result =
        #endif
        neuronLayer_deposit(network[0], buffer_a, network[0].weightMatrix_rows*network[0].weightMatrix_columns, buffer_b, network[0].ERRORVECTOR_LENGTH);
        #ifdef REPORTING_2
        if (result!=0)
            printf("Result of network was %d on index %d", result, 0);
        
        #endif
 


 
    }else {
        #ifdef REPORTING_3
            printf("FEEDFORWARD NETWORK: NOT COMPATABLE DATASET TYPE: %d", inputdata.dataSetType);
        #endif
        return 1;
    } 



        
    return returnValue;
}