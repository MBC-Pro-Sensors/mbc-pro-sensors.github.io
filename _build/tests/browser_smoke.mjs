// 全站瀏覽器冒煙測試：用 chrome-mbc（CDP 9223）逐頁載入 _site 的每個 HTML，收集
//   JS 例外（Runtime.exceptionThrown）、console.error、瀏覽器記錄的錯誤（Log.entryAdded）、
//   回應 ≥400 的資源、載入失敗的資源（script/css/img/font/fetch）、壞圖（naturalWidth = 0，lazy 圖也強制載入）
// 用法：先在背景跑 py -3 -m http.server 8772 --directory _site
//       node _build/tests/browser_smoke.mjs [--base http://localhost:8772] [--only 子字串] [--out 結果.json]
// 有問題結束碼 1。外部網域（YouTube、Google Fonts…）的失敗另列，不算本站錯誤。
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { openTab, sleep } from './cdp.mjs';

const BUILD = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const SITE = path.join(BUILD, '..', '_site');
const argv = process.argv.slice(2);
const opt = (k, d) => { const i = argv.indexOf(k); return i > -1 ? argv[i + 1] : d; };
const BASE = opt('--base', 'http://localhost:8772').replace(/\/$/, '');
const ONLY = opt('--only', '');
const OUT = opt('--out', '');

const pages = [];
(function walk(d) { for (const f of fs.readdirSync(d)) { const p = path.join(d, f); if (fs.statSync(p).isDirectory()) walk(p); else if (f.endsWith('.html')) pages.push('/' + path.relative(SITE, p).replace(/\\/g, '/').replace(/(^|\/)index\.html$/, '$1')); } })(SITE);
const list = pages.filter((p) => p.includes(ONLY)).sort();

const tab = await openTab('about:blank');
await tab.send('Page.enable'); await tab.send('Runtime.enable'); await tab.send('Log.enable'); await tab.send('Network.enable');
await tab.send('Network.setCacheDisabled', { cacheDisabled: false });
await tab.send('Emulation.setDeviceMetricsOverride', { width: 1280, height: 900, deviceScaleFactor: 1, mobile: false });

let cur = null;
const reqUrl = new Map();
tab.on('Runtime.exceptionThrown', (p) => cur && cur.js.push((p.exceptionDetails.exception?.description || p.exceptionDetails.text || '').split('\n')[0] + ` @${p.exceptionDetails.url || ''}:${p.exceptionDetails.lineNumber}`));
tab.on('Runtime.consoleAPICalled', (p) => { if (cur && (p.type === 'error' || p.type === 'assert')) cur.console.push(p.args.map((a) => a.value ?? a.description ?? '').join(' ').slice(0, 200)); });
tab.on('Log.entryAdded', (p) => { if (cur && p.entry.level === 'error') cur.log.push(`${p.entry.text} ${p.entry.url || ''}`.slice(0, 250)); });
tab.on('Network.requestWillBeSent', (p) => reqUrl.set(p.requestId, { url: p.request.url, type: p.type }));
tab.on('Network.responseReceived', (p) => { if (cur && p.response.status >= 400) cur.http.push(`${p.response.status} ${p.type} ${p.response.url}`); });
tab.on('Network.loadingFailed', (p) => { if (cur && !p.canceled) { const r = reqUrl.get(p.requestId) || {}; cur.failed.push(`${p.errorText} ${p.type} ${r.url || ''}`); } });

const results = [];
const t0 = Date.now();
for (let i = 0; i < list.length; i++) {
  const u = list[i];
  cur = { page: u, js: [], console: [], log: [], http: [], failed: [], brokenImg: [] };
  try {
    await tab.goto(BASE + encodeURI(u), 250);
    cur.brokenImg = await tab.eval(`(async () => {
      const imgs = [...document.images];
      imgs.forEach((im) => { if (im.loading === 'lazy') im.loading = 'eager'; });
      await Promise.race([Promise.all(imgs.map((im) => im.complete ? 0 : new Promise((r) => { im.onload = im.onerror = r; }))), new Promise((r) => setTimeout(r, 4000))]);
      return imgs.filter((im) => im.complete && im.naturalWidth === 0 && im.currentSrc).map((im) => im.currentSrc);
    })()`);
    await sleep(100);
  } catch (e) { cur.js.push('載入失敗：' + e.message); }
  results.push(cur);
  if ((i + 1) % 50 === 0) console.error(`… ${i + 1}/${list.length}（${Math.round((Date.now() - t0) / 1000)} 秒）`);
}
await tab.close();

const local = (s) => s.includes(BASE) || !/https?:\/\//.test(s);
const issues = [];
const external = [];
for (const r of results) {
  const own = { page: r.page, js: r.js, console: r.console, log: r.log.filter(local), http: r.http.filter(local), failed: r.failed.filter(local), brokenImg: r.brokenImg.filter(local) };
  const ext = [...r.log, ...r.http, ...r.failed, ...r.brokenImg].filter((s) => !local(s));
  if (ext.length) external.push({ page: r.page, ext: [...new Set(ext)] });
  if (own.js.length || own.console.length || own.log.length || own.http.length || own.failed.length || own.brokenImg.length) issues.push(own);
}
console.log(`載入 ${results.length} 頁，用時 ${Math.round((Date.now() - t0) / 1000)} 秒；有問題 ${issues.length} 頁；外部資源有錯 ${external.length} 頁`);
for (const x of issues) console.log(JSON.stringify(x));
const extCount = {};
for (const e of external) for (const s of e.ext) { const k = s.replace(/https?:\/\/([^/]+).*/, '$1').slice(0, 120); extCount[k] = (extCount[k] || 0) + 1; }
if (external.length) console.log('外部資源錯誤（網域：次數）', JSON.stringify(extCount));
if (OUT) fs.writeFileSync(OUT, JSON.stringify({ issues, external }, null, 1));
process.exit(issues.length ? 1 : 0);
