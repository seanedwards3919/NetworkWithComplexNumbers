#ifndef NEURONLAYER_H
#define NEURONLAYER_H
/**
 * @file neuronLayer.h
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

    //Weight matrix in row-major order
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

    //Error vectors
    acnfl_NumberObject *errorVector_pointer;
    /** the length of the error vector is the same as  the number of rows in the weight matrix. */
    #define ERRORVECTOR_LENGTH weightMatrix_rows

    //Activation function
    acnfl_GenericFunctionDefinition activationFunction_pointer;

    
} neuronLayer_RegularLayer;

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



int neuronLayer_initialize(neuronLayer_RegularLayer *toBeInitialized) ;
int neuronLayer_configure(neuronLayer_RegularLayer *toBeConfigured, long long int columns, long long int rows) ;
int  neuronLayer_destroy(neuronLayer_RegularLayer *toBeDestroyed);

int layers_feedForward (neuronLayer_RegularLayer *neuronLayer_back,
    neuronLayer_RegularLayer *neuronLayer_forward) ;
  int layers_feedForwardNetwork(
    neuronLayer_RegularLayer *networkLayers,
    long long int networkLayersLength,
    layers_DataSetObject inputdata
);

void neuronLayer_regularLayer_zeroOut(neuronLayer_RegularLayer zeroOutTarget);

acnfl_NumberObject* neuronLayer_linearCombinationCaluclate(neuronLayer_RegularLayer *behindLayer, neuronLayer_RegularLayer *aheadLayer) ;
 acnfl_NumberObject* neuronLayer_calculateOutputError(
    neuronLayer_RegularLayer outputLayer,
    layers_DataSetObject intendedResult,
    acnfl_GenericFunctionDefinition costFunction);
acnfl_NumberObject* neuronLayer_calculateHiddenError(neuronLayer_RegularLayer, neuronLayer_RegularLayer);
int neuronLayer_changeInWeights(neuronLayer_RegularLayer main, neuronLayer_RegularLayer back, acnfl_NumberObject *deposit, int depositMaximum); /** IMPLEMENT NEXT */
int neuronLayer_changeInBiases(neuronLayer_RegularLayer main, acnfl_NumberObject *deposit, int depositMaximum); /** IMPLEMENT NEXT */
int neuronLayer_deposit(
    neuronLayer_RegularLayer main,
    acnfl_NumberObject *newWeightMatrix, int newWeightMatrixLength,
    acnfl_NumberObject *newBiasVector, int newBiasVectorLength
);

int neuronLayer_backpropogateNetwork(
    neuronLayer_RegularLayer *network,
    long long int networkLayersLength,

    layers_DataSetObject intendedResult,
    acnfl_GenericFunctionDefinition costFunction
);
/** IMPLEMENT: int neuronLayer_updateWeights(neuronLayer_RegularLayer*, long long) CALL FROM INSIDE BACKPROPOGATE*/
    
#endif