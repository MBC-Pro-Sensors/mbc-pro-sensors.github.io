// 切換列與首頁選擇器的「真的點擊」測試（Input.dispatchMouseEvent／dispatchKeyEvent，不是只看 href）
//   用 chrome-mbc（CDP 9223）；先在背景跑 py -3 -m http.server 8772 --directory _site
//   node _build/tests/nav_interact.mjs [--base http://localhost:8772] [--shots 截圖資料夾]
// 測：切換列換產品／主機／軟體（≥10 組）、鍵盤 Tab／Enter／Esc、localStorage mbc.product/host/sw、
//     產品頁「繼續用 …」、首頁三步選擇器（桌機 1280 與手機 390）、停用 JS 時的對照表
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { openTab, KEYS, sleep } from './cdp.mjs';

const BUILD = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const argv = process.argv.slice(2);
const opt = (k, d) => { const i = argv.indexOf(k); return i > -1 ? argv[i + 1] : d; };
const BASE = opt('--base', 'http://localhost:8772').replace(/\/$/, '');
const SHOTS = opt('--shots', '');
const GI = JSON.parse(fs.readFileSync(path.join(BUILD, 'generated', 'guides-index.json'), 'utf8'));
const hostsOf = (p) => Object.fromEntries(GI.products[p].hosts.map((h) => [h.id, h.sws]));
const pre = (en) => (en ? '/en' : '');
function bestFor(en, p, h, s) {
  const hs = hostsOf(p)[h];
  if (!hs) return `${pre(en)}/sensors/${p}/`;
  if (s && hs.includes(s)) return `${pre(en)}/sensors/${p}/${h}/${s}.html`;
  if (hs.length === 1) return `${pre(en)}/sensors/${p}/${h}/${hs[0]}.html`;
  return `${pre(en)}/sensors/${p}/${h}/`;
}

const results = [];
const ok = (name, pass, detail = '') => { results.push({ name, pass, detail }); console.log(`${pass ? 'PASS' : 'FAIL'}  ${name}${detail ? '  ' + detail : ''}`); };
const tab = await openTab('about:blank');
await tab.send('Page.enable'); await tab.send('Runtime.enable');
const jsErrors = [];
tab.on('Runtime.exceptionThrown', (p) => jsErrors.push(p.exceptionDetails.exception?.description || p.exceptionDetails.text));
const desktop = () => tab.send('Emulation.setDeviceMetricsOverride', { width: 1280, height: 900, deviceScaleFactor: 1, mobile: false });
const mobile = () => tab.send('Emulation.setDeviceMetricsOverride', { width: 390, height: 844, deviceScaleFactor: 2, mobile: true });
const path_ = () => tab.eval('location.pathname');
const shot = async (name) => { if (!SHOTS) return; const r = await tab.send('Page.captureScreenshot', { format: 'png' }); fs.writeFileSync(path.join(SHOTS, name + '.png'), Buffer.from(r.data, 'base64')); };
async function clickNav(sel) { const loaded = tab.once('Page.loadEventFired', 15000); await tab.click(sel); await loaded; await sleep(250); }

await desktop();
await tab.goto(BASE + '/');
await tab.eval('localStorage.clear(), 1');

// ---------- 1. 切換列：滑鼠點擊
const seg = { p: 'details.gsw-p', h: 'details.gsw-h', s: 'details.gsw-s' };
const cases = [
  // [起點, 段, 目標(產品 key / 主機 id / 軟體 id), 說明]
  ['/sensors/line8/spike/pybricks.html', 'p', 'tof8', '換產品保留主機與軟體'],
  ['/sensors/tof8/spike/pybricks.html', 'p', 'imu', '新產品沒有 SPIKE → 產品頁'],
  ['/sensors/line8/spike/pybricks.html', 'h', 'ev3', '換主機保留同名軟體 Pybricks'],
  ['/sensors/line8/ev3/classroom.html', 'h', 'spike', 'SPIKE 沒有 Classroom → 平台頁'],
  ['/sensors/line8/spike/pybricks.html', 's', 'official', '換軟體'],
  ['/sensors/line8/spike/official.html', 'p', 'exp6', '換到 EXP6 保留官方 App'],
  ['/sensors/exp6/spike/mbcblocks.html', 'p', 'servo6', 'Servo6 沒有 MBC 積木網站 → 平台頁'],
  ['/sensors/line8/arduino/ide.html', 'p', 'tof2', '換產品保留 Arduino IDE'],
  ['/sensors/imu/matrix/bigchannel.html', 'p', 'esp32cam', '神攝手 MATRIX 只有 mVision → 直接到 mVision'],
  ['/sensors/line8/mblock/mblock5.html', 'h', 'microbit', '換主機到只有一種軟體的 micro:bit'],
  ['/sensors/line8/matrix/matrixblock.html', 's', 'bigchannel', 'MATRIX 換軟體'],
  ['/en/sensors/line8/spike/pybricks.html', 'p', 'tof2', '英文頁換產品'],
  ['/en/sensors/ps4/ev3/pybricks.html', 'h', 'arduino', '英文頁換主機'],
];
for (const [from, k, target, label] of cases) {
  try {
    await tab.goto(BASE + from);
    const en = from.startsWith('/en/');
    const [, p, h, s] = from.match(/\/sensors\/([^/]+)\/([^/]+)\/([^/.]+)\.html/);
    const exp = k === 'p' ? bestFor(en, target, h, s) : k === 'h' ? bestFor(en, p, target, s) : `${pre(en)}/sensors/${p}/${h}/${target}.html`;
    await tab.click(`${seg[k]} > summary`);
    await sleep(150);
    const open = await tab.eval(`document.querySelector('${seg[k]}').open`);
    const sel = `${seg[k]} a[href="${exp}"]`;
    if (!(await tab.eval(`!!document.querySelector('${sel}')`))) { ok(`切換列 ${label}`, false, `選單裡沒有 ${exp}`); continue; }
    await clickNav(sel);
    const got = await path_();
    ok(`切換列 ${label}`, open && got === exp, `${from} →（${k}:${target}）→ ${got}${got === exp ? '' : '，預期 ' + exp}${open ? '' : '，點 summary 沒打開下拉'}`);
  } catch (e) { ok(`切換列 ${label}`, false, e.message); }
}

// 一次只開一個下拉；點外面關閉
await tab.goto(BASE + '/sensors/line8/spike/pybricks.html');
await tab.click('details.gsw-p > summary'); await sleep(100);
await tab.click('details.gsw-h > summary'); await sleep(100);
let st = await tab.eval(`[...document.querySelectorAll('details.gsw-seg')].map(d => d.open)`);
ok('一次只開一個下拉', st.join() === 'false,true,false', st.join());
await tab.click('article h1'); await sleep(100);
st = await tab.eval(`[...document.querySelectorAll('details.gsw-seg')].map(d => d.open)`);
ok('點外面關閉下拉', st.every((x) => !x), st.join());

// ---------- 2. 鍵盤
await tab.goto(BASE + '/sensors/tof2/spike/pybricks.html');
let found = false;
for (let i = 0; i < 200; i++) {
  await tab.key(...KEYS.Tab);
  if (await tab.eval(`document.activeElement === document.querySelector('details.gsw-p > summary')`)) { found = true; break; }
}
ok('Tab 能走到切換列「產品」', found);
if (found) {
  await tab.key(...KEYS.Enter); await sleep(150);
  const r1 = await tab.eval(`({ open: document.querySelector('details.gsw-p').open, focus: document.activeElement.getAttribute('href') })`);
  ok('Enter 打開下拉、焦點移到目前項目', r1.open && r1.focus === '/sensors/tof2/spike/pybricks.html', JSON.stringify(r1));
  await tab.key(...KEYS.Escape); await sleep(150);
  const r2 = await tab.eval(`({ open: document.querySelector('details.gsw-p').open, onSummary: document.activeElement === document.querySelector('details.gsw-p > summary') })`);
  ok('Esc 關閉下拉、焦點回到 summary', !r2.open && r2.onSummary, JSON.stringify(r2));
  await tab.key(...KEYS.Enter); await sleep(150);
  await tab.key(...KEYS.Tab); await sleep(80);
  const nextHref = await tab.eval('document.activeElement.getAttribute("href")');
  const loaded = tab.once('Page.loadEventFired', 15000);
  await tab.key(...KEYS.Enter); await loaded; await sleep(200);
  const got = await path_();
  ok('下拉裡 Tab 到下一項、Enter 前往', got === nextHref, `${nextHref} → ${got}`);
  // 主機段用空白鍵
  let onH = false;
  for (let i = 0; i < 200; i++) { await tab.key(...KEYS.Tab); if (await tab.eval(`document.activeElement === document.querySelector('details.gsw-h > summary')`)) { onH = true; break; } }
  await tab.key(...KEYS.Space); await sleep(150);
  ok('Tab 到「主機」、空白鍵打開下拉', onH && await tab.eval(`document.querySelector('details.gsw-h').open`), onH ? '' : 'Tab 200 次沒走到主機段');
  await tab.key(...KEYS.Escape); await sleep(100);
}

// ---------- 3. localStorage 與「繼續用 …」
await tab.goto(BASE + '/sensors/tof8/ev3/pybricks.html');
let ls = await tab.eval(`[localStorage.getItem('mbc.product'), localStorage.getItem('mbc.host'), localStorage.getItem('mbc.sw')]`);
ok('localStorage 寫入 mbc.product/host/sw', ls.join() === 'tof8,ev3,pybricks', ls.join());
await tab.goto(BASE + '/sensors/line16/');
let cont = await tab.eval(`(() => { const c = document.querySelector('.pick-continue'); const a = c && c.querySelector('a'); return { hidden: c ? c.hidden : null, href: a && a.getAttribute('href'), text: a && a.textContent, last: (document.querySelector('.pick-tile.last') || {}).dataset?.host } })()`);
ok('產品頁出現「繼續用 EV3 › Pybricks」並連對', !cont.hidden && cont.href === '/sensors/line16/ev3/pybricks.html' && /EV3 › Pybricks/.test(cont.text) && cont.last === 'ev3', JSON.stringify(cont));
if (!cont.hidden) { await clickNav('.pick-continue a'); ok('點「繼續用」到軟體頁', (await path_()) === '/sensors/line16/ev3/pybricks.html', await path_()); }
await shot('continue-line16');
// 這款有主機但沒有這個軟體 → 連平台頁
await tab.goto(BASE + '/sensors/exp6/spike/mbcblocks.html');
await tab.goto(BASE + '/sensors/servo6/');
cont = await tab.eval(`(() => { const c = document.querySelector('.pick-continue'); const a = c && c.querySelector('a'); return { hidden: c ? c.hidden : null, href: a && a.getAttribute('href'), text: a && a.textContent } })()`);
ok('沒有同名軟體時「繼續用 SPIKE」連平台頁', !cont.hidden && cont.href === bestFor(false, 'servo6', 'spike', null) && !/›.*›/.test(cont.text), JSON.stringify(cont));
// 這款沒有這個主機 → 不顯示
await tab.goto(BASE + '/sensors/line8/ev3/ev3g.html');
await tab.goto(BASE + '/sensors/exp6/');
cont = await tab.eval(`(() => { const c = document.querySelector('.pick-continue'); return { hidden: c ? c.hidden : null, n: c ? c.children.length : -1 } })()`);
ok('這款沒有 EV3 → 不顯示「繼續用」', cont.hidden === true && cont.n === 0, JSON.stringify(cont));

// ---------- 4. 首頁三步選擇器（桌機、手機）
for (const [label, setVp] of [['桌機 1280', desktop], ['手機 390', mobile]]) {
  await setVp();
  await tab.goto(BASE + '/');
  await tab.eval('localStorage.clear(), 1');
  await tab.goto(BASE + '/');
  const tableClosed = await tab.eval(`!document.querySelector('.finder-table').open && getComputedStyle(document.querySelector('.finder-pick')).display !== 'none'`);
  ok(`${label}：有 JS 時顯示三步選擇、對照表收起`, tableClosed);
  const allDisabled = await tab.eval(`[...document.querySelectorAll('.finder-hosts button')].every(b => b.disabled)`);
  ok(`${label}：還沒選產品時主機都不能選`, allDisabled);
  await tab.click('.finder-prods button[data-key="exp6"]'); await sleep(120);
  const hs = await tab.eval(`[...document.querySelectorAll('.finder-hosts button')].map(b => b.dataset.host + ':' + (b.disabled ? 'off' : 'on'))`);
  const wantHs = GI.hosts.map((h) => h.id + ':' + (hostsOf('exp6')[h.id] ? 'on' : 'off'));
  ok(`${label}：EXP6 只開可用主機、其他灰掉`, hs.join() === wantHs.join(), hs.join(' '));
  const greyed = await tab.eval(`(() => { const b = document.querySelector('.finder-hosts button[data-host="ev3"]'); const cs = getComputedStyle(b); return { op: cs.opacity, cursor: cs.cursor, title: b.title } })()`);
  ok(`${label}：灰掉的主機看得出來（透明度／游標／提示）`, +greyed.op < 1 || greyed.cursor === 'not-allowed', JSON.stringify(greyed));
  await tab.click('.finder-hosts button[data-host="ev3"]'); await sleep(100);
  const ev3Picked = await tab.eval(`document.querySelector('.finder-hosts button[data-host="ev3"]').getAttribute('aria-checked')`);
  ok(`${label}：點灰掉的主機沒有反應`, ev3Picked !== 'true', ev3Picked);
  await tab.click('.finder-hosts button[data-host="spike"]'); await sleep(120);
  const sws = await tab.eval(`[...document.querySelectorAll('.finder-sws button')].map(b => b.dataset.sw)`);
  ok(`${label}：SPIKE 列出 EXP6 的軟體`, sws.join() === hostsOf('exp6').spike.join(), sws.join());
  await tab.click('.finder-sws button[data-sw="pybricks"]'); await sleep(120);
  const res = await tab.eval(`(() => { const a = document.querySelector('.finder-result a'); return a ? { href: a.getAttribute('href'), text: a.textContent } : null })()`);
  ok(`${label}：結果連結正確`, res && res.href === '/sensors/exp6/spike/pybricks.html', JSON.stringify(res));
  await shot(`finder-${label.includes('手機') ? 'mobile' : 'desktop'}`);
  // 換產品：主機不支援時清掉、只有一種軟體自動選
  await tab.click('.finder-prods button[data-key="tof2"]'); await sleep(100);
  await tab.click('.finder-hosts button[data-host="arduino"]'); await sleep(120);
  const auto = await tab.eval(`(() => { const a = document.querySelector('.finder-result a'); return a ? a.getAttribute('href') : null })()`);
  ok(`${label}：只有一種軟體時自動選好`, auto === '/sensors/tof2/arduino/ide.html', auto);
  if (res) { await clickNav('.finder-result a'); ok(`${label}：按「前往」到軟體頁`, (await path_()) === '/sensors/tof2/arduino/ide.html', await path_()); }
  ls = await tab.eval(`[localStorage.getItem('mbc.product'), localStorage.getItem('mbc.host'), localStorage.getItem('mbc.sw')]`);
  ok(`${label}：選擇器記住選擇`, ls.join() === 'tof2,arduino,ide', ls.join());
}
await desktop();

// ---------- 5. 停用 JS：對照表
await tab.send('Emulation.setScriptExecutionDisabled', { value: true });
await tab.goto(BASE + '/');
const nojs = await tab.eval(`(() => { const t = document.querySelector('.finder-table'); const links = [...t.querySelectorAll('a')].map(a => a.getAttribute('href')); return { open: t.open, pickHidden: getComputedStyle(document.querySelector('.finder-pick')).display === 'none', links } })()`, false).catch((e) => ({ err: e.message }));
await tab.send('Emulation.setScriptExecutionDisabled', { value: false });
if (nojs.err) ok('停用 JS：對照表', false, nojs.err);
else {
  let miss = 0;
  for (const l of nojs.links) { const r = await fetch(BASE + encodeURI(l), { method: 'HEAD' }); if (r.status !== 200) miss++; }
  const wantN = Object.keys(GI.products).reduce((n, p) => n + 1 + GI.products[p].hosts.reduce((m, h) => m + h.sws.length, 0), 0);
  ok('停用 JS：三步選擇藏起、對照表展開、連結全部 200', nojs.open && nojs.pickHidden && miss === 0 && nojs.links.length === wantN, `連結 ${nojs.links.length}（預期 ${wantN}），非 200 ${miss}`);
}
await tab.goto(BASE + '/');
await tab.eval('localStorage.clear(), 1');
await tab.close();

ok('過程中沒有 JS 例外', jsErrors.length === 0, jsErrors.slice(0, 3).join(' | '));
const fail = results.filter((r) => !r.pass);
console.log(`\n共 ${results.length} 項，通過 ${results.length - fail.length}，失敗 ${fail.length}`);
process.exit(fail.length ? 1 : 0);
