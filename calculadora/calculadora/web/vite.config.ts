import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'
import { VitePWA } from 'vite-plugin-pwa'

// vite-plugin-pwa: genera el SW con Workbox (GenerateSW) integrado al build de Vite
// y hashea el precache automáticamente; evita mantener un CLI/webpack aparte.
export default defineConfig({
  plugins: [
    react(),
    VitePWA({
      strategies: 'generateSW',
      registerType: 'autoUpdate',
      injectRegister: false, // el registro está en index.html
      manifest: false, // se usa public/manifest.json
      workbox: {
        // HTML fuera del precache: va network-first vía runtimeCaching.
        globPatterns: ['**/*.{js,css,wasm,png,svg,json,woff2}'],
        maximumFileSizeToCacheInBytes: 8 * 1024 * 1024,
        cleanupOutdatedCaches: true,
        clientsClaim: true,
        skipWaiting: true,
        runtimeCaching: [
          {
            urlPattern: ({ request }) => request.mode === 'navigate',
            handler: 'NetworkFirst',
            options: { cacheName: 'html', networkTimeoutSeconds: 3 },
          },
          {
            urlPattern: ({ url }) =>
              url.pathname.endsWith('.wasm') || url.pathname.endsWith('/wasm/api.js'),
            handler: 'CacheFirst',
            options: { cacheName: 'wasm', expiration: { maxEntries: 10 } },
          },
        ],
      },
    }),
  ],
})
