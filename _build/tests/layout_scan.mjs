// 手機版量測＋文字對比掃描（chrome-mbc，CDP 9223）
//   node _build/tests/layout_scan.mjs [--base http://localhost:8772] [--shots 截圖資料夾] [--mobile-only | --contrast-only] [--all（對比掃 _site 全部頁）]
// 手機（390×844，mobile 模擬）：橫向捲動、觸控目標 <44px（段落內的行內連結不算，同 hq-site-uiux-audit/mobile_metrics.js）、
//   切換列 sticky 捲動後仍在頂部、目錄放在切換列下方
// 對比（桌機 1280，所有 <details> 先展開）：同 hq-site-uiux-audit/contrast_scan.js，列出 <4.5（大字 <3）的元素
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { openTab, sleep } from './cdp.mjs';

const argv = process.argv.slice(2);
const opt = (k, d) => { const i = argv.indexOf(k); return i > -1 ? argv[i + 1] : d; };
const BASE = opt('--base', 'http://localhost:8772').replace(/\/$/, '');
const SHOTS = opt('--shots', '');

const MOBILE_PAGES = ['/', '/en/', '/sensors/line8/', '/sensors/tof8/', '/sensors/exp6/', '/sensors/line8/spike/', '/sensors/ps4/ev3/', '/sensors/imu/matrix/',
  '/sensors/line8/spike/pybricks.html', '/sensors/tof2/mblock/mblock5.html', '/sensors/line16/microbit/makecode.html', '/sensors/ps2/matrix/bigchannel.html',
  '/sensors/tof8/arduino/ide.html', '/en/sensors/esp32cam/ev3/classroom.html', '/downloads.html', '/i2c-protocol.html', '/reference.html', '/en/sensors/line8/'];
const CONTRAST_PAGES = [...MOBILE_PAGES, '/en/downloads.html', '/en/i2c-protocol.html', '/guide.html', '/faq.html', '/contact.html', '/competitions.html', '/where-to-buy.html',
  '/sensors/esp32cam/', '/sensors/servo6/', '/sensors/ps2/spike/mbcblocks.html', '/sensors/line8/ev3/ev3g.html', '/sensors/imu/arduino/ide.html',
  '/en/sensors/tof8/spike/official.html', '/en/sensors/ps4/mblock/mblock5.html', '/en/sensors/line16/matrix/', '/articles/', '/makeblock.html', '/microbit.html', '/matrix.html', '/404.html'];

const MOBILE_FN = `(async () => {
  const w = window, d = document, H = w.innerHeight;
  const hscroll = d.documentElement.scrollWidth - w.innerWidth;
  const wide = hscroll > 1 ? [...d.querySelectorAll('body *')].filter(e => { const r = e.getBoundingClientRect(); return r.right > w.innerWidth + 1 && r.width > 0 && getComputedStyle(e).position !== 'fixed'; }).filter(e => !e.closest('pre, .table-wrap, table')).slice(0, 5).map(e => e.tagName + '.' + String(e.className).slice(0, 30) + ' right=' + Math.round(e.getBoundingClientRect().right)) : [];
  let total = 0; const small = [];
  d.querySelectorAll('a[href], button, summary, input, select').forEach((el) => {
    const r = el.getBoundingClientRect(); if (!r.width || !r.height) return;
    const cs = w.getComputedStyle(el); if (cs.display === 'inline' && el.closest('p, li, td')) return;
    if (cs.visibility === 'hidden') return;
    total++; if (r.height < 43.5 || r.width < 24) small.push((el.className || el.tagName) + ':' + Math.round(r.width) + 'x' + Math.round(r.height) + ' ' + (el.textContent || el.getAttribute('aria-label') || '').trim().slice(0, 14));
  });
  const gsw = d.querySelector('nav.gsw');
  let sticky = null, tocBelow = null;
  if (gsw) {
    const toc = d.querySelector('nav.toc');
    if (toc) { const a = gsw.getBoundingClientRect(), b = toc.getBoundingClientRect(); tocBelow = toc.previousElementSibling === gsw && b.top >= a.bottom - 1; }
    w.scrollTo(0, Math.min(1500, d.documentElement.scrollHeight - H - 10)); await new Promise(r => setTimeout(r, 300));
    const r = gsw.getBoundingClientRect();
    sticky = { pos: getComputedStyle(gsw).position, top: Math.round(r.top), h: Math.round(r.height), visible: r.top >= -1 && r.top < 140 && r.height > 0 };
    w.scrollTo(0, 0);
  }
  return { screens: +(d.documentElement.scrollHeight / H).toFixed(1), hscroll, wide, touchTotal: total, touchSmall: small.length, small: small.slice(0, 8), sticky, tocBelow };
})()`;

const CONTRAST_FN = `(() => {
  const win = window, doc = document;
  doc.querySelectorAll('details').forEach(d => d.open = true);
  const parse = (c) => { const k = c.match(/color\\(srgb ([^)]+)\\)/); if (k) { const p = k[1].split(/[ \\/]+/).filter(Boolean).map(Number); return { r: p[0] * 255, g: p[1] * 255, b: p[2] * 255, a: p.length > 3 ? p[3] : 1 }; } const m = c.match(/rgba?\\(([^)]+)\\)/); if (!m) return null; const p = m[1].split(/[ ,\\/]+/).filter(Boolean).map(Number); return { r: p[0], g: p[1], b: p[2], a: p.length > 3 ? p[3] : 1 }; };
  const lum = ({ r, g, b }) => { const f = (v) => { v /= 255; return v <= 0.03928 ? v / 12.92 : Math.pow((v + 0.055) / 1.055, 2.4); }; return 0.2126 * f(r) + 0.7152 * f(g) + 0.0722 * f(b); };
  const blend = (t, b) => ({ r: t.r * t.a + b.r * (1 - t.a), g: t.g * t.a + b.g * (1 - t.a), b: t.b * t.a + b.b * (1 - t.a), a: 1 });
  // 比 hq-site-uiux-audit 原版多一步：祖先的 ::before 若是整塊鋪底（首頁主視覺：深色漸層＋照片），用它的底色
  const bgOf = (el) => { const st = []; for (let e = el; e; e = e.parentElement) { const cs = win.getComputedStyle(e);
    const pb = win.getComputedStyle(e, '::before'); if (pb.content !== 'none' && pb.position === 'absolute' && parse(cs.backgroundColor)?.a !== 1) { const pc = parse(pb.backgroundColor); if (pc && pc.a === 1) { st.push(pc); break; } } if (cs.backgroundImage.includes('gradient')) { const cols = [...cs.backgroundImage.matchAll(/rgba?\\([^)]+\\)/g)].map((m) => parse(m[0])); const solid = cols.find((c) => c.a === 1); if (solid) { st.push(solid); break; } } const c = parse(cs.backgroundColor); if (c && c.a > 0) { st.push(c); if (c.a === 1) break; } } let bg = { r: 255, g: 255, b: 255, a: 1 }; for (let i = st.length - 1; i >= 0; i--) bg = blend(st[i], bg); return bg; };
  const bad = []; const seen = new Set();
  doc.querySelectorAll('body *').forEach((el) => {
    if (el.closest('pre, svg, iframe, script, style, .scratchblocks')) return;
    const txt = [...el.childNodes].filter((n) => n.nodeType === 3).map((n) => n.textContent.trim()).join(''); if (!txt) return;
    const r = el.getBoundingClientRect(); if (!r.width || !r.height) return;
    const cs = win.getComputedStyle(el); if (cs.visibility === 'hidden' || +cs.opacity === 0) return;
    let fg = parse(cs.webkitTextFillColor && cs.webkitTextFillColor !== cs.color ? cs.webkitTextFillColor : cs.color); if (!fg) return;
    const bg = bgOf(el); if (fg.a < 1) fg = blend(fg, bg);
    if (cs.filter.includes('brightness')) { const k = parseFloat(cs.filter.match(/brightness\\(([\\d.]+)/)[1]); fg = { r: fg.r * k, g: fg.g * k, b: fg.b * k, a: 1 }; }
    const L1 = lum(fg), L2 = lum(bg); const ratio = (Math.max(L1, L2) + 0.05) / (Math.min(L1, L2) + 0.05);
    const size = parseFloat(cs.fontSize); const large = size >= 24 || (size >= 18.66 && +cs.fontWeight >= 700);
    if (ratio < (large ? 3 : 4.5)) { const k = el.tagName + cs.color + el.className; if (!seen.has(k)) { seen.add(k); bad.push(ratio.toFixed(2) + ' ' + el.tagName + '.' + String(el.className).slice(0, 24) + ' ' + cs.color + ' on ' + Math.round(bg.r) + ',' + Math.round(bg.g) + ',' + Math.round(bg.b) + ' "' + txt.slice(0, 20) + '"'); } }
  });
  return bad;
})()`;

if (argv.includes('--all')) {
  const SITE = path.join(path.dirname(fileURLToPath(import.meta.url)), '..', '..', '_site');
  const all = [];
  (function walk(d) { for (const f of fs.readdirSync(d)) { const p = path.join(d, f); if (fs.statSync(p).isDirectory()) { if (f !== 'archive_homepage') walk(p); } else if (f.endsWith('.html') && !/^google/.test(f)) all.push('/' + path.relative(SITE, p).replace(/\\/g, '/').replace(/(^|\/)index\.html$/, '$1')); } })(SITE);
  CONTRAST_PAGES.splice(0, CONTRAST_PAGES.length, ...all.sort());
}
const tab = await openTab('about:blank');
await tab.send('Page.enable'); await tab.send('Runtime.enable');
let fails = 0;
if (!argv.includes('--contrast-only')) {
  await tab.send('Emulation.setDeviceMetricsOverride', { width: 390, height: 844, deviceScaleFactor: 2, mobile: true });
  await tab.send('Emulation.setTouchEmulationEnabled', { enabled: true });
  console.log('== 手機 390×844');
  for (const u of MOBILE_PAGES) {
    await tab.goto(BASE + u, 500);
    const m = await tab.eval(MOBILE_FN);
    const pass = m.hscroll <= 1 && (m.sticky === null || (m.sticky.pos === 'sticky' && m.sticky.visible)) && m.tocBelow !== false;
    if (!pass) fails++;
    console.log(`${pass ? 'PASS' : 'FAIL'} ${u}  屏數 ${m.screens}  橫向溢出 ${m.hscroll}px${m.wide.length ? ' ' + m.wide.join('; ') : ''}  觸控目標 ${m.touchTotal}（<44px ${m.touchSmall}${m.small.length ? '：' + m.small.join('｜') : ''}）  切換列 ${m.sticky ? `${m.sticky.pos} top=${m.sticky.top}${m.sticky.visible ? ' 可見' : ' 不見了'}` : '—'}  目錄在切換列下 ${m.tocBelow === null ? '—' : m.tocBelow}`);
    if (SHOTS) { const r = await tab.send('Page.captureScreenshot', { format: 'png' }); fs.writeFileSync(path.join(SHOTS, 'm_' + (u.replace(/[\/.]+/g, '_').replace(/^_|_$/g, '') || 'home') + '.png'), Buffer.from(r.data, 'base64')); }
  }
  await tab.send('Emulation.setTouchEmulationEnabled', { enabled: false });
}
if (!argv.includes('--mobile-only')) {
  await tab.send('Emulation.setDeviceMetricsOverride', { width: 1280, height: 900, deviceScaleFactor: 1, mobile: false });
  console.log(`== 對比（${CONTRAST_PAGES.length} 頁，桌機，展開全部 details）`);
  let nBad = 0;
  for (const u of CONTRAST_PAGES) {
    await tab.goto(BASE + u, 300);
    const b = await tab.eval(CONTRAST_FN);
    if (b.length) { nBad++; console.log(`FAIL ${u}  ${b.length} 種：${b.slice(0, 10).join('｜')}`); }
  }
  console.log(`對比：${CONTRAST_PAGES.length} 頁，有 <4.5 的 ${nBad} 頁`);
  fails += nBad;
}
await tab.close();
process.exit(fails ? 1 : 0);
