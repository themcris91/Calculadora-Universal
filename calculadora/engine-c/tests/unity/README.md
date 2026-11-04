# Unity

Este directorio contiene una implementación mínima y compatible en API
(subconjunto: `UNITY_BEGIN/END`, `RUN_TEST`, `TEST_ASSERT*`) para poder correr los tests sin red.

Para usar Unity real (MIT, ThrowTheSwitch/Unity), reemplazá `unity.c`, `unity.h` y
agregá `unity_internals.h`. Los tests solo usan macros presentes en ambos.
