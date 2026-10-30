// 協定單一真相來源同步：把 85_共用資源\MBC_LegoUART\protocol\*.json 複製到 _build\protocol\（副本 commit 進 repo）
//
// 用法：node _build/sync-protocol.mjs                 複製（有差異才覆寫）
//       node _build/sync-protocol.mjs --check         只比對，不一致時結束碼 1
//       node _build/sync-protocol.mjs --src <資料夾>  改用別的來源資料夾
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { HIDE_BIG } from './site-flags.mjs';

const BUILD = path.dirname(fileURLToPath(import.meta.url));
export const PROTOCOL_SRC = 'D:\\AI_Agent_AI助理\\02_MBC產品開發\\85_共用資源\\MBC_LegoUART\\protocol';
export const PROTOCOL_DST = path.join(BUILD, 'protocol');

const argv = process.argv.slice(2);
const CHECK = argv.includes('--check');
const srcIdx = argv.indexOf('--src');
const SRC = srcIdx > -1 ? path.resolve(argv[srcIdx + 1]) : PROTOCOL_SRC;

if (!fs.existsSync(SRC)) {
  console.error(`找不到來源資料夾：${SRC}`);
  process.exit(2);
}
fs.mkdirSync(PROTOCOL_DST, { recursive: true });

// site-flags.json hideMatrixBigChannel：MATRIX 大通道只給特定客戶，網站副本拿掉所有大通道資料與字句（正本不動）
const BIG_TEXT = /大通道|big[- ]channel|LauMo|bigU\d/i;
const BIG_RULES = [
  [/R4 大通道版：/g, 'R4 版：'], [/R4 大通道版/g, 'R4 版'],
  [/；MATRIX 大通道 byte 24 同一個 byte/g, ''], [/; same byte as MATRIX big-channel byte 24/g, ''],
  [/（MATRIX 大通道 byte 24 同一個 byte）/g, ''], [/ \(same byte as MATRIX big-channel byte 24\)/g, ''],
  [/；16 路大通道尾段鎖住為 0，不進大通道/g, ''], [/; not in the MATRIX big channel \(tail bytes locked at 0 for the 16-ch\)/g, ''],
  [/、大通道 \[\d+\.\.\d+\]/g, ''], [/, SPIKE mode 6 and big-channel \[\d+\.\.\d+\] carry/g, ' and SPIKE mode 6 carry'],
  [/／大通道/g, ''], [/大通道／/g, ''],
];
function hideBig(o) {
  if (Array.isArray(o)) {
    // 字串陣列（assumptions）：套規則後還提到大通道的整條拿掉
    return o.map(hideBig).filter((x) => !(typeof x === 'string' && BIG_TEXT.test(x)));
  }
  if (o && typeof o === 'object') {
    const out = {};
    for (const [k, v] of Object.entries(o)) {
      if (k === 'big') continue;                                  // values[].matrix.big、hosts.matrix.big、lego.matrixLocked.big
      out[k] = hideBig(v);
    }
    // commands[].matrix.f（大通道 F1~F4 動作）連同說明一起拿掉
    if (out.matrix && out.matrix.f) { delete out.matrix.f; delete out.matrix.note; if (!Object.keys(out.matrix).length) delete out.matrix; }
    return out;
  }
  if (typeof o === 'string') { let s = o; for (const [re, to] of BIG_RULES) s = s.replace(re, to); return s; }
  return o;
}
function render(src) {
  if (!HIDE_BIG) return src;
  const d = hideBig(JSON.parse(src));
  const left = JSON.stringify(d).match(new RegExp(`.{0,40}(${BIG_TEXT.source}).{0,40}`, 'i'));
  if (left) { console.error(`大通道字句沒清乾淨：…${left[0]}…（請在 sync-protocol.mjs BIG_RULES 加規則）`); process.exit(2); }
  return JSON.stringify(d, null, 2) + '\n';
}

// 有頂層 layout 的產品（神攝手 unicam：0x10 起的值表依相機模式而變）不進 gen-reference 的通用頁，網站頁由 gen-unicam.mjs 產生 → 不複製
// 頂層 site: false（停產不上網站，例：循行者 5 路 line5）→ 不複製
const files = fs.readdirSync(SRC).filter((f) => {
  if (!f.endsWith('.json')) return false;
  const d = JSON.parse(fs.readFileSync(path.join(SRC, f), 'utf8'));
  return !d.layout && d.site !== false;
}).sort();
let diff = 0;
for (const f of files) {
  let src = fs.readFileSync(path.join(SRC, f), 'utf8');
  // 先確認是合法 JSON，壞檔不複製
  try { JSON.parse(src); } catch (e) { console.error(`${f}：不是合法 JSON（${e.message}）`); process.exit(2); }
  src = render(src);
  const dst = path.join(PROTOCOL_DST, f);
  const cur = fs.existsSync(dst) ? fs.readFileSync(dst, 'utf8') : null;
  if (cur === src) continue;
  diff++;
  if (CHECK) console.log(`未同步：protocol/${f}`);
  else { fs.writeFileSync(dst, src); console.log(`已同步：protocol/${f}`); }
}
// 來源已刪除的檔，副本也要拿掉
for (const f of fs.readdirSync(PROTOCOL_DST).filter((f) => f.endsWith('.json'))) {
  if (files.includes(f)) continue;
  diff++;
  if (CHECK) console.log(`來源已不存在：protocol/${f}`);
  else { fs.rmSync(path.join(PROTOCOL_DST, f)); console.log(`已刪除：protocol/${f}`); }
}
if (!diff) console.log(`protocol/ 與來源一致（${files.length} 個檔）`);
process.exit(CHECK && diff ? 1 : 0);
