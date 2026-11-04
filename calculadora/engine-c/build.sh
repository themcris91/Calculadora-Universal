#!/bin/bash
set -e
echo "🔨 Compilando el motor C..."
emcc -std=c99 -O3 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s MODULARIZE=1 -s ENVIRONMENT=web \
  -s EXPORTED_FUNCTIONS="['_malloc','_free','_api_fin_npv','_api_la_det','_api_cx_rect_to_polar','_api_basen_convert','_api_num_integral_sin']" \
  -s EXPORTED_RUNTIME_METHODS="['HEAPF64','HEAP32','UTF8ToString','stringToUTF8']" \
  -Iengine-c/src \
  engine-c/src/*.c \
  -o web/public/wasm/api.js
echo "✅ Compilación exitosa."
