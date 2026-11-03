#ifndef STATUS_H
#define STATUS_H

/* Códigos de estado comunes a todos los módulos del motor.
 * 0 = éxito; negativo = error. Los resultados salen por punteros de salida. */
typedef enum {
    ENG_OK = 0,
    ENG_ERR_ARG = -1,         /* NULL, n fuera de rango, base inválida */
    ENG_ERR_DOMAIN = -2,      /* NaN/Inf, argumento fuera de dominio */
    ENG_ERR_NO_BRACKET = -3,  /* sin cambio de signo en el intervalo */
    ENG_ERR_NO_CONVERGE = -4, /* iteraciones/profundidad agotadas o divergencia */
    ENG_ERR_SINGULAR = -5,    /* matriz singular */
    ENG_ERR_RANGE = -6,       /* overflow de int32 */
    ENG_ERR_BUFFER = -7       /* buffer de salida insuficiente */
} eng_status;

/* Mensaje estático en español para un código (nunca NULL). */
const char *eng_status_str(int status);

#endif /* STATUS_H */
