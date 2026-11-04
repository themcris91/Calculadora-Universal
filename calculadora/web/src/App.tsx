import { useState, useEffect } from 'react';
import './index.css';

export default function App() {
  const [wasm, setWasm] = useState<any>(null);
  const [activeTab, setActiveTab] = useState<'basica' | 'finanzas' | 'matrices'>('basica');

  // Estados Calculadora Básica
  const [display, setDisplay] = useState('0');
  const [prevValue, setPrevValue] = useState<number | null>(null);
  const [operator, setOperator] = useState<string | null>(null);
  const [waitingForNewValue, setWaitingForNewValue] = useState(false);

  // Estados Finanzas (VAN)
  const [rate, setRate] = useState('0.1');
  const [flows, setFlows] = useState('-100, 50, 60');
  const [npvResult, setNpvResult] = useState<string>('');

  // Estados Matrices (Det)
  const [m, setM] = useState('1, 2, 3, 4');
  const [detResult, setDetResult] = useState<string>('');

  // Cargar el motor WASM
  useEffect(() => {
    // @ts-ignore
    if (window.Module) {
      // @ts-ignore
      window.Module({
        locateFile: (path: string) => path.endsWith('.wasm') ? '/wasm/api.wasm' : path
      }).then((instance: any) => setWasm(instance));
    }
  }, []);

  // --- Lógica Calculadora Básica ---
  const handleNumber = (num: string) => {
    if (waitingForNewValue) {
      setDisplay(num);
      setWaitingForNewValue(false);
    } else {
      setDisplay(display === '0' ? num : display + num);
    }
  };

  const handleOperator = (op: string) => {
    const currentVal = parseFloat(display);
    if (prevValue === null) setPrevValue(currentVal);
    else if (operator) {
      const result = calculate(prevValue, currentVal, operator);
      setPrevValue(result);
      setDisplay(String(result));
    }
    setOperator(op);
    setWaitingForNewValue(true);
  };

  const calculate = (a: number, b: number, op: string) => {
    switch (op) {
      case '+': return a + b;
      case '-': return a - b;
      case '×': return a * b;
      case '÷': return b !== 0 ? a / b : 0;
      default: return b;
    }
  };

  const handleEqual = () => {
    if (operator && prevValue !== null) {
      const currentVal = parseFloat(display);
      const result = calculate(prevValue, currentVal, operator);
      setDisplay(String(result));
      setPrevValue(null);
      setOperator(null);
      setWaitingForNewValue(true);
    }
  };

  // --- Lógica WASM Finanzas y Matrices ---
  const calcularNPV = () => {
    if (!wasm) return alert("Motor cargando...");
    const cashflows = flows.split(',').map(Number);
    const ptr = wasm._malloc(cashflows.length * 8);
    const resultPtr = wasm._malloc(8);
    const heap = new Float64Array(wasm.HEAPF64.buffer, ptr, cashflows.length);
    heap.set(cashflows);
    wasm._api_fin_npv(parseFloat(rate), ptr, cashflows.length, resultPtr);
    setNpvResult(wasm.HEAPF64[resultPtr / 8].toFixed(4));
    wasm._free(ptr); wasm._free(resultPtr);
  };

  const calcularDet = () => {
    if (!wasm) return alert("Motor cargando...");
    const matrix = m.split(',').map(Number);
    const ptr = wasm._malloc(matrix.length * 8);
    const resultPtr = wasm._malloc(8);
    const heap = new Float64Array(wasm.HEAPF64.buffer, ptr, matrix.length);
    heap.set(matrix);
    wasm._api_la_det(ptr, 2, resultPtr);
    setDetResult(wasm.HEAPF64[resultPtr / 8].toFixed(4));
    wasm._free(ptr); wasm._free(resultPtr);
  };

  return (
    <div className="flex flex-col h-[100dvh] bg-slate-900 text-white font-sans">
      
      {/* MENÚ SUPERIOR */}
      <div className="flex justify-center space-x-2 p-3 bg-slate-950 border-b border-slate-800">
        {['basica', 'finanzas', 'matrices'].map((tab) => (
          <button
            key={tab}
            onClick={() => setActiveTab(tab as any)}
            className={`px-4 py-2 rounded-full text-sm font-semibold capitalize transition-colors ${
              activeTab === tab ? 'bg-blue-600 text-white' : 'text-slate-400 hover:bg-slate-800'
            }`}
          >
            {tab}
          </button>
        ))}
      </div>

      {/* CONTENIDO PRINCIPAL */}
      <div className="flex-1 flex flex-col overflow-y-auto">
        
        {/* VISTA BÁSICA */}
        {activeTab === 'basica' && (
          <>
            <div className="flex-1 flex flex-col justify-end items-end p-6 bg-slate-900">
              <div className="text-slate-500 text-lg h-6">
                {prevValue !== null ? `${prevValue} ${operator || ''}` : ''}
              </div>
              <div className="text-6xl font-light tracking-tight text-white overflow-hidden w-full text-right">{display}</div>
            </div>
            <div className="bg-slate-950 p-4 pb-8 rounded-t-3xl shadow-2xl border-t border-slate-800">
              <div className="grid grid-cols-4 gap-3">
                <button onClick={() => { setDisplay('0'); setPrevValue(null); setOperator(null); }} className="bg-red-900/40 text-red-400 rounded-2xl h-16 font-bold text-xl active:bg-red-900">AC</button>
                <button className="bg-slate-800 rounded-2xl h-16 font-bold text-xl text-blue-400">( )</button>
                <button className="bg-slate-800 rounded-2xl h-16 font-bold text-xl text-blue-400">%</button>
                <button onClick={() => handleOperator('÷')} className="bg-blue-900/50 text-blue-400 rounded-2xl h-16 font-bold text-2xl active:bg-blue-900">÷</button>

                <button onClick={() => handleNumber('7')} className="bg-slate-800 rounded-2xl h-16 text-2xl active:bg-slate-700">7</button>
                <button onClick={() => handleNumber('8')} className="bg-slate-800 rounded-2xl h-16 text-2xl active:bg-slate-700">8</button>
                <button onClick={() => handleNumber('9')} className="bg-slate-800 rounded-2xl h-16 text-2xl active:bg-slate-700">9</button>
                <button onClick={() => handleOperator('×')} className="bg-blue-900/50 text-blue-400 rounded-2xl h-16 font-bold text-2xl active:bg-blue-900">×</button>

                <button onClick={() => handleNumber('4')} className="bg-slate-800 rounded-2xl h-16 text-2xl active:bg-slate-700">4</button>
                <button onClick={() => handleNumber('5')} className="bg-slate-800 rounded-2xl h-16 text-2xl active:bg-slate-700">5</button>
                <button onClick={() => handleNumber('6')} className="bg-slate-800 rounded-2xl h-16 text-2xl active:bg-slate-700">6</button>
                <button onClick={() => handleOperator('-')} className="bg-blue-900/50 text-blue-400 rounded-2xl h-16 font-bold text-2xl active:bg-blue-900">-</button>

                <button onClick={() => handleNumber('1')} className="bg-slate-800 rounded-2xl h-16 text-2xl active:bg-slate-700">1</button>
                <button onClick={() => handleNumber('2')} className="bg-slate-800 rounded-2xl h-16 text-2xl active:bg-slate-700">2</button>
                <button onClick={() => handleNumber('3')} className="bg-slate-800 rounded-2xl h-16 text-2xl active:bg-slate-700">3</button>
                <button onClick={() => handleOperator('+')} className="bg-blue-900/50 text-blue-400 rounded-2xl h-16 font-bold text-2xl active:bg-blue-900">+</button>

                <button onClick={() => handleNumber('0')} className="bg-slate-800 rounded-2xl h-16 text-2xl active:bg-slate-700 col-span-2">0</button>
                <button onClick={() => handleNumber('.')} className="bg-slate-800 rounded-2xl h-16 text-2xl active:bg-slate-700">.</button>
                <button onClick={handleEqual} className="bg-emerald-600 text-white rounded-2xl h-16 font-bold text-3xl active:bg-emerald-500">=</button>
              </div>
            </div>
          </>
        )}

        {/* VISTA FINANZAS (VAN) */}
        {activeTab === 'finanzas' && (
          <div className="p-6 space-y-6">
            <h2 className="text-2xl font-bold text-emerald-400">Calcular VAN</h2>
            <div className="space-y-4">
              <div>
                <label className="block text-sm text-slate-400 mb-1">Tasa (ej. 0.1)</label>
                <input type="text" value={rate} onChange={e => setRate(e.target.value)} className="w-full bg-slate-800 border border-slate-700 p-4 rounded-xl text-lg" />
              </div>
              <div>
                <label className="block text-sm text-slate-400 mb-1">Flujos de Caja (separados por coma)</label>
                <input type="text" value={flows} onChange={e => setFlows(e.target.value)} className="w-full bg-slate-800 border border-slate-700 p-4 rounded-xl text-lg" />
              </div>
              <button onClick={calcularNPV} className="w-full bg-emerald-600 text-white font-bold py-4 rounded-xl text-lg active:bg-emerald-500">Calcular</button>
              
              {npvResult && (
                <div className="bg-slate-800 p-6 rounded-xl text-center border border-emerald-900 mt-4">
                  <p className="text-sm text-slate-400 mb-2">Resultado</p>
                  <p className="text-4xl font-bold text-emerald-400">{npvResult}</p>
                </div>
              )}
            </div>
          </div>
        )}

        {/* VISTA MATRICES (DET) */}
        {activeTab === 'matrices' && (
          <div className="p-6 space-y-6">
            <h2 className="text-2xl font-bold text-purple-400">Determinante 2x2</h2>
            <div className="space-y-4">
              <div>
                <label className="block text-sm text-slate-400 mb-1">Matriz (ej. 1, 2, 3, 4)</label>
                <input type="text" value={m} onChange={e => setM(e.target.value)} className="w-full bg-slate-800 border border-slate-700 p-4 rounded-xl text-lg" />
              </div>
              <button onClick={calcularDet} className="w-full bg-purple-600 text-white font-bold py-4 rounded-xl text-lg active:bg-purple-500">Calcular</button>
              
              {detResult && (
                <div className="bg-slate-800 p-6 rounded-xl text-center border border-purple-900 mt-4">
                  <p className="text-sm text-slate-400 mb-2">Resultado</p>
                  <p className="text-4xl font-bold text-purple-400">{detResult}</p>
                </div>
              )}
            </div>
          </div>
        )}

      </div>
    </div>
  );
}
