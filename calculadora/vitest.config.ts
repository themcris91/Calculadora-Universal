import { defineConfig } from 'vitest/config'

export default defineConfig({
  test: {
    include: ['tests-integration/**/*.test.ts'],
    environment: 'node',
    // El glue Emscripten (.mjs) se carga con el import nativo de Node, sin transformar.
    server: { deps: { external: [/engine-c[\\/]build/] } },
  },
})
