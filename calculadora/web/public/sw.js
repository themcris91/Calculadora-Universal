const CACHE_NAME = 'calc-wasm-v3';

// Guardamos los archivos iniciales
self.addEventListener('install', (event) => {
  self.skipWaiting();
});

self.addEventListener('activate', (event) => {
  event.waitUntil(self.clients.claim());
});

self.addEventListener('fetch', (event) => {
  // ESTRATEGIA CACHÉ PRIMERO: Busca en el celular, si no está, va a internet.
  event.respondWith(
    caches.match(event.request).then((response) => {
      if (response) {
        return response; // Si ya lo descargamos antes, lo devuelve al instante
      }
      return fetch(event.request).then((networkResponse) => {
        // Si viene de internet, guarda una copia en caché
        if (networkResponse && networkResponse.status === 200 && networkResponse.type === 'basic') {
          const responseClone = networkResponse.clone();
          caches.open(CACHE_NAME).then((cache) => {
            cache.put(event.request, responseClone);
          });
        }
        return networkResponse;
      });
    }).catch(() => {
      return new Response('Estás offline', { status: 503 });
    })
  );
});
