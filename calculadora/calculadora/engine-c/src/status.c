#include "status.h"

const char *eng_status_str(int status)
{
    switch (status) {
    case ENG_OK:              return "ok";
    case ENG_ERR_ARG:         return "argumento invalido";
    case ENG_ERR_DOMAIN:      return "fuera de dominio (NaN/Inf)";
    case ENG_ERR_NO_BRACKET:  return "el intervalo no encierra una raiz (sin cambio de signo)";
    case ENG_ERR_NO_CONVERGE: return "no converge";
    case ENG_ERR_SINGULAR:    return "matriz singular";
    case ENG_ERR_RANGE:       return "fuera de rango (int32)";
    case ENG_ERR_BUFFER:      return "buffer insuficiente";
    default:                  return "estado desconocido";
    }
}
