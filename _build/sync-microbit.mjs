// micro:bit 擴充積木表同步：從 pxt-mbc-sensors 原始碼（*.ts 的 //% block＋_locales/zh-TW）抽出每顆積木的
// 英文／繁中文字、參數、種類，寫成 _build/microbit-blocks.json（副本 commit 進 repo）。
// 網站的 micro:bit 積木文字一律從這份表來（protocol/*.json 的 microbit.js 只寫「用哪個 JavaScript 呼叫」），
// 擴充改名後重跑這支就會跟著改，不會再跟擴充實際的積木名稱不一樣。
//
// 用法：node _build/sync-microbit.mjs                 產生（有差異才覆寫）
//       node _build/sync-microbit.mjs --check         只比對，不一致時結束碼 1
//       node _build/sync-microbit.mjs --src <資料夾>  改用別的擴充資料夾
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const BUILD = path.dirname(fileURLToPath(import.meta.url));
export const EXT_SRC = 'D:\\AI_Agent_AI助理\\02_MBC產品開發\\09_圖控軟體\\圖控軟體開發_microbit擴充\\pxt-mbc-sensors';
export const BLOCKS_DST = path.join(BUILD, 'microbit-blocks.json');

const argv = process.argv.slice(2);
const CHECK = argv.includes('--check');
const srcIdx = argv.indexOf('--src');
const SRC = srcIdx > -1 ? path.resolve(argv[srcIdx + 1]) : EXT_SRC;

if (!fs.existsSync(path.join(SRC, 'pxt.json'))) {
  console.error(`找不到擴充：${SRC}\\pxt.json`);
  process.exit(2);
}
const pxt = JSON.parse(fs.readFileSync(path.join(SRC, 'pxt.json'), 'utf8'));
const loc = JSON.parse(fs.readFileSync(path.join(SRC, `_locales/zh-TW/${pxt.name}-strings.json`), 'utf8'));
const tsFiles = pxt.files.filter((f) => f.endsWith('.ts'));

const attr = (ann, k) => { const m = ann.match(new RegExp(`\\b${k}=(?:"([^"]*)"|(\\S+))`)); return m ? (m[1] !== undefined ? m[1] : m[2]) : null; };
const blocks = {};
const enums = {};
const errs = [];
for (const f of tsFiles) {
  const lines = fs.readFileSync(path.join(SRC, f), 'utf8').split(/\r?\n/);
  let ann = [];
  let curEnum = null;
  for (const raw of lines) {
    const ln = raw.trim();
    if (ln.startsWith('//%')) { ann.push(ln.slice(3).trim()); continue; }
    const em = ln.match(/^(?:export\s+)?enum\s+(\w+)/);
    if (em) { curEnum = em[1]; ann = []; continue; }
    if (curEnum) {
      if (ln.startsWith('}')) { curEnum = null; ann = []; continue; }
      const mm = ln.match(/^(\w+)\s*=/);
      if (mm) {
        const k = `${curEnum}.${mm[1]}`;
        const en = attr(ann.join(' '), 'block');
        if (en == null) errs.push(`${f}：${k} 沒有 //% block`);
        enums[k] = { en, zh: loc[`${k}|block`] || en };
        ann = [];
      }
      continue;
    }
    const fm = ln.match(/^export\s+function\s+(\w+)\s*\(([^)]*)\)\s*(?::\s*([\w\[\]]+))?/);
    if (fm) {
      const a = ann.join(' ');
      const en = attr(a, 'block');
      if (en != null && !/blockHidden=true/.test(a)) {
        const params = fm[2].split(',').map((s) => s.trim()).filter(Boolean).map((s) => s.split(':')[0].trim());
        const ret = fm[3] || 'void';
        const kind = ret === 'boolean' ? 'boolean' : ret === 'void' ? (params.includes('handler') ? 'event' : 'command') : 'reporter';
        const zh = loc[`mbc.${fm[1]}|block`];
        if (!zh) errs.push(`${f}：mbc.${fm[1]} 沒有繁中翻譯`);
        blocks[fm[1]] = { file: f, blockId: attr(a, 'blockId'), kind, params, en, zh: zh || en, group: attr(a, 'group'), advanced: /advanced=true/.test(a) };
      }
      ann = [];
      continue;
    }
    if (ln && !ln.startsWith('*') && !ln.startsWith('/*') && !ln.startsWith('//')) ann = [];
  }
}
if (errs.length) { for (const e of errs) console.error(e); process.exit(2); }

const out = JSON.stringify({
  _note: '由 _build/sync-microbit.mjs 從 pxt-mbc-sensors 原始碼產生，請勿手改',
  extension: pxt.name, version: pxt.version, category: { en: 'MBC Sensors', zh: loc['{id:category}Mbc'] || 'MBC Sensors' },
  blocks, enums,
}, null, 2) + '\n';
const cur = fs.existsSync(BLOCKS_DST) ? fs.readFileSync(BLOCKS_DST, 'utf8') : null;
const n = Object.keys(blocks).length;
if (cur === out) { console.log(`microbit-blocks.json 與擴充 v${pxt.version} 一致（${n} 顆積木）`); process.exit(0); }
if (CHECK) { console.log(`未同步：microbit-blocks.json（擴充 v${pxt.version}，${n} 顆積木）`); process.exit(1); }
fs.writeFileSync(BLOCKS_DST, out);
console.log(`已同步：microbit-blocks.json（擴充 v${pxt.version}，${n} 顆積木、${Object.keys(enums).length} 個選項）`);
