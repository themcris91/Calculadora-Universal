/** Tipos y carga del módulo Emscripten (engine-c). */

export interface EngineModule {
  cwrap(
    name: string,
    returnType: 'number' | 'string' | null,
    argTypes: Array<'number' | 'string'>,
  ): (...args: number[]) => number
}

export interface Engine {
  sumar(a: number, b: number): number
  integralSin(a: number, b: number, n: number): number
}

type EngineFactory = () => Promise<EngineModule>

/** Envuelve un módulo ya instanciado con una API tipada (también la usan los tests en Node). */
export function wrapEngine(mod: EngineModule): Engine {
  const sumar = mod.cwrap('sumar', 'number', ['number', 'number'])
  const integralSin = mod.cwrap('integral_sin', 'number', ['number', 'number', 'number'])
  return {
    sumar: (a, b) => sumar(a, b),
    integralSin: (a, b, n) => integralSin(a, b, n),
  }
}

let cached: Promise<Engine> | undefined

/** Carga perezosa desde /wasm/api.js (public/, cache-first en el SW). */
export function loadEngine(): Promise<Engine> {
  if (!cached) {
    const url = `${import.meta.env.BASE_URL}wasm/api.js`
    cached = import(/* @vite-ignore */ url).then(async (m: { default: EngineFactory }) =>
      wrapEngine(await m.default()),
    )
    cached.catch(() => {
      cached = undefined // permitir reintento
    })
  }
  return cached
}
