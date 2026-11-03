import { useEffect, useState } from 'react'
import katex from 'katex'
import 'katex/dist/katex.min.css'
import { loadEngine, type Engine } from './wasm/api'

function Tex({ expr }: { expr: string }) {
  const html = katex.renderToString(expr, { throwOnError: false, displayMode: true })
  // El LaTeX se arma solo con números ya formateados: no hay texto de usuario sin parsear.
  return <div className="tex" dangerouslySetInnerHTML={{ __html: html }} />
}

const fmt = (x: number) => String(Number(x.toPrecision(12)))
const par = (x: number) => (x < 0 ? `(${fmt(x)})` : fmt(x))
const parse = (s: string): number | null => {
  const t = s.trim().replace(',', '.')
  const v = Number(t)
  return t !== '' && Number.isFinite(v) ? v : null
}

export default function App() {
  const [engine, setEngine] = useState<Engine | null>(null)
  const [loadError, setLoadError] = useState<string | null>(null)

  const [x, setX] = useState('2')
  const [y, setY] = useState('3')
  const [sumTex, setSumTex] = useState<string | null>(null)
  const [sumErr, setSumErr] = useState<string | null>(null)

  const [a, setA] = useState('0')
  const [b, setB] = useState(String(Math.PI))
  const [n, setN] = useState('1000')
  const [intTex, setIntTex] = useState<string | null>(null)
  const [intErr, setIntErr] = useState<string | null>(null)

  useEffect(() => {
    loadEngine()
      .then(setEngine)
      .catch((e: unknown) => setLoadError(e instanceof Error ? e.message : String(e)))
  }, [])

  const sumar = () => {
    const vx = parse(x)
    const vy = parse(y)
    if (!engine || vx === null || vy === null) {
      setSumTex(null)
      setSumErr('Ingresá dos números válidos.')
      return
    }
    setSumErr(null)
    setSumTex(`${fmt(vx)} + ${par(vy)} = ${fmt(engine.sumar(vx, vy))}`)
  }

  const integral = () => {
    const va = parse(a)
    const vb = parse(b)
    const vn = parse(n)
    if (!engine || va === null || vb === null || vn === null || !Number.isInteger(vn) || vn < 2) {
      setIntTex(null)
      setIntErr('a y b deben ser números; n, un entero ≥ 2.')
      return
    }
    setIntErr(null)
    const r = engine.integralSin(va, vb, vn)
    setIntTex(`\\int_{${fmt(va)}}^{${fmt(vb)}} \\sin(x)\\,dx \\approx ${fmt(r)}`)
  }

  return (
    <main>
      <h1>Calculadora Universal</h1>
      <p className={engine ? 'status ok' : 'status'} role="status">
        {loadError ? `Error al cargar el motor: ${loadError}` : engine ? 'Motor WASM listo' : 'Cargando motor…'}
      </p>

      <section aria-labelledby="h-suma">
        <h2 id="h-suma">Suma</h2>
        <div className="row">
          <label>x<input inputMode="decimal" value={x} onChange={(e) => setX(e.target.value)} /></label>
          <label>y<input inputMode="decimal" value={y} onChange={(e) => setY(e.target.value)} /></label>
          <button onClick={sumar} disabled={!engine}>Sumar</button>
        </div>
        {sumErr && <p className="err" role="alert">{sumErr}</p>}
        {sumTex && <Tex expr={sumTex} />}
      </section>

      <section aria-labelledby="h-int">
        <h2 id="h-int">Integral de sin (Simpson)</h2>
        <div className="row">
          <label>a<input inputMode="decimal" value={a} onChange={(e) => setA(e.target.value)} /></label>
          <label>b<input inputMode="decimal" value={b} onChange={(e) => setB(e.target.value)} /></label>
          <label>n<input inputMode="numeric" value={n} onChange={(e) => setN(e.target.value)} /></label>
          <button onClick={integral} disabled={!engine}>Integral de sin</button>
        </div>
        {intErr && <p className="err" role="alert">{intErr}</p>}
        {intTex && <Tex expr={intTex} />}
      </section>
    </main>
  )
}
