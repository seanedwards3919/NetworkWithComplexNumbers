/** @file lua.c 
 * 
 */

#include <stdio.h>
void compilationTest(int *x) {
    printf("Compilation test: %d\n", *x);
    *x = (*x + 1);
    return; 
}