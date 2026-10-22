// 封包圖 SVG 樣板：I2C 時序、mBlock 3 byte、FD 整包、MATRIX 大通道 30 B、mVision
import { svg, C, MONO, W } from './values.svg.mjs';

const txt = (x, y, s, o = {}) => `<text x="${x}" y="${y}" font-size="${o.size || 12}" fill="${o.fill || C.ink}" text-anchor="${o.anchor || 'middle'}"${o.mono ? ` font-family='${MONO}'` : ''}${o.weight ? ` font-weight="${o.weight}"` : ''}>${s}</text>`;

// 一排 byte 方塊：items = [{label, sub, color}]
function row(x, y, items, bw = 44, bh = 34, gap = 4) {
  let s = '';
  items.forEach((it, i) => {
    const cx = x + i * (bw + gap);
    s += `<rect x="${cx}" y="${y}" width="${bw}" height="${bh}" rx="5" fill="${it.color || '#e6f6f5'}" stroke="${C.mute}"/>` +
      txt(cx + bw / 2, y + bh / 2 + 4, it.label, { size: 12, mono: true, weight: 700 });
    if (it.sub) s += txt(cx + bw / 2, y + bh + 14, it.sub, { size: 10, fill: C.mute });
  });
  return s;
}
const arrow = (x1, y1, x2, y2) => `<line x1="${x1}" y1="${y1}" x2="${x2}" y2="${y2}" stroke="${C.ink}" stroke-width="2" marker-end="url(#a)"/>`;
const DEFS = `<defs><marker id="a" markerWidth="8" markerHeight="8" refX="6" refY="4" orient="auto"><path d="M0 0 L8 4 L0 8 Z" fill="${C.ink}"/></marker></defs>`;
const HI = '#fff2bf', DATA = '#e6f6f5', META = '#e3e8ee';

export const PACKETS = {
  // I2C：寫編號 → 2 ms → 讀 2k byte
  i2c: () => svg(DEFS +
    txt(60, 40, 'HOST', { weight: 700, fill: C.mute }) + txt(60, 150, 'MODULE', { weight: 700, fill: C.mute }) +
    row(110, 22, [{ label: 'addr W', color: META }, { label: 'id', color: HI, sub: '01' }]) +
    arrow(170, 60, 170, 120) + txt(215, 95, 'wait 2 ms', { size: 11, fill: C.warn, mono: true }) +
    row(110, 128, [{ label: 'addr R', color: META }, { label: 'lo', color: DATA, sub: 'id' }, { label: 'hi', color: DATA }, { label: 'lo', color: DATA, sub: 'id+1' }, { label: 'hi', color: DATA }, { label: '…', color: DATA, sub: '≤16 values' }]) +
    txt(W / 2, 190, 'int16 little-endian · undefined id = -32768 (0x8000)', { size: 11, fill: C.mute, mono: true })),
  // mBlock 3 byte
  mblock: () => svg(DEFS +
    txt(60, 48, 'HOST', { weight: 700, fill: C.mute }) + row(110, 30, [{ label: 'id', color: HI, sub: 'e.g. 01' }]) +
    arrow(160, 70, 160, 100) +
    txt(60, 128, 'MODULE', { weight: 700, fill: C.mute }) + row(110, 110, [{ label: 'lo', color: DATA, sub: 'value' }, { label: 'hi', color: DATA }, { label: 'xor', color: META, sub: 'id^lo^hi' }]) +
    txt(W / 2, 180, 'UART 38400 8N1 · value = int16 LE · command E0~EE → [00][00][id] then runs', { size: 11, fill: C.mute, mono: true })),
  // FD 整包
  fd: () => svg(DEFS +
    txt(60, 48, 'HOST', { weight: 700, fill: C.mute }) + row(110, 30, [{ label: 'FD', color: HI }]) +
    arrow(160, 70, 160, 100) +
    txt(60, 128, 'MODULE', { weight: 700, fill: C.mute }) + row(110, 110, [{ label: 'AA', color: META, sub: 'head' }, { label: 'N', color: META, sub: 'count' }, { label: 'lo', color: DATA, sub: 'v[0]' }, { label: 'hi', color: DATA }, { label: '…', color: DATA, sub: 'v[N-1]' }, { label: 'xor', color: META, sub: 'AA^N^data' }]) +
    txt(W / 2, 180, 'values in ascending id order (defined ids only)', { size: 11, fill: C.mute, mono: true })),
  // MATRIX 大通道 30 B
  big30: () => {
    const bw = 13, gap = 2, x0 = 18;
    let s = '';
    for (let i = 0; i < 30; i++) {
      const color = i < 24 ? DATA : i === 24 ? HI : i === 29 ? '#ffd9d9' : META;
      s += `<rect x="${x0 + i * (bw + gap)}" y="60" width="${bw}" height="40" rx="2" fill="${color}" stroke="${C.mute}" stroke-width="0.8"/>`;
      if (i % 4 === 0 || i >= 24) s += txt(x0 + i * (bw + gap) + bw / 2, 114, String(i), { size: 8, mono: true, fill: C.mute });
    }
    const lab = (a, b, t, y) => txt(x0 + ((a + b + 1) / 2) * (bw + gap) - gap / 2, y, t, { size: 10, mono: true });
    s += lab(0, 23, 'data 0~23 (LE)', 48) + lab(24, 24, 'F3', 140) + lab(25, 26, 'F1 F2', 155) + lab(27, 28, 'ID hw0', 170) + lab(29, 29, 'CRC', 185);
    s += txt(W / 2, 22, 'write 0x21 → read 30 bytes @ 0x22 · CRC-8 poly 0x07 over 0~28', { size: 11, fill: C.mute, mono: true });
    return svg(s);
  },
  // mVision
  mvision: () => svg(DEFS +
    txt(60, 128, 'CAMERA', { weight: 700, fill: C.mute }) + row(110, 110, [{ label: 'AA', color: META, sub: 'head' }, { label: 'N', color: META, sub: '≤10' }, { label: 'lo', color: DATA, sub: 'data0' }, { label: 'hi', color: DATA }, { label: '…', color: DATA, sub: 'data9' }, { label: 'xor', color: META, sub: 'AA^N^data' }]) +
    txt(W / 2, 50, 'Serial1 115200 · one packet every 20 ms · MATRIXblock "mVision data 0~9"', { size: 11, fill: C.mute, mono: true }) +
    txt(W / 2, 180, 'uint16 little-endian', { size: 11, fill: C.mute, mono: true })),
  // LUMP（EV3／SPIKE 串口）模式表：主機選模式 → 模組持續送 DATA 封包
  'lump-modes': () => {
    let s = DEFS + txt(60, 40, 'HOST', { weight: 700, fill: C.mute }) + txt(60, 150, 'MODULE', { weight: 700, fill: C.mute });
    s += row(110, 22, [{ label: '43', color: HI, sub: 'SELECT' }, { label: 'mode', color: HI, sub: '0~9' }, { label: 'chk', color: META }]);
    s += arrow(170, 60, 170, 120) + txt(230, 95, 'every ~10 ms', { size: 11, fill: C.warn, mono: true });
    s += row(110, 128, [{ label: 'C0|m', color: META, sub: 'DATA' }, { label: 'v0', color: DATA, sub: 'int8/16/32' }, { label: 'v1', color: DATA }, { label: '…', color: DATA }, { label: 'chk', color: META, sub: 'xor FF' }]);
    s += txt(W / 2, 190, 'EV3 57600 · SPIKE 115200 · modes listed per product (mode 0 = default view)', { size: 11, fill: C.mute, mono: true });
    return svg(s);
  },
  // SPIKE 官方 App 模式 0：假裝成顏色感應器 RGB，每個 byte 塞不同值
  'spike-mode0': () => {
    let s = DEFS + txt(W / 2, 28, 'SPIKE app reads mode 0 as a colour sensor', { size: 12, weight: 700 });
    s += row(60, 50, [{ label: 'b0', color: DATA, sub: 'red' }, { label: 'b1', color: DATA, sub: 'green' }, { label: 'b2', color: DATA, sub: 'blue' }, { label: 'b3', color: DATA, sub: 'reflect' }, { label: 'b4', color: DATA, sub: 'ambient' }, { label: 'b5', color: META, sub: 'colour' }, { label: 'b6', color: META, sub: '-' }, { label: 'b7', color: META, sub: '-' }], 42);
    s += txt(W / 2, 130, 'each byte carries one product value (0~100)', { size: 12, fill: C.warn, mono: true });
    s += txt(W / 2, 156, 'read them with the colour-sensor blocks: red / green / blue / reflected / ambient', { size: 11, fill: C.mute });
    s += txt(W / 2, 182, 'exact byte map: see each product reference page', { size: 11, fill: C.mute, mono: true });
    return svg(s);
  },
};
export function render(name) { return PACKETS[name] ? PACKETS[name]() : null; }
