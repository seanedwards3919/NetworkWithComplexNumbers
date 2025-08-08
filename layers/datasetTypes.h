/** 
 * @file 
 * Defines types that will represent dataset types and 
 * functions to manage them. 
 **/

 #ifndef LAYERTYPES_H
 #define LAYERTYPES_H

 #include "../acnfl/acnfl_math.h" // Defines acnfl_NumberObject
 /*********************************************************
  * DATASET STRUCTURES AND FUNCTIONS
  **********************************************************/
 /**
  * Polymorphic meta-container for all sorts of dataSet 
  * objects, if they would be needed in the future.
  * 
  * @param dataSetType Indicates what kind of data set object
  * is pointed to by dataSet 
  * 
  * @param dataSet Pointer to the actual dataSet itself. 
  */
 typedef struct layers_dataSetObject {
    int dataSetType;
    void* dataSet;
 } layers_DataSetObject;

/**
 * Generic function that erases the dataSet object w/in 
 * the layers_dataSetObject.
 * 
 * @param object the dataSetObject whose dataSet object you 
 * need to erase. 
 *
 * @return Error code. 0 if everything went succesfully.
 **/

 int layers_generic_objectDestroy(layers_DataSetObject *object);





 /// The enum for a singularDataset struct, used in DataSetObject.dataSetType
 #define LAYERS_DATASET_SINGULAR 1

typedef long long int layers_lengthObject;
/**
  * Structure that holds a singular "row" of data made up of 
  * acnfl_NumberObject objects
  *
  * @param length The length of the data. How many data points are 
  *  pointed to by this struct?
  * 
  * @param allocatedMemory The amount of memory allocated by the 
  * struct. This may or may not be the same as length. Not to be 
  * modified by the user. 
  *
  * @param dataList Pointer into a list of data points. These are expected to be ordered correctly when used.
  **/
typedef struct layers_dataSetSingular {
    layers_lengthObject length; 
    layers_lengthObject allocatedMemory; 
    /** TODO: These might need to be expanded to an arbitrary-length integer.*/
    acnfl_NumberObject *dataList;
} layers_DataSetSingular;

/**
 * Initializes a layers_DataSetSingular object into memory
 * and returns a pointer. The length of this new object is 0. 
 * and dataList is not allocated yet (allocatedMemory == 0).
 * As always, check if this is NULL.
 * @return Pointer to new DataSetSingular object
 */
layers_DataSetSingular* layers_dataSetSingular_initializeMemory(void);
/**
 * Frees all the memory taken up by a DataSetSingular object.
 * @param toBeDestroyed Pointer to a DataSetSingular object 
 * to be deallocated.
 * @return Error code. If this is zero, the operation executed succesfully
 */
int layers_dataSetSingular_destroyObject(layers_DataSetSingular *toBeDestroyed);
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
 *
 *  If 0: Executed successfully.
 *
 *  If 1: Memory allocation failed, and point not pushed.
 */
int layers_dataSetSingular_pushDataPoint(acnfl_NumberObject dataPoint, layers_DataSetSingular *dataSet);

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
 * If 2: Error with list, point not push. 
 * 
 */
int layers_dataSetSingular_pushListOfDataPoints(acnfl_NumberObject *dataPoints, long long int length, layers_DataSetSingular *dataSet);


#endif