// 連結檢查：爬 _site/**/*.html 的 href／src，內部路徑檢查檔案存在、#錨點檢查目標頁有該 id；外部連結只列不查
// 用法：node _build/check-links.mjs                 檢查 ../_site，有錯結束碼 1
//       node _build/check-links.mjs --site <資料夾>  改檢查別的輸出資料夾
//       node _build/check-links.mjs --external       另外列出外部連結清單
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const BUILD = path.dirname(fileURLToPath(import.meta.url));
const argv = process.argv.slice(2);
const siteIdx = argv.indexOf('--site');
const SITE = path.resolve(siteIdx > -1 ? argv[siteIdx + 1] : path.join(BUILD, '..', '_site'));
const LIST_EXTERNAL = argv.includes('--external');

if (!fs.existsSync(SITE)) { console.error(`找不到 ${SITE}（先跑 cd _build && node build.mjs）`); process.exit(2); }

const htmlFiles = [];
(function walk(d) { for (const f of fs.readdirSync(d)) { const p = path.join(d, f); if (fs.statSync(p).isDirectory()) walk(p); else if (f.endsWith('.html')) htmlFiles.push(p); } })(SITE);

// 每頁的 id 集合（錨點檢查用）
const idCache = new Map();
function idsOf(file) {
  if (idCache.has(file)) return idCache.get(file);
  const ids = new Set();
  if (fs.existsSync(file)) for (const m of fs.readFileSync(file, 'utf8').matchAll(/\s(?:id|name)="([^"]+)"/g)) ids.add(decodeURIComponent(m[1]));
  idCache.set(file, ids);
  return ids;
}
// 網址路徑 → 實體檔案
function resolveTarget(urlPath) {
  let p = decodeURIComponent(urlPath);
  if (p.endsWith('/')) p += 'index.html';
  const abs = path.join(SITE, p);
  if (fs.existsSync(abs) && fs.statSync(abs).isDirectory()) return path.join(abs, 'index.html');
  return abs;
}

const errors = [];
const external = new Set();
let checked = 0;
for (const file of htmlFiles) {
  const html = fs.readFileSync(file, 'utf8');
  const pageUrl = '/' + path.relative(SITE, file).split(path.sep).join('/');
  const pageDir = path.posix.dirname(pageUrl);
  for (const m of html.matchAll(/\b(?:href|src)="([^"]*)"/g)) {
    let u = m[1].trim();
    if (!u || u.startsWith('javascript:') || u.startsWith('data:')) continue;
    // 互動教學頁的 <script> 用字串拼 src（例如 src="' + BLK.img[i] + '"），不是真的連結
    if (/'\s*\+|\+\s*'/.test(u)) continue;
    if (/^(https?:)?\/\//.test(u) || /^(mailto|tel|line):/.test(u)) { external.add(u); continue; }
    checked++;
    // 拆成路徑＋錨點（去掉 ?query）
    const hashIdx = u.indexOf('#');
    const frag = hashIdx > -1 ? u.slice(hashIdx + 1) : null;
    let p = hashIdx > -1 ? u.slice(0, hashIdx) : u;
    p = p.replace(/\?.*$/, '');
    let targetFile;
    if (!p) targetFile = file; // 同頁錨點
    else {
      const abs = p.startsWith('/') ? p : path.posix.join(pageDir, p);
      targetFile = resolveTarget(abs);
      if (!fs.existsSync(targetFile)) { errors.push(`${pageUrl} → ${u}：找不到檔案`); continue; }
    }
    if (frag !== null && frag !== '' && /\.html$/.test(targetFile)) {
      const id = decodeURIComponent(frag);
      if (!idsOf(targetFile).has(id)) errors.push(`${pageUrl} → ${u}：找不到錨點 #${id}`);
    }
  }
}

console.log(`檢查 ${htmlFiles.length} 頁、${checked} 個內部連結，外部連結 ${external.size} 個（只列不查）`);
if (LIST_EXTERNAL) [...external].sort().forEach((u) => console.log('  外部：' + u));
if (errors.length) {
  console.log(`錯誤 ${errors.length} 個：`);
  errors.forEach((e) => console.log('  ' + e));
  process.exit(1);
}
console.log('內部連結與錨點全部正確');
