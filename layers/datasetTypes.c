/** @file layerTypes.c
   
 * Defines types that will represent dataset types and 
 * functions to manage them.
  */

#include "datasetTypes.h"
#include "../reporting/reporting_3.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Generic function that erases the dataSet object w/in 
 * the layers_dataSetObject.
 * 
 * @param object the dataSetObject whose dataSet object you 
 * need to erase. 
 *
 * @return Error code. 0 if everything went succesfully.
 **/

int layers_generic_objectDestroy(layers_DataSetObject *object) {
  switch (object->dataSetType) {
    case LAYERS_DATASET_SINGULAR:
      layers_dataSetSingular_destroyObject(object->dataSet);
      object->dataSet = NULL;
      break;
    default:
      #ifdef REPORTING_3
        printf("dataSetType not recognized! Simply deallocating dataSet pointer as a fallback, but there may be unallocated memory left!!\n");
        printf("The dataSetType was %d\n", object->dataSetType);

        free(object->dataSet);
        object->dataSet = NULL;
      #endif
  }
  return 0;
}
 



/**
 * Initializes a layers_DataSetSingular object into memory
 * and returns a pointer. The length of this new object is 0. 
 * and dataList is not allocated yet (allocatedMemory == 0).
 * As always, check if this is NULL.
 * @return Pointer to new DataSetSingular object
 */
layers_DataSetSingular* layers_dataSetSingular_initializeMemory(void) {
  layers_DataSetSingular *toReturn = malloc(sizeof(layers_DataSetSingular));
  if (toReturn == NULL) return toReturn;

  toReturn->length =0;
  toReturn->allocatedMemory=0;
  toReturn->dataList = NULL;

  return toReturn;
}
;
/**
 * Frees all the memory taken up by a DataSetSingular object.
 * @param toBeDestroyed Pointer to a DataSetSingular object 
 * to be deallocated.
 * @return Error code. If this is zero, the operation executed succesfully
 * If 1, then error with toBeDestroyed
 */
int layers_dataSetSingular_destroyObject(layers_DataSetSingular *toBeDestroyed) {
  if (toBeDestroyed == NULL) {
    return 1;
  }

  if (toBeDestroyed->allocatedMemory != 0) {
    free(toBeDestroyed->dataList);
  }
  free(toBeDestroyed);

  return  0;

};
/**
 * Puts a new NumberObject into a DataSetSingular object.
 *
 * If dataList is  not allocated yet (allocatedMemory == 0), 
 * then list is allocated.
 *
 * If pushing new object onto memory would make it so length > allocatedMemory, then allocated memory is doubled
 * 
 * @param dataPoint The point to be pushed into the DataSetSingular.
 * 
 * @param dataSet The data set for the point to be pushed into.
 * 
 * @return Error code. 
 * - If 0: Executed successfully
 * - If 1: Memory allocation failed, and point not pushed.
 * - If 2: Error with dataSet
 */
int layers_dataSetSingular_pushDataPoint(acnfl_NumberObject dataPoint, layers_DataSetSingular *dataSet) {
    if (dataSet == NULL) return 2;

    // Check if adding 1 to length would make it so that length > allocatedMemory. If that's the case then length == allocatedMemory. 
    if (dataSet->length == dataSet->allocatedMemory) {
        // Check if there's any memory allocated; 
        // if there is ... 
        if (dataSet->length > 0) {
            // Reallocate memory 
            dataSet->dataList = realloc(dataSet->dataList,
                    ((dataSet->length)*2) * sizeof(acnfl_NumberObject));
                
            dataSet->allocatedMemory *= 2;
        }
        // if there isn't...
        else {
            dataSet->dataList = malloc(sizeof(acnfl_NumberObject));
            dataSet->allocatedMemory = 1;
        }
        if (dataSet->dataList == NULL) return 1;
    }

    *(dataSet->dataList + dataSet->length) = dataPoint;
    dataSet->length ++;

    return 0;
};

/**
 * Pushes multiple NumberObjects into a DataSetSingular object
 * using layers_dataSetSingular_pushDataPoint. 
 * 
 * @param dataPoints Pointer to list of datapoints tobe pushed into 
 * dataSet
 * 
 * @param length Length of list pointed to by dataPoints.
 *
 * @param dataSet The dataset for the points to be pushed into.
 *
 * @return Error code.
 * 
 * If 0: Executed succesfully 
 * 
 * If 1: Memory allocation failed, point not pushed
 * 
 * If 99: Error with list, point not push. 
 * 
 */
int layers_dataSetSingular_pushListOfDataPoints(acnfl_NumberObject *dataPoints, long long int length, layers_DataSetSingular *dataSet) {
  if (dataPoints == NULL) return 99;
  
  for (int i = 0; i < length; i++) {
    int result = layers_dataSetSingular_pushDataPoint(*(dataPoints + i),  dataSet);
    // If an error occured quit and return error code;
    if (result != 0) {
      return result;
    }
  }

  return 0;
}

