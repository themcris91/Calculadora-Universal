cmake_minimum_required(VERSION 3.16)
project(engine C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Werror)

set(ENGINE_SOURCES src/api.c src/print.c)

if(EMSCRIPTEN)
  # Uso: emcmake cmake -S engine-c -B engine-c/build/cmake && cmake --build engine-c/build/cmake
  add_executable(api ${ENGINE_SOURCES})
  set_target_properties(api PROPERTIES
    SUFFIX ".js"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../web/public/wasm"
    LINK_FLAGS "-O3 --no-entry -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s MODULARIZE=1 -s EXPORT_ES6=1 -s ENVIRONMENT=web -s EXPORT_NAME=createEngine -s EXPORTED_RUNTIME_METHODS=cwrap"
  )
else()
  # Tests unitarios nativos: cmake -S engine-c -B engine-c/build/native && cmake --build engine-c/build/native && ctest --test-dir engine-c/build/native
  enable_testing()
  add_library(engine STATIC ${ENGINE_SOURCES})
  add_executable(test_sumar tests/test_sumar.c)
  target_link_libraries(test_sumar engine m)
  add_test(NAME test_sumar COMMAND test_sumar)
endif()
