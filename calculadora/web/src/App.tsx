import { useState, useEffect } from 'react';
import './index.css';

export default function App() {
  const [wasm, setWasm] = useState<any>(null);
  const [activeTab, setActiveTab] = useState<'basica' | 'finanzas' | 'matrices' | 'complejos' | 'bases' | 'integral'>('basica');

  // Estados Calculadora Básica
  const [display, setDisplay] = useState('0');
  const [prevValue, setPrevValue] = useState<number | null>(null);
  const [operator, setOperator] = useState<string | null>(null);
  const [waitingForNewValue, setWaitingForNewValue] = useState(false);

  // Estados Finanzas
  const [rate, setRate] = useState('0.1');
  const [flows, setFlows] = useState('-100, 50, 60');
  const [npvResult, setNpvResult] = useState<string>('');

  // Estados Matrices
  const [m, setM] = useState('1, 2, 3, 4');
  const [detResult, setDetResult] = useState<string>('');

  // Estados Complejos
  const [re, setRe] = useState('3');
  const [im, setIm] = useState('4');
  const [polarResult, setPolarResult] = useState<{r: string, theta: string} | null>(null);

  // Estados Bases
  const [numStr, setNumStr] = useState('255');
  const [toBase, setToBase] = useState('16');
  const [baseResult, setBaseResult] = useState('');

  // Estados Integral
  const [intA, setIntA] = useState('0');
  const [intB, setIntB] = useState('3.14159');
  const [integralResult, setIntegralResult] = useState('');

  useEffect(() => {
    // @ts-ignore
    if (window.Module) {
      // @ts-ignore
      window.Module({ locateFile: (path: string) => path.endsWith('.wasm') ? '/wasm/api.wasm' : path })
        .then((instance: any) => setWasm(instance));
    }
  }, []);

  const handleNumber = (num: string) => {
    if (waitingForNewValue) { setDisplay(num); setWaitingForNewValue(false); }
    else { setDisplay(display === '0' ? num : display + num); }
  };
  const handleOperator = (op: string) => {
    const currentVal = parseFloat(display);
    if (prevValue === null) setPrevValue(currentVal);
    else if (operator) {
      const result = calculate(prevValue, currentVal, operator);
      setPrevValue(result); setDisplay(String(result));
    }
    setOperator(op); setWaitingForNewValue(true);
  };
  const calculate = (a: number, b: number, op: string) => {
    switch (op) {
      case '+': return a + b; case '-': return a - b;
      case '×': return a * b; case '÷': return b !== 0 ? a / b : 0;
      default: return b;
    }
  };
  const handleEqual = () => {
    if (operator && prevValue !== null) {
      const currentVal = parseFloat(display);
      setDisplay(String(calculate(prevValue, currentVal, operator)));
      setPrevValue(null); setOperator(null); setWaitingForNewValue(true);
    }
  };

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

  const calcularPolar = () => {
    if (!wasm) return alert("Motor cargando...");
    const rPtr = wasm._malloc(8); const tPtr = wasm._malloc(8);
    wasm._api_cx_rect_to_polar(parseFloat(re), parseFloat(im), 0, rPtr, tPtr);
    setPolarResult({
      r: wasm.HEAPF64[rPtr / 8].toFixed(4),
      theta: wasm.HEAPF64[tPtr / 8].toFixed(4)
    });
    wasm._free(rPtr); wasm._free(tPtr);
  };

  const convertirBase = () => {
    if (!wasm) return alert("Motor cargando...");
    const bufferSize = 64;
    const bufPtr = wasm._malloc(bufferSize);
    const strPtr = wasm._malloc(numStr.length + 1);
    wasm.stringToUTF8(numStr, strPtr, numStr.length + 1);
    
    const status = wasm._api_basen_convert(strPtr, 10, parseInt(toBase), bufPtr, bufferSize);
    if (status === 0) setBaseResult(wasm.UTF8ToString(bufPtr));
    else setBaseResult("Error");

    wasm._free(bufPtr); wasm._free(strPtr);
  };

  const calcularIntegral = () => {
    if (!wasm) return alert("Motor cargando...");
    const resultPtr = wasm._malloc(8);
    wasm._api_num_integral_sin(parseFloat(intA), parseFloat(intB), 1000, resultPtr);
    setIntegralResult(wasm.HEAPF64[resultPtr / 8].toFixed(6));
    wasm._free(resultPtr);
  };

  return (
    <div className="flex flex-col h-[100dvh] bg-slate-900 text-white font-sans">
      <div className="flex space-x-2 p-3 bg-slate-950 border-b border-slate-800 overflow-x-auto whitespace-nowrap hide-scrollbar">
        {['basica', 'finanzas', 'matrices', 'complejos', 'bases', 'integral'].map((tab) => (
          <button key={tab} onClick={() => setActiveTab(tab as any)}
            className={`px-4 py-2 rounded-full text-sm font-semibold capitalize transition-colors ${activeTab === tab ? 'bg-blue-600 text-white' : 'text-slate-400 bg-slate-900'}`}>
            {tab}
          </button>
        ))}
      </div>

      <div className="flex-1 flex flex-col overflow-y-auto pb-4">
        {activeTab === 'basica' && (
          <div className="flex flex-col h-full">
            <div className="flex-1 flex flex-col justify-end items-end p-6 bg-slate-900">
              <div className="text-slate-500 text-lg h-6">{prevValue !== null ? `${prevValue} ${operator || ''}` : ''}</div>
              <div className="text-6xl font-light text-white overflow-hidden w-full text-right">{display}</div>
            </div>
            <div className="bg-slate-950 p-4 rounded-t-3xl shadow-2xl">
              <div className="grid grid-cols-4 gap-3">
                <button onClick={() => { setDisplay('0'); setPrevValue(null); setOperator(null); }} className="bg-red-900/40 text-red-400 rounded-2xl h-16 font-bold text-xl">AC</button>
                <button className="bg-slate-800 rounded-2xl h-16 text-xl text-blue-400">( )</button>
                <button className="bg-slate-800 rounded-2xl h-16 text-xl text-blue-400">%</button>
                <button onClick={() => handleOperator('÷')} className="bg-blue-900/50 text-blue-400 rounded-2xl h-16 font-bold text-2xl">÷</button>
                <button onClick={() => handleNumber('7')} className="bg-slate-800 rounded-2xl h-16 text-2xl">7</button>
                <button onClick={() => handleNumber('8')} className="bg-slate-800 rounded-2xl h-16 text-2xl">8</button>
                <button onClick={() => handleNumber('9')} className="bg-slate-800 rounded-2xl h-16 text-2xl">9</button>
                <button onClick={() => handleOperator('×')} className="bg-blue-900/50 text-blue-400 rounded-2xl h-16 font-bold text-2xl">×</button>
                <button onClick={() => handleNumber('4')} className="bg-slate-800 rounded-2xl h-16 text-2xl">4</button>
                <button onClick={() => handleNumber('5')} className="bg-slate-800 rounded-2xl h-16 text-2xl">5</button>
                <button onClick={() => handleNumber('6')} className="bg-slate-800 rounded-2xl h-16 text-2xl">6</button>
                <button onClick={() => handleOperator('-')} className="bg-blue-900/50 text-blue-400 rounded-2xl h-16 font-bold text-2xl">-</button>
                <button onClick={() => handleNumber('1')} className="bg-slate-800 rounded-2xl h-16 text-2xl">1</button>
                <button onClick={() => handleNumber('2')} className="bg-slate-800 rounded-2xl h-16 text-2xl">2</button>
                <button onClick={() => handleNumber('3')} className="bg-slate-800 rounded-2xl h-16 text-2xl">3</button>
                <button onClick={() => handleOperator('+')} className="bg-blue-900/50 text-blue-400 rounded-2xl h-16 font-bold text-2xl">+</button>
                <button onClick={() => handleNumber('0')} className="bg-slate-800 rounded-2xl h-16 text-2xl col-span-2">0</button>
                <button onClick={() => handleNumber('.')} className="bg-slate-800 rounded-2xl h-16 text-2xl">.</button>
                <button onClick={handleEqual} className="bg-emerald-600 rounded-2xl h-16 font-bold text-3xl">=</button>
              </div>
            </div>
          </div>
        )}

        {activeTab === 'finanzas' && (
          <div className="p-6 space-y-4">
            <h2 className="text-2xl font-bold text-emerald-400">Calcular VAN</h2>
            <input type="text" value={rate} onChange={e => setRate(e.target.value)} className="w-full bg-slate-800 p-4 rounded-xl" />
            <input type="text" value={flows} onChange={e => setFlows(e.target.value)} className="w-full bg-slate-800 p-4 rounded-xl" />
            <button onClick={calcularNPV} className="w-full bg-emerald-600 font-bold py-4 rounded-xl">Calcular</button>
            {npvResult && <div className="bg-slate-800 p-6 rounded-xl text-center"><p className="text-4xl font-bold text-emerald-400">{npvResult}</p></div>}
          </div>
        )}

        {activeTab === 'matrices' && (
          <div className="p-6 space-y-4">
            <h2 className="text-2xl font-bold text-purple-400">Determinante 2x2</h2>
            <input type="text" value={m} onChange={e => setM(e.target.value)} className="w-full bg-slate-800 p-4 rounded-xl" />
            <button onClick={calcularDet} className="w-full bg-purple-600 font-bold py-4 rounded-xl">Calcular</button>
            {detResult && <div className="bg-slate-800 p-6 rounded-xl text-center"><p className="text-4xl font-bold text-purple-400">{detResult}</p></div>}
          </div>
        )}

        {activeTab === 'complejos' && (
          <div className="p-6 space-y-4">
            <h2 className="text-2xl font-bold text-orange-400">Rectangular a Polar</h2>
            <div className="flex space-x-2">
              <input type="text" value={re} onChange={e => setRe(e.target.value)} className="w-1/2 bg-slate-800 p-4 rounded-xl" />
              <input type="text" value={im} onChange={e => setIm(e.target.value)} className="w-1/2 bg-slate-800 p-4 rounded-xl" />
            </div>
            <button onClick={calcularPolar} className="w-full bg-orange-600 font-bold py-4 rounded-xl">Convertir</button>
            {polarResult && (
              <div className="bg-slate-800 p-6 rounded-xl text-center flex justify-around">
                <div><p className="text-sm text-slate-400">r</p><p className="text-2xl font-bold text-orange-400">{polarResult.r}</p></div>
                <div><p className="text-sm text-slate-400">θ (rad)</p><p className="text-2xl font-bold text-orange-400">{polarResult.theta}</p></div>
              </div>
            )}
          </div>
        )}

        {activeTab === 'bases' && (
          <div className="p-6 space-y-4">
            <h2 className="text-2xl font-bold text-cyan-400">Conversor de Bases</h2>
            <input type="text" value={numStr} onChange={e => setNumStr(e.target.value)} className="w-full bg-slate-800 p-4 rounded-xl" />
            <select value={toBase} onChange={e => setToBase(e.target.value)} className="w-full bg-slate-800 p-4 rounded-xl">
              <option value="2">A Binario</option>
              <option value="8">A Octal</option>
              <option value="16">A Hexadecimal</option>
            </select>
            <button onClick={convertirBase} className="w-full bg-cyan-600 font-bold py-4 rounded-xl">Convertir</button>
            {baseResult && <div className="bg-slate-800 p-6 rounded-xl text-center"><p className="text-4xl font-mono font-bold text-cyan-400">{baseResult}</p></div>}
          </div>
        )}

        {activeTab === 'integral' && (
          <div className="p-6 space-y-4">
            <h2 className="text-2xl font-bold text-pink-400">Integral de sin(x)</h2>
            <div className="flex space-x-2">
              <input type="text" value={intA} onChange={e => setIntA(e.target.value)} className="w-1/2 bg-slate-800 p-4 rounded-xl" />
              <input type="text" value={intB} onChange={e => setIntB(e.target.value)} className="w-1/2 bg-slate-800 p-4 rounded-xl" />
            </div>
            <button onClick={calcularIntegral} className="w-full bg-pink-600 font-bold py-4 rounded-xl">Integrar</button>
            {integralResult && <div className="bg-slate-800 p-6 rounded-xl text-center"><p className="text-4xl font-bold text-pink-400">{integralResult}</p></div>}
          </div>
        )}
      </div>
    </div>
  );
}
