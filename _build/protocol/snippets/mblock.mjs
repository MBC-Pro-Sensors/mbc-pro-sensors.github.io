// mBlock 5 片段：scratchblocks 積木文字＋新版 3 byte 封包（mBlock欄位定案）
import { L, T, hex, valueName } from '../../gen-lib.mjs';

export function available(product) { return !!(product.hosts && product.hosts.mblock); }

// 積木前綴：與擴展 build_mext.js 的產品前綴完全一致（2026-10-22 對齊；8／16 路與 PS2／PS4 共用一組積木）
const PREFIX = {
  line8: { zh: '循行者', en: 'Pathfinder' }, line16: { zh: '循行者', en: 'Pathfinder' },
  tof2: { zh: '測距者 2 路', en: 'Ranger 2' }, tof8: { zh: '測距者 8 路', en: 'Ranger 8' },
  ps2: { zh: '掌控者', en: 'Controller' }, ps4: { zh: '掌控者', en: 'Controller' },
  imu: { zh: 'IMU 陀螺儀', en: 'IMU Gyro' },
  cam_school: { zh: '神攝手舊版', en: 'Sharpshooter Vision (legacy)' }, cam_wro: { zh: '神攝手舊版 WRO', en: 'Sharpshooter Vision WRO (legacy)' },
};
export function category(product, lang) {
  const p = PREFIX[product.id];
  if (p) return L(p, lang);
  return L(product.name, lang).replace(/\s*\d+\s*路$/, '').replace(/\s*\d+-ch$/, '').replace(/\s*\(.*\)$/, '').trim();
}
// 擴展的連接埠選單只放數字：「循行者 連接埠 [1] 線的位置」
const PORT = (lang) => T(lang, '連接埠 [1 v]', 'port [1 v]');

// 積木文字（純文字，表格用）
export function blockText(product, v, lang) {
  if (!v.mblock || !v.mblock.block) return null;
  return `${category(product, lang)} ${T(lang, '連接埠 ［埠］', 'port [port]')} ${L(v.mblock.block, lang).replace('[N]', T(lang, '［1~' + (v.count || 1) + '］', '[1~' + (v.count || 1) + ']'))}`;
}
export function commandText(product, c, lang) {
  if (!c.mblock) return null;
  const params = (c.params || []).map((p) => T(lang, `［${L(p.name, lang)}］`, `[${L(p.name, lang)}]`)).join(' ');
  return `${category(product, lang)} ${T(lang, '連接埠 ［埠］', 'port [port]')} ${L(c.name, lang)}${params ? ' ' + params : ''}`;
}

// scratchblocks 區塊
const blocks = (txt) => `<pre class="blocks">${txt}</pre>`;

export function value(product, v, lang) {
  if (!v.mblock || !v.mblock.block) return null;
  const label = L(v.mblock.block, lang).replace('[N]', '[1 v]');
  const reporter = `(${category(product, lang)} ${PORT(lang)} ${label} :: sensing)`;
  const first = v.count ? hex(v.id) : hex(v.id);
  const bytes = T(lang,
    `主機送 \`[${first.slice(2)}]\` → 回 \`[值低][值高][${first.slice(2)} ^ 值低 ^ 值高]\`，值 = int16 小端${v.count ? `；第 N 路的編號 = ${hex(v.id)} + (N − 1)` : ''}。`,
    `Host sends \`[${first.slice(2)}]\` → reply \`[lo][hi][${first.slice(2)} ^ lo ^ hi]\`, value = int16 little-endian${v.count ? `; channel N uses id ${hex(v.id)} + (N − 1)` : ''}.`);
  return { html: blocks(reporter), note: `${T(lang, '積木傳回', 'Block returns')} ${valueName(v, lang)}（${v.range}${v.unit ? ' ' + v.unit : ''}）。${bytes}` };
}

export function command(product, c, lang) {
  if (!c.mblock) return null;
  const params = (c.params || []).map((p) => `(${sample(p)})`).join(' ');
  const stack = `${category(product, lang)} ${PORT(lang)} ${L(c.name, lang)}${params ? ' ' + params : ''} :: sensing`;
  const n = (c.params || []).reduce((s, p) => s + (p.bytes || 1), 0);
  const bytes = T(lang,
    `主機送 \`[${c.id}]${n ? '[參數…]' : ''}\` → 感應器回 \`[00][00][${c.id}]\`（收到了）再執行${c.busyFlag != null ? `；執行期間 F3 bit${c.busyFlag} = 1` : ''}。`,
    `Host sends \`[${c.id}]${n ? '[params…]' : ''}\` → reply \`[00][00][${c.id}]\` (ack), then it runs${c.busyFlag != null ? `; F3 bit${c.busyFlag} = 1 while busy` : ''}.`);
  return { html: blocks(stack), note: bytes };
}
function sample(p) {
  const m = String(p.range || '').match(/^(-?\d+)~(-?\d+)$/);
  return m ? Math.round((Number(m[1]) + Number(m[2])) / 2) : 0;
}

// 值表以外、擴展另外做的積木（照 build_mext.js 的「產品專屬布林」與每類一顆「更新全部數值」，2026-10-26 補）
//   循行者：第 N 路在線上？（拆 05 壓線位元圖）；掌控者：[按鈕] 被按下？（拆 01 十六鍵）
const PAD_BTN = { zh: '✕ ● ▲ ■ 上 下 左 右 L1 R1 L2 R2 L3 R3 SELECT START', en: '✕ ● ▲ ■ up down left right L1 R1 L2 R2 L3 R3 SELECT START' };
export function extraReporters(product, lang) {
  const out = [];
  const cat = category(product, lang);
  const bm = product.values.find((v) => v.key === 'bitmap');
  if (/^line(8|16)$/.test(product.id) && bm) {
    const n = product.hw0 || (product.id === 'line16' ? 16 : 8);
    out.push({ key: 'onLine', id: bm.id, name: T(lang, '第 N 路在線上？', 'channel N on line?'),
      text: `${cat} ${T(lang, '連接埠 ［埠］', 'port [port]')} ${T(lang, `第 ［1~${n}］ 路在線上？`, `channel [1~${n}] on line?`)}`,
      html: blocks(`<${cat} ${PORT(lang)} ${T(lang, '第 [1 v] 路在線上？', 'channel [1 v] on line?')} :: sensing>`),
      desc: T(lang, `壓線位元圖 ${bm.id} 的第 N 個 bit（第 1 路 = bit0，最左）：那一路壓到線 = 是`, `bit N−1 of line bitmap ${bm.id} (channel 1 = bit0, leftmost): true when that channel sees the line`) });
  }
  const b16 = product.values.find((v) => v.key === 'buttons16');
  if (/^ps[24]$/.test(product.id) && b16) {
    out.push({ key: 'pressed', id: b16.id, name: T(lang, '［按鈕］ 被按下？', '[button] pressed?'),
      text: `${cat} ${T(lang, '連接埠 ［埠］', 'port [port]')} ${T(lang, '［按鈕］ 被按下？', '[button] pressed?')}`,
      html: blocks(`<${cat} ${PORT(lang)} [■ v] ${T(lang, '被按下？', 'pressed?')} :: sensing>`),
      desc: T(lang, `從十六鍵 ${b16.id} 拆出的單一按鈕（PSX 排列），選單：${PAD_BTN.zh}`, `one button split out of 16 buttons ${b16.id} (PSX order); menu: ${PAD_BTN.en}`) });
  }
  return out;
}
export function extraCommands(product, lang) {
  const cat = category(product, lang);
  return [{ key: 'readAll', id: 'FD', name: T(lang, '更新全部數值', 'update all values'),
    text: `${cat} ${T(lang, '連接埠 ［埠］', 'port [port]')} ${T(lang, '更新全部數值', 'update all values')}`,
    html: blocks(`${cat} ${PORT(lang)} ${T(lang, '更新全部數值', 'update all values')} :: sensing`),
    desc: T(lang, '一次讀整張值表進快取（FD 整包）；接下來 10 ms 內的讀值積木直接用快取，不再逐一詢問', 'reads the whole value table into a cache (FD pack-all); reporter blocks within the next 10 ms use the cache instead of asking one by one') }];
}

export function status(product, lang) {
  const ok = `<${category(product, lang)} ${PORT(lang)} ${T(lang, '連線正常？', 'connected?')} :: sensing>`;
  return { html: blocks(ok), note: T(lang, '狀態旗標 F3 的各 bit 由擴展拆成布林積木（就緒、校準中…）。', 'The extension splits status flags F3 into boolean blocks (ready, calibrating, …).') };
}

// 整包 FD
export function packAll(product, lang) {
  const n = product.values.reduce((s, v) => s + (v.count || 1), 0);
  return T(lang,
    `送 \`[FD]\` → 回 \`[AA][${n.toString(16).toUpperCase().padStart(2, '0')}][${n} 個 int16 小端][xor]\`，xor = AA ^ N ^ 所有資料 byte；內容 = 本頁值表由小到大排（不含共用編號與命令）。擴展用它一次抓全部、10 ms 內的積木直接讀快取。`,
    `Send \`[FD]\` → reply \`[AA][${n.toString(16).toUpperCase().padStart(2, '0')}][${n} × int16 LE][xor]\`, xor = AA ^ N ^ all data bytes; content = this value table in ascending id order (shared ids and commands excluded). The extension fetches it once and serves blocks from a 10 ms cache.`);
}

// ---------------------------------------------------------------- 依軟體（gen-guides.mjs 軟體頁用；這個平台只有 mblock5）
export function bySoftware(sw, product, v, lang) { return sw === 'mblock5' ? value(product, v, lang) : null; }
export function commandBySoftware(sw, product, c, lang) { return sw === 'mblock5' ? command(product, c, lang) : null; }
