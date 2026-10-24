// MATRIX Mini R4 片段：A 原廠 0x22 getColor(R~K)、B LauMo 大通道指標讀法（MATRIX大通道定案、MATRIX積木對照表）
import { L, T, valueName } from '../../gen-lib.mjs';

export function available(product) { return !!(product.hosts && product.hosts.matrix); }

const CPP = 'cpp';
const CH = ['', '', '', '', 'R', 'G', 'B', 'C', 'M', 'Y', 'K'];
export const chName = (cmd) => CH[cmd] || `?${cmd}`;
const OBJ = 'MiniR4.I2C1.MXColor';

// 大通道 byte n 讀法（指標：ColorID 歸零 → Blue 讀 1 byte → Red 前進）
export function bigHelpers(lang) {
  return `// ${T(lang, '大通道：讀第 n 個 byte（uint8）／第 n 個起的 2 byte（uint16 小端）', 'big channel: read byte n (uint8) / 2 bytes from n (uint16 little-endian)')}
uint8_t bigU8(uint8_t n) {
  ${OBJ}.getColorID();                 // ${T(lang, '指標回到 byte 0', 'pointer back to byte 0')}
  uint8_t v = ${OBJ}.getColor(B);      // ${T(lang, '讀 byte 0', 'read byte 0')}
  for (uint8_t i = 0; i < n; i++) { ${OBJ}.getColor(R); v = ${OBJ}.getColor(B); }   // Red = ${T(lang, '前進 1', 'advance 1')}
  return v;
}
uint16_t bigU16(uint8_t n) {
  ${OBJ}.getColorID();
  ${OBJ}.getColor(B);
  for (uint8_t i = 0; i < n; i++) ${OBJ}.getColor(R);
  return (uint16_t)${OBJ}.getS();      // Saturation = ${T(lang, '讀 2 byte', 'read 2 bytes')}
}
int16_t bigI16(uint8_t n) {
  ${OBJ}.getColorID();
  ${OBJ}.getColor(B);
  for (uint8_t i = 0; i < n; i++) ${OBJ}.getColor(R);
  return (int16_t)${OBJ}.getH();       // Hue = ${T(lang, '讀 2 byte 有正負', 'read signed 2 bytes')}
}`;
}

// 大通道尾段 byte 24~28 不在指標串流裡（Red 前進到 24 會繞回 0，bigU8(24 以上) 會讀錯格）：
// 24 用 ColorID 讀；25~28 用 C／M／Y／K 讀，但這四個積木同時會送 F1~F4 命令
// matrix.f 可以是單一個（"F1"）或範圍（"F1~F4" = 任一個都做同一件事，IMU 航向歸零）
const fList = (f) => { const m = /^F([1-4])~F([1-4])$/.exec(f || ''); return m ? Array.from({ length: m[2] - m[1] + 1 }, (_, i) => `F${+m[1] + i}`) : f ? [f] : []; };
const TAIL = { 24: ['getColorID()', null], 25: ['getColor(C)', 'F1'], 26: ['getColor(M)', 'F2'], 27: ['getColor(Y)', 'F3'], 28: ['getColor(K)', 'F4'] };
function tailCode(product, v, b, lang) {
  const t = !b.count && TAIL[b.byte];
  if (!t) return null;
  const act = t[1] && (product.commands || []).find((c) => c.matrix && fList(c.matrix.f).includes(t[1]));
  const warn = act
    ? T(lang, `；byte ${b.byte} 只在送 ${t[1]} 時回傳，這行會同時執行「${L(act.name, lang)}」`, `; byte ${b.byte} only comes back with ${t[1]}, so this line also runs "${L(act.name, lang)}"`)
    : '';
  return `int ${v.key} = ${OBJ}.${t[0]};   // ${valueName(v, lang)}${warn}`;
}

export function value(product, v, lang) {
  const m = v.matrix;
  if (!m || (!m.ch22 && !m.big)) {
    return { text: T(lang, 'MATRIX 0x22 與大通道都沒有這個值。', 'Not available over MATRIX 0x22 or the big channel.') };
  }
  const name = valueName(v, lang);
  const extra = [];
  let code = null;
  if (m.ch22) {
    const c = m.ch22;
    if (c.count) {
      code = `// A. ${T(lang, '原廠 0x22 積木', 'stock 0x22 blocks')}：${CH[c.cmd]}~${CH[c.cmd + c.count - 1]} = ${name}${c.note ? `（${c.note}）` : ''}\nint raw = ${OBJ}.getColor(${CH[c.cmd]});   // ${T(lang, '第 1 組', 'group 1')}`;
    } else {
      code = `// A. ${T(lang, '原廠 0x22 積木', 'stock 0x22 blocks')}：${CH[c.cmd]} = ${name}${c.offset ? ` + ${c.offset}` : ''}${c.note && c.note !== CH[c.cmd] ? `（${c.note}）` : ''}\nint ${v.key} = ${OBJ}.getColor(${CH[c.cmd]})${c.offset ? ` - ${c.offset}` : ''};`;
    }
  }
  if (m.big) {
    const b = m.big;
    const fn = b.type === 'u16' ? 'bigU16' : b.type === 'i16' ? 'bigI16' : 'bigU8';
    const cast = b.type === 'i8' ? '(int8_t)' : '';
    let bc;
    if (b.count) bc = `// B. ${T(lang, '大通道', 'big channel')} byte ${b.byte}~${b.byte + b.count * (b.type.endsWith('16') ? 2 : 1) - 1}：${name}\nint ${v.key}[${b.count}];\nfor (uint8_t i = 0; i < ${b.count}; i++) ${v.key}[i] = ${cast}${fn}(${b.byte} + i${b.type.endsWith('16') ? ' * 2' : ''});`;
    else bc = `// B. ${T(lang, '大通道', 'big channel')} byte ${b.byte}${b.type.endsWith('16') ? `~${b.byte + 1}` : ''}：${name}${b.offset ? ` + ${b.offset}` : ''}${b.note ? `（${b.note}）` : ''}\n${tailCode(product, v, b, lang) || `int ${v.key} = ${cast}${fn}(${b.byte})${b.offset ? ` - ${b.offset}` : ''};`}`;
    if (code) extra.push({ title: T(lang, 'B. 大通道（LauMo 函式庫）', 'B. big channel (LauMo library)'), code: bc, codeLang: CPP });
    else code = bc;
  }
  const where = [m.ch22 ? `0x22 ${m.ch22.count ? `${CH[m.ch22.cmd]}~${CH[m.ch22.cmd + m.ch22.count - 1]}` : CH[m.ch22.cmd]}${m.ch22.offset ? ` (+${m.ch22.offset})` : ''}` : T(lang, '0x22 沒有', 'no 0x22'), m.big ? `${T(lang, '大通道', 'big')} byte ${m.big.byte}${m.big.count ? `~${m.big.byte + m.big.count * (m.big.type.endsWith('16') ? 2 : 1) - 1}` : m.big.type.endsWith('16') ? `~${m.big.byte + 1}` : ''} ${m.big.type}${m.big.offset ? ` (+${m.big.offset})` : ''}` : T(lang, '大通道沒有', 'no big channel')].join(T(lang, '；', '; '));
  return { code, codeLang: CPP, extra, note: where + T(lang, '。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。', '. A and B need different library files and cannot be mixed; `bigU8()` helpers are on this product\'s MATRIX page.') };
}

export function command(product, c, lang) {
  if (!c.matrix || !c.matrix.f) return { text: T(lang, 'MATRIX 沒有這個命令（大通道只有 F1／F2 兩個動作）。', 'Not available on MATRIX (the big channel only carries the F1 / F2 actions).') };
  const fs = fList(c.matrix.f);
  const name = { F1: 'C', F2: 'M', F3: 'Y', F4: 'K' }[fs[0]];
  const any = fs.length > 1 ? T(lang, `任一個都可以，這裡用 ${fs[0]} =`, `any of them works, here ${fs[0]} =`) + ' ' : '';
  return { code: `${OBJ}.getColor(${name});   // ${T(lang, '大通道', 'big channel')} ${c.matrix.f}（${any}${{ C: 'Cyan', M: 'Magenta', Y: 'Yellow', K: 'Key' }[name]}）= ${L(c.name, lang)}${T(lang, '，回傳上一包的完成次數', ', returns the completion counter from the previous packet')}`, codeLang: CPP, note: T(lang, '只有大通道函式庫有這個動作；0x22 原廠函式庫沒有。', 'Big-channel library only; the stock 0x22 library has no actions.') };
}

export function status(product, lang) {
  const tail0 = (product.id === 'line16' || product.id === 'imu');
  if (tail0) return { code: `uint8_t st = ${OBJ}.getColorID();   // ${T(lang, '這款已流出：byte 24 維持 0', 'shipped product: byte 24 stays 0')}`, codeLang: CPP, note: T(lang, '已流出的大通道格式鎖住，狀態 byte 維持 0。', 'The shipped big-channel layout is locked; the status byte stays 0.') };
  return { code: `uint8_t st = ${OBJ}.getColorID();   // ${T(lang, '大通道 byte 24 = F3 狀態旗標', 'big channel byte 24 = F3 status flags')}\nbool ready = st & 1;\nuint8_t id  = ${OBJ}.getColor(Y);    // byte 27 ${T(lang, '裝置 ID', 'device ID')} ${product.addr}\nuint8_t hw0 = ${OBJ}.getColor(K);    // byte 28 hw[0] = ${product.hw0}`, codeLang: CPP, note: '' };
}

// ---------------------------------------------------------------- 依軟體（gen-guides.mjs 軟體頁用）
// sw = matrixblock（原廠 0x22）｜ bigchannel（LauMo 大通道）｜ mvision（神攝手串口）；讀不到回 null
export function bySoftware(sw, product, v, lang) {
  const m = v.matrix || {};
  const name = valueName(v, lang);
  if (sw === 'matrixblock') {
    const c = m.ch22;
    if (!c) return null;
    if (c.count) return { text: T(lang, `積木「顏色感應器 → ${CH[c.cmd]}~${CH[c.cmd + c.count - 1]}」${c.note ? `：${c.note}` : ''}`, `Blocks "colour sensor → ${CH[c.cmd]}~${CH[c.cmd + c.count - 1]}"${c.note ? `: ${c.note}` : ''}`), code: `int raw = ${OBJ}.getColor(${CH[c.cmd]});   // ${T(lang, '第 1 組', 'group 1')}`, codeLang: CPP };
    return { text: T(lang, `積木「顏色感應器 → ${CH[c.cmd]}」${c.offset ? `，讀到的數減 ${c.offset}` : ''}`, `Block "colour sensor → ${CH[c.cmd]}"${c.offset ? `, subtract ${c.offset}` : ''}`), code: `int ${v.key} = ${OBJ}.getColor(${CH[c.cmd]})${c.offset ? ` - ${c.offset}` : ''};   // ${name}`, codeLang: CPP };
  }
  if (sw === 'bigchannel') {
    const b = m.big;
    if (!b) return null;
    const fn = b.type === 'u16' ? 'bigU16' : b.type === 'i16' ? 'bigI16' : 'bigU8';
    const cast = b.type === 'i8' ? '(int8_t)' : '';
    const two = b.type.endsWith('16');
    const code = b.count
      ? `int ${v.key}[${b.count}];\nfor (uint8_t i = 0; i < ${b.count}; i++) ${v.key}[i] = ${cast}${fn}(${b.byte} + i${two ? ' * 2' : ''});   // ${name}`
      : tailCode(product, v, b, lang) || `int ${v.key} = ${cast}${fn}(${b.byte})${b.offset ? ` - ${b.offset}` : ''};   // ${name}`;
    const span = b.count ? `${b.byte}~${b.byte + b.count * (two ? 2 : 1) - 1}` : two ? `${b.byte}~${b.byte + 1}` : `${b.byte}`;
    return { code, codeLang: CPP, note: `${T(lang, '大通道', 'Big channel')} byte ${span}（${b.type}）${b.note ? T(lang, '；', '; ') + b.note : ''}` };
  }
  if (sw === 'mvision') {
    const mv = v.mvision;
    if (!mv) return null;
    const slot = mv.count ? `${mv.idx}~${mv.idx + mv.count - 1}` : `${mv.idx}`;
    return { text: T(lang, `MATRIXblock 的 mVision 積木讀第 ${slot} 格資料${mv.note ? `：${mv.note}` : ''}。`, `The MATRIXblock mVision block reads data slot ${slot}${mv.note ? `: ${mv.note}` : ''}.`) };
  }
  return null;
}
export function commandBySoftware(sw, product, c, lang) {
  if (sw !== 'bigchannel' || !c.matrix || !c.matrix.f) return null;
  return command(product, c, lang);
}
