// 線上站檢查（不開瀏覽器）：https://mbc-pro-sensors.github.io/
//   1. sitemap.xml 全部網址 GET 要 200
//   2. downloads.html（中英）每個下載連結 200 且大小 > 0；.zip 下載後 tar -tf 列得出內容
//   3. 英文頁 hreflang 互指（抽樣）
//   4. robots.txt、llms*.txt、IndexNow 金鑰檔 200
//   5. 舊頁（moved）有 canonical 指到新頁、頁面顯示「已整合到」提示
//   6. 線上與本機 _site 抽樣比對（去掉 ?v= 版本參數）
// 用法：node _build/tests/site_live.mjs [--zipdir 暫存資料夾] [--sample 20] [--compare 10]
// 有錯結束碼 1。
import fs from 'node:fs';
import os from 'node:os';
import path from 'node:path';
import { execFileSync } from 'node:child_process';
import { fileURLToPath } from 'node:url';

const BUILD = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const ROOT = path.join(BUILD, '..');
const SITE = path.join(ROOT, '_site');
const LIVE = 'https://mbc-pro-sensors.github.io';
const argv = process.argv.slice(2);
const opt = (k, d) => { const i = argv.indexOf(k); return i > -1 ? argv[i + 1] : d; };
const ZIPDIR = opt('--zipdir', fs.mkdtempSync(path.join(os.tmpdir(), 'mbc-zip-')));
const SAMPLE = +opt('--sample', 20);
const COMPARE = +opt('--compare', 10);
const problems = [];
const bad = (m) => problems.push(m);
const report = [];

async function get(u, { method = 'GET', buf = false } = {}) {
  for (let t = 0; t < 3; t++) {
    try {
      const r = await fetch(u, { method, redirect: 'follow' });
      const body = method === 'HEAD' ? null : buf ? Buffer.from(await r.arrayBuffer()) : await r.text();
      return { status: r.status, body, url: r.url, len: +(r.headers.get('content-length') || 0) };
    } catch (e) { if (t === 2) return { status: 0, err: e.message }; await new Promise((r) => setTimeout(r, 1000)); }
  }
}
async function pool(items, n, fn) { const out = []; let i = 0; await Promise.all(Array.from({ length: n }, async () => { while (i < items.length) { const k = i++; out[k] = await fn(items[k]); } })); return out; }

// 1. sitemap
const sm = await get(LIVE + '/sitemap.xml');
if (sm.status !== 200) bad(`sitemap.xml → ${sm.status}`);
const locs = [...(sm.body || '').matchAll(/<loc>([^<]+)<\/loc>/g)].map((m) => m[1]);
const smRes = await pool(locs, 12, async (u) => [u, (await get(u)).status]);
const smBad = smRes.filter(([, s]) => s !== 200);
for (const [u, s] of smBad) bad(`sitemap 網址 ${u} → ${s}`);
report.push(`sitemap：${locs.length} 個網址，非 200 的 ${smBad.length} 個`);
// sitemap 與本機一致？
const localSm = fs.existsSync(path.join(SITE, 'sitemap.xml')) ? [...fs.readFileSync(path.join(SITE, 'sitemap.xml'), 'utf8').matchAll(/<loc>([^<]+)<\/loc>/g)].map((m) => m[1]) : [];
const onlyLocal = localSm.filter((u) => !locs.includes(u)), onlyLive = locs.filter((u) => !localSm.includes(u));
report.push(`sitemap 本機 ${localSm.length} 個；只在本機 ${onlyLocal.length}、只在線上 ${onlyLive.length}${onlyLocal.length || onlyLive.length ? '（' + [...onlyLocal.map((u) => '+' + u), ...onlyLive.map((u) => '-' + u)].slice(0, 6).join(' ') + '）' : ''}`);

// 2. 下載
let nDl = 0, nZip = 0;
for (const page of ['/downloads.html', '/en/downloads.html']) {
  const r = await get(LIVE + page);
  if (r.status !== 200) { bad(`${page} → ${r.status}`); continue; }
  const links = [...new Set([...r.body.matchAll(/href="(\/[^"#?]+\.(?:zip|py|pdf|mext|llsp3|lms|lmsp|ev3|bp|ino|hex|sb3))"/g)].map((m) => m[1]))];
  const res = await pool(links, 6, async (l) => {
    const x = await get(LIVE + encodeURI(decodeURI(l)), { buf: true });
    const size = x.body ? x.body.length : 0;
    let tarOk = null;
    if (x.status === 200 && /\.zip$/i.test(l) && size) {
      const f = path.join(ZIPDIR, path.basename(decodeURI(l)));
      fs.writeFileSync(f, x.body);
      try { const out = execFileSync(process.platform === 'win32' ? path.join(process.env.SystemRoot || 'C:\\Windows', 'System32', 'tar.exe') : 'tar', ['-tf', f], { encoding: 'utf8' }); tarOk = out.trim().split('\n').length; } catch (e) { tarOk = 0; }
    }
    return { l, status: x.status, size, tarOk };
  });
  for (const x of res) {
    nDl++;
    if (x.status !== 200 || !x.size) bad(`${page} 下載 ${x.l} → ${x.status}，${x.size} bytes`);
    if (x.tarOk !== null) { nZip++; if (!x.tarOk) bad(`${page} 下載 ${x.l}：tar -tf 列不出內容`); }
  }
  report.push(`${page}：下載連結 ${links.length} 個`);
}
report.push(`下載檔共檢查 ${nDl} 次，其中 zip ${nZip} 個用 tar -tf 列出內容`);

// 3. hreflang 互指（抽樣：sitemap 裡的英文頁）
const enLocs = locs.filter((u) => u.startsWith(LIVE + '/en/'));
const step = Math.max(1, Math.floor(enLocs.length / SAMPLE));
const enSample = enLocs.filter((_, i) => i % step === 0).slice(0, SAMPLE);
const alt = (html, lang) => (html.match(new RegExp(`<link rel="alternate" hreflang="${lang}" href="([^"]+)"`)) || [])[1];
let hrefOk = 0;
for (const en of enSample) {
  const e = await get(en);
  const zh = alt(e.body || '', 'zh-TW');
  if (alt(e.body || '', 'en') !== en) { bad(`hreflang：${en} 的 en 不是自己（${alt(e.body || '', 'en')}）`); continue; }
  if (!zh) { bad(`hreflang：${en} 沒有 zh-TW`); continue; }
  const z = await get(zh);
  if (z.status !== 200) { bad(`hreflang：${en} → zh ${zh} ${z.status}`); continue; }
  if (alt(z.body, 'en') !== en) { bad(`hreflang：${zh} 的 en 指到 ${alt(z.body, 'en')}，不是 ${en}`); continue; }
  if (alt(z.body, 'zh-TW') !== zh) { bad(`hreflang：${zh} 的 zh-TW 不是自己`); continue; }
  hrefOk++;
}
report.push(`hreflang 抽 ${enSample.length} 頁英文頁，互指正確 ${hrefOk}`);

// 4. 機器讀的檔案
const key = fs.readdirSync(ROOT).find((f) => /^[0-9a-f]{32}\.txt$/.test(f));
for (const f of ['/robots.txt', '/llms.txt', '/llms-full.txt', '/llms-protocol.txt', '/en/llms.txt', key ? '/' + key : null].filter(Boolean)) {
  const r = await get(LIVE + f);
  if (r.status !== 200 || !(r.body || '').trim()) bad(`${f} → ${r.status}`);
  else if (f === '/' + key && r.body.trim() !== key.replace('.txt', '')) bad(`IndexNow 金鑰檔內容不是金鑰`);
}
report.push(`robots／llms／IndexNow 金鑰檔已檢查（金鑰 ${key}）`);

// 5. 舊頁
const GI = JSON.parse(fs.readFileSync(path.join(BUILD, 'generated', 'guides-index.json'), 'utf8'));
const movedPairs = Object.entries(GI.moved).filter(([f]) => /^(en\/)?sensors\/line8\//.test(f)).slice(0, 6);
movedPairs.unshift(['sensors/line8/spike-education.md', null]);
let movedOk = 0;
for (const [md] of movedPairs) {
  const u = LIVE + '/' + md.replace(/\.md$/, '.html');
  const r = await get(u);
  const canon = (r.body?.match(/<link rel="canonical" href="([^"]+)"/) || [])[1];
  const note = /已整合到|has moved into/.test(r.body || '');
  if (r.status !== 200) { bad(`舊頁 ${u} → ${r.status}`); continue; }
  if (!canon || canon === u) bad(`舊頁 ${u} canonical 沒指到新頁（${canon}）`);
  else { const c = await get(canon); if (c.status !== 200) bad(`舊頁 ${u} canonical ${canon} → ${c.status}`); }
  if (!note) bad(`舊頁 ${u} 沒有「已整合到」提示`);
  if (canon && canon !== u && note) movedOk++;
}
report.push(`舊頁抽 ${movedPairs.length} 頁，canonical＋提示正確 ${movedOk}`);

// 6. 線上 vs 本機
const norm = (s) => s.replace(/\?v=[a-z0-9]+/g, '').replace(/\r\n/g, '\n');
const cmp = ['/', '/en/', '/sensors/line8/', '/sensors/line8/spike/', '/sensors/line8/spike/pybricks.html', '/en/sensors/tof8/ev3/pybricks.html', '/sensors/ps4/mblock/mblock5.html', '/downloads.html', '/i2c-protocol.html', '/sensors/exp6/arduino/ide.html', '/en/sensors/esp32cam/microbit/makecode.html', '/reference.html'].slice(0, Math.max(COMPARE, 1));
let same = 0;
const diffs = [];
for (const u of cmp) {
  const r = await get(LIVE + u);
  const lf = path.join(SITE, u.endsWith('/') ? u + 'index.html' : u);
  if (!fs.existsSync(lf)) { diffs.push(`${u}：本機沒有`); continue; }
  const a = norm(r.body || ''), b = norm(fs.readFileSync(lf, 'utf8'));
  if (a === b) { same++; continue; }
  let i = 0; while (i < a.length && a[i] === b[i]) i++;
  diffs.push(`${u}：第 ${i} 字開始不同｜線上「${a.slice(i, i + 80).replace(/\s+/g, ' ')}」｜本機「${b.slice(i, i + 80).replace(/\s+/g, ' ')}」`);
}
report.push(`線上 vs 本機抽 ${cmp.length} 頁：相同 ${same}，不同 ${diffs.length}`);

console.log(report.join('\n'));
if (diffs.length) { console.log('線上與本機不同（本機有未推送的修改時是預期的）：'); for (const d of diffs) console.log('  ' + d); }
if (problems.length) { console.log(`問題 ${problems.length} 個：`); for (const p of problems) console.log('  ' + p); process.exit(1); }
console.log('線上檢查全部通過');
