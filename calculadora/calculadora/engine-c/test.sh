#!/usr/bin/env bash
# Compila y corre todos los tests C en nativo. COVERAGE=1 instrumenta con gcov.
set -euo pipefail
cd "$(dirname "$0")"

CC="${CC:-cc}"
OUT=build/native
CFLAGS="-std=c99 -O0 -g -Wall -Wextra -Werror -Isrc -Itests/unity ${COVERAGE:+--coverage}"
mkdir -p "$OUT"

fail=0
for t in tests/test_*.c; do
  n=$(basename "$t" .c)
  echo "== $n"
  # shellcheck disable=SC2086
  $CC $CFLAGS "$t" tests/unity/unity.c src/*.c -lm -o "$OUT/$n"
  "$OUT/$n" || fail=1
done
exit $fail
