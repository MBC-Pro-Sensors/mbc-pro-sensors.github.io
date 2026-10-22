// 協定單一真相來源同步：把 85_共用資源\MBC_LegoUART\protocol\*.json 複製到 _build\protocol\（副本 commit 進 repo）
//
// 用法：node _build/sync-protocol.mjs                 複製（有差異才覆寫）
//       node _build/sync-protocol.mjs --check         只比對，不一致時結束碼 1
//       node _build/sync-protocol.mjs --src <資料夾>  改用別的來源資料夾
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

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

const files = fs.readdirSync(SRC).filter((f) => f.endsWith('.json')).sort();
let diff = 0;
for (const f of files) {
  const src = fs.readFileSync(path.join(SRC, f), 'utf8');
  // 先確認是合法 JSON，壞檔不複製
  try { JSON.parse(src); } catch (e) { console.error(`${f}：不是合法 JSON（${e.message}）`); process.exit(2); }
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
