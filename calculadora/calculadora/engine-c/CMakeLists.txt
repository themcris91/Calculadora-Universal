cmake_minimum_required(VERSION 3.16)
project(engine C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Werror)

set(ENGINE_SOURCES src/api.c src/print.c src/status.c src/numeric.c)

if(EMSCRIPTEN)
  # Uso: emcmake cmake -S engine-c -B engine-c/build/cmake && cmake --build engine-c/build/cmake
  add_executable(api ${ENGINE_SOURCES})
  set_target_properties(api PROPERTIES
    SUFFIX ".js"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../web/public/wasm"
    LINK_FLAGS "-O3 --no-entry -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s MODULARIZE=1 -s EXPORT_ES6=1 -s ENVIRONMENT=web -s EXPORT_NAME=createEngine -s EXPORTED_RUNTIME_METHODS=cwrap"
  )
else()
  # Tests nativos: cmake -S engine-c -B engine-c/build/native [-DCOVERAGE=ON] && cmake --build engine-c/build/native && ctest --test-dir engine-c/build/native --output-on-failure
  option(COVERAGE "Instrumentar con --coverage" OFF)
  if(COVERAGE)
    add_compile_options(--coverage)
    add_link_options(--coverage)
  endif()
  enable_testing()
  add_library(engine STATIC ${ENGINE_SOURCES})
  target_include_directories(engine PUBLIC src)
  add_library(unity STATIC tests/unity/unity.c)
  target_include_directories(unity PUBLIC tests/unity)
  foreach(t test_sumar test_status test_numeric)
    add_executable(${t} tests/${t}.c)
    target_link_libraries(${t} engine unity m)
    add_test(NAME ${t} COMMAND ${t})
  endforeach()
endif()
