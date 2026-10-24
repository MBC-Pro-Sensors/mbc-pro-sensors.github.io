// 積木圖與程式碼區塊（chrome-mbc，CDP 9223）
//   node _build/tests/code_blocks.mjs [--base http://localhost:8772]
// 1. 每個 mBlock 軟體頁（中英）：<pre class="blocks"> 都被 scratchblocks 畫成 SVG
// 2. 抽樣頁的程式碼區塊（cpp／python／typescript／basic／vb）都有「複製」按鈕；真的點一下，剪貼簿內容 = 程式碼（Windows 剪貼簿會把換行變成 \r\n，比對前先換回 \n）
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { openTab, sleep } from './cdp.mjs';

const BUILD = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const argv = process.argv.slice(2);
const opt = (k, d) => { const i = argv.indexOf(k); return i > -1 ? argv[i + 1] : d; };
const BASE = opt('--base', 'http://localhost:8772').replace(/\/$/, '');
const GI = JSON.parse(fs.readFileSync(path.join(BUILD, 'generated', 'guides-index.json'), 'utf8'));
const LANGS = ['cpp', 'python', 'typescript', 'basic', 'vb'];

let fails = 0;
const say = (pass, msg) => { if (!pass) fails++; console.log(`${pass ? 'PASS' : 'FAIL'}  ${msg}`); };
const tab = await openTab('about:blank');
await tab.send('Page.enable'); await tab.send('Runtime.enable');
await tab.send('Emulation.setDeviceMetricsOverride', { width: 1280, height: 900, deviceScaleFactor: 1, mobile: false });
await tab.send('Emulation.setFocusEmulationEnabled', { enabled: true });
await tab.send('Browser.grantPermissions', { origin: BASE, permissions: ['clipboardReadWrite', 'clipboardSanitizedWrite'] }).catch(() => {});

// 1. scratchblocks
const mblockPages = [];
for (const [p, v] of Object.entries(GI.products)) if (v.hosts.some((h) => h.id === 'mblock')) for (const pre of ['', '/en']) mblockPages.push(`${pre}/sensors/${p}/mblock/mblock5.html`);
let totalBlocks = 0;
for (const u of mblockPages) {
  await tab.goto(BASE + u, 600);
  const r = await tab.eval(`(() => { const all = [...document.querySelectorAll('.markdown-section .blocks')]; return { n: all.length, svg: all.filter(e => e.querySelector('svg')).length, raw: all.filter(e => !e.querySelector('svg')).map(e => e.textContent.slice(0, 40)).slice(0, 2) } })()`);
  totalBlocks += r.n;
  say(r.n > 0 && r.svg === r.n, `${u}：積木 ${r.n} 段、畫成 SVG ${r.svg}${r.raw.length ? '，沒畫的：' + JSON.stringify(r.raw) : ''}`);
}
console.log(`mBlock 頁 ${mblockPages.length} 頁，積木共 ${totalBlocks} 段`);

// 2. 複製按鈕
const codePages = ['/sensors/line8/spike/pybricks.html', '/sensors/tof8/arduino/ide.html', '/en/sensors/ps4/arduino/ide.html', '/sensors/line16/microbit/makecode.html', '/sensors/line8/ev3/clev3r.html', '/en/sensors/esp32cam/ev3/clev3r.html', '/sensors/imu/matrix/matrixblock.html', '/i2c-protocol.html'];
for (const u of codePages) {
  await tab.goto(BASE + u, 400);
  const r = await tab.eval(`(() => { const sel = ${JSON.stringify(LANGS)}.map(l => '.markdown-section pre[data-lang="' + l + '"]').join(','); const pres = [...document.querySelectorAll(sel)]; return { n: pres.length, btn: pres.filter(p => p.querySelector('button.code-copy')).length, langs: [...new Set(pres.map(p => p.dataset.lang))] } })()`);
  if (!r.n) { console.log(`—     ${u}：沒有程式碼區塊`); continue; }
  say(r.btn === r.n, `${u}：程式碼區塊 ${r.n}（${r.langs.join('/')}），有複製按鈕 ${r.btn}`);
  // 點第一個「看得到」的按鈕（摺起來的 <details> 先打開）
  await tab.eval(`(() => { const b = document.querySelector('button.code-copy'); const d = b && b.closest('details'); if (d) d.open = true; window.__want = b ? b.parentElement.querySelector('code').innerText : null; return 1 })()`);
  await tab.click('button.code-copy');
  await sleep(300);
  const got = await tab.eval(`(async () => { try { return { text: (await navigator.clipboard.readText()).replace(/\\r\\n/g, "\\n"), label: document.querySelector('button.code-copy').textContent, want: window.__want }; } catch (e) { return { err: e.message, label: document.querySelector('button.code-copy').textContent }; } })()`);
  say(!got.err && got.text === got.want && /已複製|Copied/.test(got.label), `${u}：點「複製」→ 剪貼簿 ${got.err ? '讀取失敗 ' + got.err : got.text === got.want ? '= 程式碼（' + got.text.length + ' 字）' : '≠ 程式碼'}，按鈕顯示「${got.label}」`);
}
await tab.close();
process.exit(fails ? 1 : 0);
