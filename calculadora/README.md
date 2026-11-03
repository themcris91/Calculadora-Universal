# Calculadora Universal — Fase 0

Motor C99 → WebAssembly → PWA offline-first (React + TS + Vite + Workbox + KaTeX).

## Requisitos
- Emscripten (emsdk) activado: `emcc --version`
- Node ≥ 20 (`nvm use`)

## Comandos
```bash
./engine-c/build.sh        # genera web/public/wasm/api.{js,wasm} y engine-c/build/test/api.{mjs,wasm}
npm install                # raíz (Vitest)
cd web && npm install && npm run dev
```
Producción: `cd web && npm run build && npm run preview` (el SW solo se registra en build).

## Verificar PWA offline
1. `npm run build && npm run preview`; abrir la URL con conexión y recargar una vez.
2. DevTools → Application → Service Workers: estado *activated*.
3. Marcar **Offline** y recargar: la app abre; "Sumar" e "Integral de sin" responden.
4. Lighthouse → Progressive Web App: instalable.

## Tests
```bash
./engine-c/build.sh && npm test      # smoke Vitest sobre el WASM
cc -std=c99 -Wall -Wextra -Werror engine-c/src/*.c engine-c/tests/test_sumar.c -lm -o /tmp/t && /tmp/t   # unit C nativo
```

## Notas
- CAS: placeholder en `cas/README.md` (Fase 2).
- `build.sh` genera además un build `.mjs` con `ENVIRONMENT=web,node` solo para Vitest; el build web usa `ENVIRONMENT=web`.
