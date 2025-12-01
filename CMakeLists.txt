#Set user options:
set(DEBUG FALSE CACHE BOOL "Instructs the compiler to generate debug-ready code")

# Set requirements for compilation
cmake_minimum_required(VERSION 4.0)
#Raise warning level & debug 
if(MSVC)
  add_compile_options(ANNImp PRIVATE /W4 /WX)
else()
    if(DEBUG)
     add_compile_options(-Wall -Wextra -Wpedantic -Werror -Og -g -ggdb)
    else()
      
     add_compile_options(-Wall -Wextra -Wpedantic -Werror -O2)
    endif()
endif()


# Set up project
project(ANNImp)
add_library(ANNImp main.c)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

#set up C's math library  
find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
  target_link_libraries(ANNImp PUBLIC ${MATH_LIBRARY})
endif()


# Set up Lua script (Not Lua itself)
add_subdirectory(luaInterpret)
target_link_libraries(ANNImp PRIVATE luaInterpret)

# Set up acnfl_math 
add_subdirectory(acnfl)
target_link_libraries(ANNImp PRIVATE acnfl_math)

# Set up tests 
add_subdirectory(tests)
target_link_libraries(ANNImp PRIVATE tests)

#set up layers 
add_subdirectory(layers)
target_link_libraries(ANNImp PRIVATE acnfl_layers)

# Set up CUnit  (Only on linux.)
find_library(CUNIT_Library 
  NAMES libcunit.so libcunit.a )
find_path(CUNIT_Main 
  NAMES CUnit/CUnit.h )
  
find_path(CUNIT_DB
  NAMES CUnit/TestDB.h )
find_path(CUNIT_Whole
  NAMES CUnit)
  
include_directories(${CUNIT_Main} ${CUNIT_DB} ${CUNIT_Whole})
target_link_libraries(ANNImp PUBLIC ${CUNIT_Library})