import { beforeAll, describe, expect, it } from 'vitest'
import { wrapEngine, type Engine, type EngineModule } from '../web/src/wasm/api'

let engine: Engine

beforeAll(async () => {
  // Build de test generado por engine-c/build.sh (ENVIRONMENT=web,node).
  const href = new URL('../engine-c/build/test/api.mjs', import.meta.url).href
  const { default: createEngine } = (await import(/* @vite-ignore */ href)) as {
    default: () => Promise<EngineModule>
  }
  engine = wrapEngine(await createEngine())
})

describe('smoke WASM', () => {
  it('sumar(2, 3) === 5', () => {
    expect(engine.sumar(2, 3)).toBe(5)
  })

  it('integral_sin(0, π, 1000) ≈ 2 (tol 1e-6)', () => {
    expect(Math.abs(engine.integralSin(0, Math.PI, 1000) - 2)).toBeLessThan(1e-6)
  })
})
