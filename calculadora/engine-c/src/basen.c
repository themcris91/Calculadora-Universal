#include "basen.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

int basen_parse(const char *str, int base, int32_t *result) {
    if (!str || !result || base < 2 || base > 36) return ENG_ERR_ARG;
    char *endptr;
    long val = strtol(str, &endptr, base);
    if (endptr == str) return ENG_ERR_ARG;
    *result = (int32_t)val;
    return ENG_OK;
}

int basen_format(int32_t value, int base, char *buffer, int buffer_size) {
    if (!buffer || base < 2 || base > 36) return ENG_ERR_ARG;
    if (base == 10) {
        if (snprintf(buffer, buffer_size, "%d", value) >= buffer_size) return ENG_ERR_RANGE;
    } else if (base == 16) {
        if (snprintf(buffer, buffer_size, "%X", (uint32_t)value) >= buffer_size) return ENG_ERR_RANGE;
    } else if (base == 8) {
        if (snprintf(buffer, buffer_size, "%o", (uint32_t)value) >= buffer_size) return ENG_ERR_RANGE;
    } else {
        char temp[64];
        int i = 0;
        uint32_t v = (uint32_t)value;
        const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        do {
            temp[i++] = digits[v % base];
            v /= base;
        } while (v > 0 && i < 63);
        if (value < 0 && base == 10) temp[i++] = '-';
        if (i >= buffer_size) return ENG_ERR_RANGE;
        for (int j = 0; j < i; j++) buffer[j] = temp[i - 1 - j];
        buffer[i] = '\0';
    }
    return ENG_OK;
}

int basen_convert(int32_t value, int from_base, int to_base, char *buffer, int buffer_size) {
    return basen_format(value, to_base, buffer, buffer_size);
}

int32_t basen_and(int32_t a, int32_t b) { return a & b; }
int32_t basen_or(int32_t a, int32_t b) { return a | b; }
int32_t basen_xor(int32_t a, int32_t b) { return a ^ b; }
int32_t basen_not(int32_t a) { return ~a; }
