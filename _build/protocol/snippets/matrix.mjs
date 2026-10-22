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
    else bc = `// B. ${T(lang, '大通道', 'big channel')} byte ${b.byte}${b.type.endsWith('16') ? `~${b.byte + 1}` : ''}：${name}${b.offset ? ` + ${b.offset}` : ''}${b.note ? `（${b.note}）` : ''}\nint ${v.key} = ${cast}${fn}(${b.byte})${b.offset ? ` - ${b.offset}` : ''};`;
    if (code) extra.push({ title: T(lang, 'B. 大通道（LauMo 函式庫）', 'B. big channel (LauMo library)'), code: bc, codeLang: CPP });
    else code = bc;
  }
  const where = [m.ch22 ? `0x22 ${m.ch22.count ? `${CH[m.ch22.cmd]}~${CH[m.ch22.cmd + m.ch22.count - 1]}` : CH[m.ch22.cmd]}${m.ch22.offset ? ` (+${m.ch22.offset})` : ''}` : T(lang, '0x22 沒有', 'no 0x22'), m.big ? `${T(lang, '大通道', 'big')} byte ${m.big.byte}${m.big.count ? `~${m.big.byte + m.big.count * (m.big.type.endsWith('16') ? 2 : 1) - 1}` : m.big.type.endsWith('16') ? `~${m.big.byte + 1}` : ''} ${m.big.type}${m.big.offset ? ` (+${m.big.offset})` : ''}` : T(lang, '大通道沒有', 'no big channel')].join(T(lang, '；', '; '));
  return { code, codeLang: CPP, extra, note: where + T(lang, '。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。', '. A and B need different library files and cannot be mixed; `bigU8()` helpers are on this product\'s MATRIX page.') };
}

export function command(product, c, lang) {
  if (!c.matrix || !c.matrix.f) return { text: T(lang, 'MATRIX 沒有這個命令（大通道只有 F1／F2 兩個動作）。', 'Not available on MATRIX (the big channel only carries the F1 / F2 actions).') };
  const name = { F1: 'C', F2: 'M', F3: 'Y', F4: 'K' }[c.matrix.f];
  return { code: `${OBJ}.getColor(${name});   // ${T(lang, '大通道', 'big channel')} ${c.matrix.f}（${{ C: 'Cyan', M: 'Magenta', Y: 'Yellow', K: 'Key' }[name]}）= ${L(c.name, lang)}${T(lang, '，回傳上一包的完成次數', ', returns the completion counter from the previous packet')}`, codeLang: CPP, note: T(lang, '只有大通道函式庫有這個動作；0x22 原廠函式庫沒有。', 'Big-channel library only; the stock 0x22 library has no actions.') };
}

export function status(product, lang) {
  const tail0 = (product.id === 'line16' || product.id === 'imu');
  if (tail0) return { code: `uint8_t st = ${OBJ}.getColorID();   // ${T(lang, '這款已流出：byte 24 維持 0', 'shipped product: byte 24 stays 0')}`, codeLang: CPP, note: T(lang, '已流出的大通道格式鎖住，狀態 byte 維持 0。', 'The shipped big-channel layout is locked; the status byte stays 0.') };
  return { code: `uint8_t st = ${OBJ}.getColorID();   // ${T(lang, '大通道 byte 24 = F3 狀態旗標', 'big channel byte 24 = F3 status flags')}\nbool ready = st & 1;\nuint8_t id  = ${OBJ}.getColor(Y);    // byte 27 ${T(lang, '裝置 ID', 'device ID')} ${product.addr}\nuint8_t hw0 = ${OBJ}.getColor(K);    // byte 28 hw[0] = ${product.hw0}`, codeLang: CPP, note: '' };
}
