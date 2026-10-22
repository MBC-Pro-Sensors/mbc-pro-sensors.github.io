// 產生器共用工具：讀 protocol JSON、products.json、中英文字、輸出／--check 比對
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

export const BUILD = path.dirname(fileURLToPath(import.meta.url));
export const ROOT = path.resolve(BUILD, '..');
export const PROTOCOL_DIR = path.join(BUILD, 'protocol');

// ---------------------------------------------------------------- 平台
// 六個上位機平台：固定順序、固定顏色（D8）
export const PLATFORMS = [
  { id: 'spike', color: '#F2C200', name: { zh: 'SPIKE', en: 'SPIKE' }, page: 'spike-examples', hostKey: 'spike' },
  { id: 'ev3', color: '#D62828', name: { zh: 'EV3', en: 'EV3' }, page: 'ev3-examples', hostKey: 'ev3' },
  { id: 'arduino', color: '#0ABAB5', name: { zh: 'Arduino I2C', en: 'Arduino I2C' }, page: 'arduino-i2c', hostKey: 'i2c' },
  { id: 'mblock', color: '#1E6FE8', name: { zh: 'mBlock', en: 'mBlock' }, page: 'mblock', hostKey: 'mblock' },
  { id: 'matrix', color: '#2E9E4F', name: { zh: 'MATRIX', en: 'MATRIX' }, page: 'matrix', hostKey: 'matrix' },
  { id: 'microbit', color: '#E8711E', name: { zh: 'micro:bit', en: 'micro:bit' }, page: 'microbit', hostKey: 'microbit' },
];
export const PLATFORM_BY_ID = Object.fromEntries(PLATFORMS.map((p) => [p.id, p]));

export const hostOn = (product, hostKey) => !!(product.hosts && product.hosts[hostKey]);

// ---------------------------------------------------------------- 讀檔
export function loadCommon() {
  return JSON.parse(fs.readFileSync(path.join(PROTOCOL_DIR, 'common.json'), 'utf8'));
}
export function loadProducts(only) {
  const files = fs.readdirSync(PROTOCOL_DIR).filter((f) => f.endsWith('.json') && f !== 'common.json').sort();
  const list = files.map((f) => JSON.parse(fs.readFileSync(path.join(PROTOCOL_DIR, f), 'utf8')));
  return only ? list.filter((p) => only.includes(p.id)) : list;
}
export function loadSiteProducts() {
  return JSON.parse(fs.readFileSync(path.join(BUILD, 'products.json'), 'utf8'));
}

// 網站用的產品鍵：協定 id → sensors/<key>（神攝手兩版共用 esp32cam 頁）
export const SITE_KEY = { cam_school: 'esp32cam', cam_wro: 'esp32cam' };
export const siteKey = (product) => SITE_KEY[product.id] || product.id;
// 同一個網站資料夾裡的第二款（神攝手 WRO 版）：頁面檔名加字尾，不互相覆蓋
export const SITE_SUFFIX = { cam_wro: '-wro' };
export const pageFile = (product, base) => `/sensors/${siteKey(product)}/${base}${SITE_SUFFIX[product.id] || ''}.md`;

// ---------------------------------------------------------------- 文字
export const L = (obj, lang) => (obj && typeof obj === 'object' ? (obj[lang] ?? obj.zh ?? '') : (obj ?? ''));
export const T = (lang, zh, en) => (lang === 'en' ? en : zh);
export const hex = (id) => `0x${String(id).toUpperCase().padStart(2, '0')}`;
export const idNum = (id) => parseInt(id, 16);

// 多路值（count）展開成每一路：{id:"10", key:"light", count:8} → [{id:"10", n:1}, …]
export function expandCount(v) {
  if (!v.count) return [{ ...v, n: null }];
  const base = idNum(v.id);
  return Array.from({ length: v.count }, (_, i) => ({ ...v, id: (base + i).toString(16).toUpperCase().padStart(2, '0'), n: i + 1, keyN: `${v.key}${i + 1}` }));
}
// 值的顯示名稱：第 N 路 → 第 1~8 路
export const valueName = (v, lang) => L(v.name, lang).replace(/\bN\b/, v.count ? `1~${v.count}` : 'N');
export const valueIdRange = (v) => (v.count ? `${v.id}~${(idNum(v.id) + v.count - 1).toString(16).toUpperCase().padStart(2, '0')}` : v.id);

// Markdown 表格
export function table(header, rows, align) {
  const a = align || header.map(() => ':---');
  return [`| ${header.join(' | ')} |`, `| ${a.join(' | ')} |`, ...rows.map((r) => `| ${r.map((c) => String(c ?? '').replace(/\|/g, '\\|').replace(/\r?\n/g, ' ')).join(' | ')} |`)].join('\n');
}

// 頁面連結（.md 路徑，build.mjs 會轉成 .html）
export const pageHref = (lang, p) => (lang === 'en' ? '/en' : '') + p;

// ---------------------------------------------------------------- 輸出／檢查
export function makeWriter({ check, label }) {
  const changed = [];
  const written = [];
  const norm = (s) => s.replace(/\r\n/g, '\n');
  return {
    write(rel, content) {
      const abs = path.join(ROOT, rel);
      const cur = fs.existsSync(abs) ? norm(fs.readFileSync(abs, 'utf8')) : null;
      if (cur === norm(content)) return false;
      changed.push(rel);
      if (!check) {
        fs.mkdirSync(path.dirname(abs), { recursive: true });
        fs.writeFileSync(abs, content);
        written.push(rel);
      }
      return true;
    },
    // 標記式局部覆寫：<!-- gen:name -->…<!-- /gen --> 之間換成 content；沒有標記回傳 null
    patch(rel, name, content) {
      const abs = path.join(ROOT, rel);
      if (!fs.existsSync(abs)) return null;
      const src = fs.readFileSync(abs, 'utf8');
      const nl = src.includes('\r\n') ? '\r\n' : '\n';
      // 允許空內容（兩個標記相鄰）與結尾標記前的縮排（側欄巢狀清單用）
      // 內容不能含有另一個 <!-- /gen -->（否則會吞掉下一段），所以逐行取到第一個結尾標記為止
      const re = new RegExp(`(<!-- gen:${name} -->)\\r?\\n(?:(?:(?![ \\t]*<!-- /gen -->)[^\\n]*\\r?\\n)*)([ \\t]*<!-- /gen -->)`);
      if (!re.test(src)) return null;
      const out = src.replace(re, (m, a, b) => `${a}${nl}${content.replace(/\r?\n/g, nl)}${nl}${b}`);
      if (out === src) return false;
      changed.push(`${rel} [gen:${name}]`);
      if (!check) { fs.writeFileSync(abs, out); written.push(`${rel} [gen:${name}]`); }
      return true;
    },
    done() {
      if (check) {
        if (changed.length) { console.log(`${label}：${changed.length} 個檔案與產生結果不一致`); changed.forEach((c) => console.log('  未同步：' + c)); process.exit(1); }
        console.log(`${label}：全部一致`);
      } else {
        console.log(`${label}：寫入 ${written.length} 個檔案`);
        written.forEach((c) => console.log('  ' + c));
      }
    },
  };
}
