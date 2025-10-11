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

acnfl_NumberObject* neuronLayer_linearCombinationCaluclate(neuronLayer_RegularLayer *behindLayer, neuronLayer_RegularLayer *aheadLayer) ;
 acnfl_NumberObject* neuronLayer_calculateOutputError(
    neuronLayer_RegularLayer outputLayer,
    layers_DataSetObject intendedResult,
    acnfl_GenericFunctionDefinition costFunction);
#endif