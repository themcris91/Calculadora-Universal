#ifndef BASEN_H
#define BASEN_H

#include <stdint.h>
#include "status.h"

int basen_parse(const char *str, int base, int32_t *result);
int basen_format(int32_t value, int base, char *buffer, int buffer_size);
int basen_convert(int32_t value, int from_base, int to_base, char *buffer, int buffer_size);
int32_t basen_and(int32_t a, int32_t b);
int32_t basen_or(int32_t a, int32_t b);
int32_t basen_xor(int32_t a, int32_t b);
int32_t basen_not(int32_t a);

#endif
