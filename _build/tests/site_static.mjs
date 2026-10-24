// 三層導覽靜態檢查（不開瀏覽器）：讀 _site 的 HTML 比對 _build/generated/guides-index.json
//   node _build/tests/site_static.mjs                       只看檔案
//   node _build/tests/site_static.mjs --http http://localhost:8772   另外對切換列／格子的每個連結目標發 HTTP 要求（要 200）
// 有錯結束碼 1。檢查：
//   軟體頁（sensors/<p>/<host>/<sw>.html，中英）：切換列存在、data-p/h/s、三段目前值、aria-current、下拉連結目標存在且符合規則、
//     麵包屑、h2「安裝與設定／讀值／範例」（沒有範例要有說明）、範例區只有本軟體的 <details>
//   平台頁：② 軟體格子＝這款這個主機的軟體，連結存在
//   產品頁：① 選你的主機 格子＝這款可用的主機
//   全部頁：未展開的建置指令（<!-- setup: / examples: / include… / sw-grid / platform-grid / product-hero / finder -->）、{{佔位}}、安裝步驟的備用文字
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const BUILD = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const SITE = path.join(BUILD, '..', '_site');
const argv = process.argv.slice(2);
const httpIdx = argv.indexOf('--http');
const HTTP = httpIdx > -1 ? argv[httpIdx + 1].replace(/\/$/, '') : null;

const GI = JSON.parse(fs.readFileSync(path.join(BUILD, 'generated', 'guides-index.json'), 'utf8'));
const G = JSON.parse(fs.readFileSync(path.join(BUILD, 'guides.json'), 'utf8'));
const HOST = Object.fromEntries(GI.hosts.map((h) => [h.id, h]));
const SWDEF = {}; // host/sw -> guides.json 定義
for (const h of G.hosts) for (const s of h.software) SWDEF[`${h.id}/${s.id}`] = s;

const problems = [];
const bad = (page, msg) => problems.push(`${page}：${msg}`);
const read = (rel) => { const f = path.join(SITE, rel); return fs.existsSync(f) ? fs.readFileSync(f, 'utf8') : null; };
const decode = (s) => s.replace(/<[^>]+>/g, '').replace(/&amp;/g, '&').replace(/&lt;/g, '<').replace(/&gt;/g, '>').replace(/&quot;/g, '"').replace(/&#39;/g, "'").trim();
function urlToFile(u) {
  let p = decodeURIComponent(u.split('#')[0].split('?')[0]);
  if (p.endsWith('/')) p += 'index.html';
  return p.replace(/^\//, '');
}
const linkTargets = new Set();
function checkTarget(page, href, what) {
  if (!href.startsWith('/')) return;
  linkTargets.add(href.split('#')[0]);
  if (!fs.existsSync(path.join(SITE, urlToFile(href)))) bad(page, `${what} 連結目標不存在 ${href}`);
}

const pre = (lang) => (lang === 'en' ? '/en' : '');
const swUrl = (lang, p, h, s) => `${pre(lang)}/sensors/${p}/${h}/${s}.html`;
const hostUrl = (lang, p, h) => `${pre(lang)}/sensors/${p}/${h}/`;
const prodUrl = (lang, p) => `${pre(lang)}/sensors/${p}/`;
const hostsOf = (p) => Object.fromEntries(GI.products[p].hosts.map((h) => [h.id, h.sws]));
// 從某產品／主機挑最接近的頁：同軟體 > 只有一種軟體 > 平台頁
function bestFor(lang, p, h, s) {
  const hs = hostsOf(p)[h];
  if (!hs) return prodUrl(lang, p);
  if (s && hs.includes(s)) return swUrl(lang, p, h, s);
  if (hs.length === 1) return swUrl(lang, p, h, hs[0]);
  return hostUrl(lang, p, h);
}

function segs(html) {
  const nav = (html.match(/<nav class="gsw[^"]*"[^>]*>[\s\S]*?<\/nav>/) || [])[0];
  if (!nav) return null;
  const attrs = nav.match(/data-p="([^"]*)" data-h="([^"]*)"(?: data-s="([^"]*)")?/) || [];
  const out = { p: attrs[1], h: attrs[2], s: attrs[3], seg: {} };
  for (const m of nav.matchAll(/<details class="gsw-seg gsw-(\w)[^"]*">([\s\S]*?)<\/details>/g)) {
    const v = decode((m[2].match(/<span class="gsw-v">([\s\S]*?)<\/span>/) || [])[1] || '');
    const links = [...m[2].matchAll(/<a href="([^"]+)"( aria-current="page")?>([\s\S]*?)<\/a>/g)].map((a) => ({ href: a[1], cur: !!a[2], text: decode(a[3].replace(/<small>[\s\S]*?<\/small>/, '')) }));
    out.seg[m[1]] = { v, links };
  }
  return out;
}

let nSw = 0, nPlat = 0, nProd = 0;
const noExamples = [];
for (const lang of ['zh', 'en']) {
  for (const p of Object.keys(GI.products)) {
    const prod = GI.products[p];
    const hosts = hostsOf(p);
    // ---------- 產品頁
    {
      const rel = urlToFile(prodUrl(lang, p));
      const html = read(rel);
      if (!html) bad(rel, '產品頁不存在');
      else {
        nProd++;
        const pick = (html.match(/<div class="pick-grid pick-hosts">([\s\S]*?)<\/div><script/) || [])[1];
        if (!pick) bad(rel, '沒有「① 選你的主機」格子');
        else {
          const tiles = [...pick.matchAll(/<a class="pick-tile[^"]*" href="([^"]+)" data-host="([^"]+)"/g)].map((m) => ({ href: m[1], host: m[2] }));
          const got = tiles.map((t) => t.host).join(','), want = Object.keys(hosts).join(',');
          if (got !== want) bad(rel, `選主機格子 ${got} ≠ 可用主機 ${want}`);
          for (const t of tiles) { checkTarget(rel, t.href, '選主機格子'); if (t.href !== bestFor(lang, p, t.host, null)) bad(rel, `選主機格子 ${t.host} 連到 ${t.href}，預期 ${bestFor(lang, p, t.host, null)}`); }
        }
      }
    }
    for (const [h, sws] of Object.entries(hosts)) {
      // ---------- 平台頁
      {
        const rel = urlToFile(hostUrl(lang, p, h));
        const html = read(rel);
        if (!html) bad(rel, '平台頁不存在');
        else {
          nPlat++;
          const grid = (html.match(/<div class="pick-grid pick-sw[^"]*">([\s\S]*?)<\/div>/) || [])[1];
          if (!grid) bad(rel, '沒有「② 選你用的軟體」格子');
          else {
            const tiles = [...grid.matchAll(/<a class="pick-tile[^"]*" href="([^"]+)" data-sw="([^"]+)"/g)].map((m) => ({ href: m[1], sw: m[2] }));
            if (tiles.map((t) => t.sw).join(',') !== sws.join(',')) bad(rel, `軟體格子 ${tiles.map((t) => t.sw)} ≠ ${sws}`);
            for (const t of tiles) { checkTarget(rel, t.href, '軟體格子'); if (t.href !== swUrl(lang, p, h, t.sw)) bad(rel, `軟體格子 ${t.sw} 連到 ${t.href}`); }
          }
          const sg = segs(html);
          if (!sg) bad(rel, '平台頁沒有切換列');
          else for (const s of Object.values(sg.seg)) for (const l of s.links) checkTarget(rel, l.href, '切換列');
        }
      }
      // ---------- 軟體頁
      for (const s of sws) {
        const url = swUrl(lang, p, h, s);
        const rel = urlToFile(url);
        const html = read(rel);
        if (!html) { bad(rel, '軟體頁不存在'); continue; }
        nSw++;
        const sg = segs(html);
        if (!sg) { bad(rel, '沒有切換列'); continue; }
        if (sg.p !== p || sg.h !== h || sg.s !== s) bad(rel, `切換列 data-p/h/s = ${sg.p}/${sg.h}/${sg.s}`);
        const want = { p: prod.short[lang], h: HOST[h].name[lang], s: SWDEF[`${h}/${s}`].name[lang] };
        for (const k of ['p', 'h', 's']) {
          const seg = sg.seg[k];
          if (!seg) { bad(rel, `切換列缺第 ${k} 段`); continue; }
          if (seg.v !== want[k]) bad(rel, `切換列 ${k} 段目前值「${seg.v}」≠「${want[k]}」`);
          const cur = seg.links.filter((l) => l.cur);
          if (cur.length !== 1 || cur[0].href !== url) bad(rel, `切換列 ${k} 段 aria-current 不對（${cur.map((c) => c.href)}）`);
          for (const l of seg.links) checkTarget(rel, l.href, `切換列 ${k} 段`);
        }
        // 產品段：每款其他產品 → 同主機同軟體 > 平台頁 > 產品頁
        for (const l of sg.seg.p?.links || []) {
          const q = (l.href.match(/\/sensors\/([^/]+)\//) || [])[1];
          if (!q || !GI.products[q]) { bad(rel, `產品段連結 ${l.href} 認不出產品`); continue; }
          const exp = bestFor(lang, q, h, s);
          if (l.href !== exp) bad(rel, `產品段 ${q} 連到 ${l.href}，預期 ${exp}`);
        }
        // 主機段：只列這款的主機，同名軟體優先
        const hl = (sg.seg.h?.links || []).map((l) => (l.href.match(/\/sensors\/[^/]+\/([^/]+)\//) || [])[1]);
        if (hl.join(',') !== Object.keys(hosts).join(',')) bad(rel, `主機段 ${hl} ≠ ${Object.keys(hosts)}`);
        for (const l of sg.seg.h?.links || []) {
          const h2 = (l.href.match(/\/sensors\/[^/]+\/([^/]+)\//) || [])[1];
          const exp = bestFor(lang, p, h2, s);
          if (l.href !== exp) bad(rel, `主機段 ${h2} 連到 ${l.href}，預期 ${exp}`);
        }
        // 軟體段：這個主機的全部軟體＋平台總覽
        const sl = (sg.seg.s?.links || []).map((l) => l.href);
        const wantSl = [...sws.map((x) => swUrl(lang, p, h, x)), hostUrl(lang, p, h)];
        if (sl.join(',') !== wantSl.join(',')) bad(rel, `軟體段連結 ${sl} ≠ ${wantSl}`);
        // 麵包屑
        const bc = (html.match(/<nav class="breadcrumbs"[^>]*>([\s\S]*?)<\/nav>/) || [])[1];
        if (!bc) bad(rel, '沒有麵包屑');
        else {
          const hrefs = [...bc.matchAll(/<a href="([^"]+)"/g)].map((m) => m[1]);
          const wantBc = [lang === 'en' ? '/en/' : '/', prodUrl(lang, p), hostUrl(lang, p, h)];
          if (hrefs.join(',') !== wantBc.join(',')) bad(rel, `麵包屑 ${hrefs} ≠ ${wantBc}`);
          for (const x of hrefs) checkTarget(rel, x, '麵包屑');
        }
        // h2
        const ids = [...html.matchAll(/<h2 id="([^"]+)"/g)].map((m) => m[1]);
        const H = lang === 'en' ? { setup: 'setup', read: 'reading-values', ex: 'examples' } : { setup: '安裝與設定', read: '讀值', ex: '範例' };
        if (!ids.includes(H.setup)) bad(rel, '缺 h2「安裝與設定」');
        if (!ids.some((i) => i === H.read || i.startsWith(H.read))) {
          if (p !== 'exp6' && p !== 'servo6') bad(rel, '缺 h2「讀值」');
        }
        const article = (html.match(/<article[^>]*>([\s\S]*?)<\/article>/) || [])[1] || html;
        const exIds = ids.filter((i) => i === H.ex || i.startsWith(H.ex));
        if (!exIds.length) bad(rel, '缺 h2「範例」');
        else if (/class="[^"]*no-examples/.test(article)) noExamples.push(rel);
        for (const exId of exIds) {
          // 範例區只有本軟體的 details
          const i0 = article.indexOf(`<h2 id="${exId}"`);
          const i1 = article.indexOf('<h2 ', i0 + 5);
          const sec = article.slice(i0, i1 > -1 ? i1 : undefined);
          if (/no-examples/.test(sec)) continue;
          const own = new RegExp(SWDEF[`${h}/${s}`].summaryMatch?.[lang] || '(?!)', 'i');
          const others = sws.filter((x) => x !== s).map((x) => [x, new RegExp(SWDEF[`${h}/${x}`].summaryMatch?.[lang] || '(?!)', 'i')]);
          const sums = [...sec.matchAll(/<summary[^>]*>([\s\S]*?)<\/summary>/g)].map((m) => decode(m[1]));
          for (const t of sums) {
            if (own.test(t)) continue;
            const o = others.find(([, re]) => re.test(t));
            if (o) bad(rel, `範例區出現 ${o[0]} 的區塊「${t.slice(0, 50)}」`);
          }
          if (!/<details|<pre|<a /.test(sec)) bad(rel, '範例節是空的');
        }
      }
    }
  }
}

// ---------- 全部頁：未展開的指令與佔位
const DIRECTIVE = /<!--\s*(setup:|examples:|include:|include-fallback|\/include-fallback|sw-grid|platform-grid|product-hero|finder\b|pick\b)/;
const FALLBACK = /安裝步驟正在搬到這一頁|being moved onto this page/;
let nAll = 0;
(function walk(d) {
  for (const f of fs.readdirSync(d)) {
    const p = path.join(d, f);
    if (fs.statSync(p).isDirectory()) { if (f !== 'archive_homepage') walk(p); continue; }
    if (!f.endsWith('.html')) continue;
    nAll++;
    const rel = path.relative(SITE, p).replace(/\\/g, '/');
    const html = fs.readFileSync(p, 'utf8');
    const m = html.match(DIRECTIVE);
    if (m) bad(rel, `未展開的指令 ${m[0]}`);
    // {{x}} 佔位（排除 <pre>/<code>/<script> 內）
    const plain = html.replace(/<(pre|code|script|style)[\s\S]*?<\/\1>/g, '');
    const ph = plain.match(/\{\{\s*[\w.]+\s*\}\}/);
    if (ph) bad(rel, `殘留佔位 ${ph[0]}`);
    if (FALLBACK.test(html)) bad(rel, '出現「安裝步驟正在搬到這一頁」備用文字（setup include 沒解析到）');
  }
})(SITE);

// ---------- HTTP
let httpChecked = 0;
if (HTTP) {
  const list = [...linkTargets];
  const fails = [];
  // 本機 py http.server 同時太多連線會偶發 fetch failed：一次 6 個、失敗重試 3 次
  const head = async (u) => { let err; for (let t = 0; t < 3; t++) { try { return await fetch(HTTP + encodeURI(decodeURI(u)), { method: 'HEAD' }); } catch (e) { err = e; await new Promise((r) => setTimeout(r, 300)); } } throw err; };
  for (let i = 0; i < list.length; i += 6) {
    await Promise.all(list.slice(i, i + 6).map(async (u) => {
      try { const r = await head(u); httpChecked++; if (r.status !== 200) fails.push(`${u} → ${r.status}`); }
      catch (e) { fails.push(`${u} → ${e.message}`); }
    }));
  }
  for (const f of fails) problems.push(`HTTP：${f}`);
}

console.log(`軟體頁 ${nSw}、平台頁 ${nPlat}、產品頁 ${nProd}、全部 HTML ${nAll}；切換列／格子連結目標 ${linkTargets.size} 個${HTTP ? `，HTTP 檢查 ${httpChecked}` : ''}`);
console.log(`範例節只有說明（沒有範例程式）的軟體頁 ${noExamples.length} 頁`);
if (problems.length) { console.log(`問題 ${problems.length} 個：`); for (const p of problems) console.log('  ' + p); process.exit(1); }
console.log('全部通過');
