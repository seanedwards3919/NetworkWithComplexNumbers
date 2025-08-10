/**
 * @file neuronLayer.c
 * 
 * Defines the neuron layer object and operations that act upon it.
 **/

#include "../acnfl/acnfl_math.h"
#include <stdlib.h>

/**
 * Struct that holds all of the parameters for  one 
 * neural network layer.
 *
 * ---- WEIGHT MATRIX PARAMETERS
 *
 * @param weightMatrix_pointer
 * Pointer to a matrix of weight values. Upon initialization
 * and deinitialization this pointer should be NULL. For an  m x n
 * matrix (m rows, n columns), this is implemented as a pointer to
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
 * vector, this is implemented as a poitner to a dynamically allocated
 * array of m acnfl_NumberObjects
 *
 *
 * ---- OUTPUT VECTOR PARAMETERS
 * @param outputVector_pointer
 * Pinter to a vector of output values. Upon initalization 
 * and deinitialization this pointer should be NULL. For an 
 * m-length vector, this is implemented as a poitner to a dynamically allocated
 * array of m acnfl_NumberObjects
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

    //Output vectors
    acnfl_NumberObject *outputVector_pointer;
    /** The length of the output vector is the same as the number of rows in the weight matrix. */
    #define OUTPUTVECTOR_LENGTH weightMatrix_rows

    
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
    toBeConfigured->weightMatrix_columns = columns;
    toBeConfigured->weightMatrix_rows = rows;

    toBeConfigured->weightMatrix_pointer = malloc(sizeof(acnfl_NumberObject) * columns * rows);
    if (toBeConfigured->weightMatrix_pointer == NULL) return 1;

    toBeConfigured->biasVector_pointer = malloc(sizeof(acnfl_NumberObject) * toBeConfigured->BIASVECTOR_LENGTH);
    if (toBeConfigured->biasVector_pointer == NULL) return 1;

    toBeConfigured->outputVector_pointer = malloc(sizeof(acnfl_NumberObject) * toBeConfigured->OUTPUTVECTOR_LENGTH );
    if (toBeConfigured->outputVector_pointer == NULL) return 1;

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
    free(toBeDestroyed->weightMatrix_pointer);
    free(toBeDestroyed->outputVector_pointer);

    
    (toBeDestroyed->biasVector_pointer) = NULL;
    (toBeDestroyed->weightMatrix_pointer) = NULL;
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
acnfl_NumberObject* linearCombinationCaluclate(neuronLayer_RegularLayer *behindLayer, neuronLayer_RegularLayer *aheadLayer) {
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

