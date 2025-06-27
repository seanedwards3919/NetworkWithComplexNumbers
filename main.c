/** @file main.c
 * 
 *
 */


#include <stdio.h>
#include "luaInterpret/lua.h"

/**
 * This is the main function for this program. We're just putting this here to set up Doxygen.
 */
int main() {
    int x =5;
    printf("Compilation test.\n");
    compilationTest(&x);
    printf("%d\n", x);


}