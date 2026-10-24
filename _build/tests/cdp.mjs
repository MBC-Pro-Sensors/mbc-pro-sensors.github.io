// 極簡 Chrome DevTools Protocol 客戶端（Node 22+ 內建 WebSocket），給 _build/tests/*.mjs 用
// 一律連產品專用瀏覽器 chrome-mbc（CDP 9223，D:\ChromeAgentProfile_MBC）；自己開分頁、用完關掉，不碰別的分頁
export const CDP_PORT = +(process.env.CDP_PORT || 9223);

export async function openTab(url = 'about:blank') {
  // 一般分頁。注意：這個瀏覽器別的 agent 也在用，它們把自己的分頁拉到前面時，我們的分頁會變背景、收不到滑鼠／鍵盤事件，
  // 所以 click()／key() 每次都先 Page.bringToFront。（試過 Target.createTarget newWindow，在這台反而完全收不到輸入）
  let t;
  try {
    const r = await fetch(`http://127.0.0.1:${CDP_PORT}/json/new?${encodeURI(url)}`, { method: 'PUT' });
    t = await r.json();
  } catch (e) {
    throw new Error(`連不上 chrome-mbc（CDP ${CDP_PORT}）：先執行 工具\\01_AI開發工具\\網頁AI討論群\\tools\\agent-chrome-mbc.ps1`);
  }
  // 測試程式中途丟例外時也要關掉自己開的分頁
  const closeOnExit = () => { try { fetch(`http://127.0.0.1:${CDP_PORT}/json/close/${t.id}`); } catch {} };
  process.once('uncaughtException', (e) => { console.error(e); closeOnExit(); setTimeout(() => process.exit(1), 300); });
  process.once('unhandledRejection', (e) => { console.error(e); closeOnExit(); setTimeout(() => process.exit(1), 300); });
  const ws = new WebSocket(t.webSocketDebuggerUrl);
  await new Promise((res, rej) => { ws.onopen = res; ws.onerror = rej; });
  let id = 0;
  const pending = new Map();
  const listeners = new Map();
  ws.onmessage = (ev) => {
    const m = JSON.parse(ev.data);
    if (m.id && pending.has(m.id)) {
      const { res, rej } = pending.get(m.id); pending.delete(m.id);
      if (m.error) rej(new Error(m.error.message)); else res(m.result);
    } else if (m.method) for (const cb of listeners.get(m.method) || []) cb(m.params);
  };
  const tab = {
    id: t.id,
    send(method, params = {}) { return new Promise((res, rej) => { const i = ++id; pending.set(i, { res, rej }); ws.send(JSON.stringify({ id: i, method, params })); }); },
    on(method, cb) { if (!listeners.has(method)) listeners.set(method, []); listeners.get(method).push(cb); return () => listeners.set(method, listeners.get(method).filter((x) => x !== cb)); },
    once(method, timeout = 15000) { return new Promise((res) => { let off; const tm = setTimeout(() => { off(); res(null); }, timeout); off = tab.on(method, (p) => { clearTimeout(tm); off(); res(p); }); }); },
    async eval(expr, awaitPromise = true) {
      const r = await tab.send('Runtime.evaluate', { expression: expr, awaitPromise, returnByValue: true, userGesture: true });
      if (r.exceptionDetails) throw new Error('頁內執行錯誤：' + (r.exceptionDetails.exception?.description || r.exceptionDetails.text));
      return r.result.value;
    },
    async goto(u, settle = 300) {
      // 分頁在背景時 Chrome 不處理滑鼠／鍵盤事件（別的分頁跑 Lighthouse 後最常見），每次換頁都拉到前面
      await tab.send('Page.bringToFront').catch(() => {});
      const loaded = tab.once('Page.loadEventFired', 20000);
      await tab.send('Page.navigate', { url: u });
      await loaded;
      if (settle) await new Promise((r) => setTimeout(r, settle));
    },
    // 真的滑鼠點擊（Input.dispatchMouseEvent），點元素中心；先捲進畫面
    async click(selector) {
      const box = await tab.eval(`(() => { const e = document.querySelector(${JSON.stringify(selector)}); if (!e) return null; e.scrollIntoView({block:'center', behavior:'instant'}); const r = e.getBoundingClientRect(); return { x: r.left + r.width / 2, y: r.top + r.height / 2 }; })()`);
      if (!box) throw new Error('找不到元素 ' + selector);
      await tab.send('Page.bringToFront').catch(() => {});
      for (const type of ['mouseMoved', 'mousePressed', 'mouseReleased']) await tab.send('Input.dispatchMouseEvent', { type, x: box.x, y: box.y, button: 'left', clickCount: 1 });
    },
    async key(key, code = key, keyCode = 0, modifiers = 0) {
      const text = key === 'Enter' ? '\r' : key.length === 1 ? key : undefined;
      await tab.send('Input.dispatchKeyEvent', { type: 'keyDown', key, code, windowsVirtualKeyCode: keyCode, modifiers, text });
      await tab.send('Input.dispatchKeyEvent', { type: 'keyUp', key, code, windowsVirtualKeyCode: keyCode, modifiers });
    },
    async close() { try { ws.close(); } catch {} try { await fetch(`http://127.0.0.1:${CDP_PORT}/json/close/${t.id}`); } catch {} },
  };
  return tab;
}

export const KEYS = { Tab: ['Tab', 'Tab', 9], Enter: ['Enter', 'Enter', 13], Escape: ['Escape', 'Escape', 27], Space: [' ', 'Space', 32], ArrowDown: ['ArrowDown', 'ArrowDown', 40] };
export const sleep = (ms) => new Promise((r) => setTimeout(r, ms));
