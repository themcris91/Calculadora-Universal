import { useState, useEffect } from 'react';
import './index.css';

export default function App() {
  const [wasm, setWasm] = useState<any>(null);
  const [rate, setRate] = useState('0.1');
  const [flows, setFlows] = useState('-100, 50, 60');
  const [npvResult, setNpvResult] = useState<string>('');
  const [m, setM] = useState('1, 2, 3, 4');
  const [detResult, setDetResult] = useState<string>('');

  useEffect(() => {
    // Accedemos a la función global que crea el index.html
    // @ts-ignore
    if (window.createModule) {
      // @ts-ignore
      window.createModule().then((instance: any) => {
        setWasm(instance);
      });
    }
  }, []);

  const calcularNPV = () => {
    if (!wasm) return;
    const cashflows = flows.split(',').map(Number);
    const ptr = wasm._malloc(cashflows.length * 8);
    const resultPtr = wasm._malloc(8);
    const heap = new Float64Array(wasm.HEAPF64.buffer, ptr, cashflows.length);
    heap.set(cashflows);
    wasm._api_fin_npv(parseFloat(rate), ptr, cashflows.length, resultPtr);
    const res = wasm.HEAPF64[resultPtr / 8];
    setNpvResult(res.toFixed(4));
    wasm._free(ptr);
    wasm._free(resultPtr);
  };

  const calcularDet = () => {
    if (!wasm) return;
    const matrix = m.split(',').map(Number);
    const ptr = wasm._malloc(matrix.length * 8);
    const resultPtr = wasm._malloc(8);
    const heap = new Float64Array(wasm.HEAPF64.buffer, ptr, matrix.length);
    heap.set(matrix);
    wasm._api_la_det(ptr, 2, resultPtr);
    const res = wasm.HEAPF64[resultPtr / 8];
    setDetResult(res.toFixed(4));
    wasm._free(ptr);
    wasm._free(resultPtr);
  };

  return (
    <div className="min-h-screen bg-slate-900 text-white p-8 font-sans">
      <div className="max-w-4xl mx-auto">
        <h1 className="text-3xl font-bold mb-8 text-center text-blue-400">Calculadora Universal (WASM)</h1>
        <div className="grid md:grid-cols-2 gap-8">
          <div className="bg-slate-800 p-6 rounded-xl shadow-lg border border-slate-700">
            <h2 className="text-xl font-bold mb-4 text-emerald-400">Finanzas: Calcular VAN</h2>
            <label className="block text-sm mb-1">Tasa (ej. 0.1)</label>
            <input type="text" value={rate} onChange={e => setRate(e.target.value)} className="w-full bg-slate-900 border border-slate-600 p-2 rounded mb-4" />
            <label className="block text-sm mb-1">Flujos de Caja</label>
            <input type="text" value={flows} onChange={e => setFlows(e.target.value)} className="w-full bg-slate-900 border border-slate-600 p-2 rounded mb-4" />
            <button onClick={calcularNPV} className="w-full bg-emerald-600 hover:bg-emerald-500 font-bold py-2 rounded transition">Calcular VAN</button>
            {npvResult && <div className="mt-4 p-3 bg-slate-900 rounded text-center text-lg">Resultado: <span className="text-emerald-400 font-bold">{npvResult}</span></div>}
          </div>
          <div className="bg-slate-800 p-6 rounded-xl shadow-lg border border-slate-700">
            <h2 className="text-xl font-bold mb-4 text-purple-400">Álgebra: Determinante 2x2</h2>
            <label className="block text-sm mb-1">Matriz (ej. 1, 2, 3, 4)</label>
            <input type="text" value={m} onChange={e => setM(e.target.value)} className="w-full bg-slate-900 border border-slate-600 p-2 rounded mb-4" />
            <button onClick={calcularDet} className="w-full bg-purple-600 hover:bg-purple-500 font-bold py-2 rounded transition">Calcular Determinante</button>
            {detResult && <div className="mt-4 p-3 bg-slate-900 rounded text-center text-lg">Resultado: <span className="text-purple-400 font-bold">{detResult}</span></div>}
          </div>
        </div>
      </div>
    </div>
  );
}
