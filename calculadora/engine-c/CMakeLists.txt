cmake_minimum_required(VERSION 3.10)
project(CalculadoraEngine C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(src)
include_directories(tests/unity)

set(SOURCES 
    src/status.c 
    src/numeric.c 
    src/finance.c 
    src/basen.c
    src/complexnum.c
    src/linalg.c
)

add_library(math_engine ${SOURCES})

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_numeric.c)
    add_executable(run_test_numeric tests/test_numeric.c tests/unity/unity.c)
    target_link_libraries(run_test_numeric math_engine m)
endif()

add_executable(run_test_finance tests/test_finance.c tests/unity/unity.c)
target_link_libraries(run_test_finance math_engine m)

add_executable(run_test_basen tests/test_basen.c tests/unity/unity.c)
target_link_libraries(run_test_basen math_engine m)

add_executable(run_test_complexnum tests/test_complexnum.c tests/unity/unity.c)
target_link_libraries(run_test_complexnum math_engine m)

add_executable(run_test_linalg tests/test_linalg.c tests/unity/unity.c)
target_link_libraries(run_test_linalg math_engine m)
