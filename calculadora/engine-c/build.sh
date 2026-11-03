#!/usr/bin/env bash
# Compila el motor C99 a WebAssembly (requiere emcc en el PATH).
set -euo pipefail
cd "$(dirname "$0")"

SRC="src/api.c src/print.c"
WEB_OUT="../web/public/wasm"
TEST_OUT="build/test"
COMMON="-std=c99 -O3 -Wall -Wextra -Werror --no-entry \
  -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s MODULARIZE=1 -s EXPORT_ES6=1 \
  -s EXPORT_NAME=createEngine -s EXPORTED_RUNTIME_METHODS=cwrap"

command -v emcc >/dev/null || { echo "ERROR: emcc no encontrado. Activá emsdk (source emsdk_env.sh)." >&2; exit 1; }

mkdir -p "$WEB_OUT" "$TEST_OUT"

# Build web (el que consume la PWA)
# shellcheck disable=SC2086
emcc $SRC $COMMON -s ENVIRONMENT=web -o "$WEB_OUT/api.js"

# Build de test: mismo código, glue .mjs con soporte Node para Vitest
# shellcheck disable=SC2086
emcc $SRC $COMMON -s ENVIRONMENT=web,node -o "$TEST_OUT/api.mjs"

echo "OK -> $WEB_OUT/api.{js,wasm} y $TEST_OUT/api.{mjs,wasm}"
