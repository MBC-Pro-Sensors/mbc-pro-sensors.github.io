// 接線圖 SVG 樣板：主機剪影（有接頭／腳位辨識特徵）＋產品方塊＋四線（SDA/SCL/5V/GND）或一條樂高線
import { svg, C, MONO, W, H } from './values.svg.mjs';

const txt = (x, y, s, o = {}) => `<text x="${x}" y="${y}" font-size="${o.size || 13}" fill="${o.fill || C.ink}" text-anchor="${o.anchor || 'middle'}"${o.mono ? ` font-family='${MONO}'` : ''}${o.weight ? ` font-weight="${o.weight}"` : ''}>${s}</text>`;

// 各主機：標籤、四線腳位（或 cable 名稱）、剪影顏色
export const HOSTS = {
  uno: { label: 'Arduino UNO / Nano', pins: { SDA: 'A4', SCL: 'A5', '5V': '5V', GND: 'GND' }, color: '#0ABAB5' },
  esp32: { label: 'ESP32', pins: { SDA: 'GPIO 21', SCL: 'GPIO 22', '5V': '5V', GND: 'GND' }, color: '#0ABAB5' },
  r4: { label: 'Arduino UNO R4', pins: { SDA: 'SDA', SCL: 'SCL', '5V': '5V', GND: 'GND' }, color: '#0ABAB5' },
  microbit: { label: 'micro:bit', pins: { SDA: 'P20', SCL: 'P19', '5V': '5V ext.', GND: 'GND' }, color: '#E8711E' },
  matrix: { label: 'MATRIX Mini R4', pins: { SDA: 'I2C1 SDA', SCL: 'I2C1 SCL', '5V': '5V', GND: 'GND' }, color: '#2E9E4F' },
  mbot: { label: 'mBot / Ranger (RJ25)', pins: { TX: 'S1', RX: 'S2', '5V': '5V', GND: 'GND' }, color: '#1E6FE8', uart: true },
  spike: { label: 'SPIKE Prime', cable: 'LPF2 cable', port: 'Port A~F', color: '#F2C200' },
  ev3: { label: 'EV3', cable: 'RJ12 cable', port: 'Port 1~4', color: '#D62828' },
};
const WIRE = { SDA: '#2E9E4F', SCL: '#1E6FE8', TX: '#2E9E4F', RX: '#1E6FE8', '5V': '#D62828', GND: '#1f2933' };

// 主機剪影：畫在 (24,40)~(194,160) 的框裡
const BX = 24, BY = 40, BW = 170, BH = 120;
const pinRow = (x, y, n, dx = 7) => Array.from({ length: n }, (_, i) => `<rect x="${x + i * dx}" y="${y}" width="4" height="4" fill="#1f2933" opacity="0.6"/>`).join('');
const ledGrid = (x, y, c, gap = 9, size = 5, color = '#1f2933') => { let s = ''; for (let i = 0; i < 5; i++) for (let j = 0; j < 5; j++) s += `<rect x="${x + i * gap}" y="${y + j * gap}" width="${size}" height="${size}" rx="1" fill="${color}" opacity="${(i + j) % 3 === 0 ? 0.9 : 0.3}"/>`; return s; };
const SIL = {
  // UNO：藍板、USB-B 方塊、電源座、上下排針、A4/A5
  uno: (c) => `<rect x="${BX + 10}" y="${BY + 20}" width="150" height="86" rx="6" fill="#1E6FE8" opacity="0.18" stroke="#1E6FE8" stroke-width="2"/>` +
    `<rect x="${BX + 2}" y="${BY + 30}" width="16" height="18" rx="2" fill="#8a97a3"/>` + `<rect x="${BX + 2}" y="${BY + 76}" width="14" height="16" rx="2" fill="#1f2933"/>` +
    pinRow(BX + 60, BY + 22, 14) + pinRow(BX + 70, BY + 100, 12) + `<rect x="${BX + 118}" y="${BY + 97}" width="22" height="8" rx="2" fill="#0ABAB5"/>` +
    txt(BX + 129, BY + 92, 'A4 A5', { size: 8, mono: true }) + `<rect x="${BX + 70}" y="${BY + 50}" width="40" height="24" rx="2" fill="#1f2933" opacity="0.75"/>` + txt(BX + 40, BY + 66, 'UNO', { size: 11, weight: 700, fill: '#1E6FE8' }),
  // ESP32：窄板、兩側排針、金屬屏蔽罩＋天線、micro USB
  esp32: (c) => `<rect x="${BX + 50}" y="${BY + 6}" width="70" height="112" rx="6" fill="#1f2933" opacity="0.12" stroke="#1f2933" stroke-width="1.5"/>` +
    `<rect x="${BX + 62}" y="${BY + 12}" width="46" height="40" rx="3" fill="#8a97a3"/>` + `<path d="M${BX + 66} ${BY + 16} l6 0 l0 6 l6 0 l0 -6 l6 0 l0 6 l6 0 l0 -6 l6 0" fill="none" stroke="#fff" stroke-width="1.5"/>` +
    Array.from({ length: 15 }, (_, i) => `<rect x="${BX + 52}" y="${BY + 10 + i * 7}" width="4" height="4" fill="#1f2933" opacity="0.6"/><rect x="${BX + 114}" y="${BY + 10 + i * 7}" width="4" height="4" fill="#1f2933" opacity="0.6"/>`).join('') +
    `<rect x="${BX + 76}" y="${BY + 108}" width="18" height="8" rx="2" fill="#8a97a3"/>` + txt(BX + 85, BY + 80, '21', { size: 9, mono: true }) + txt(BX + 85, BY + 94, '22', { size: 9, mono: true }) +
    `<rect x="${BX + 70}" y="${BY + 58}" width="30" height="12" rx="2" fill="#1f2933" opacity="0.7"/>`,
  // UNO R4：同 UNO 外型，多 Qwiic 接頭與 SDA/SCL 獨立腳
  r4: (c) => SIL.uno(c).replace('UNO</text>', 'R4</text>') + `<rect x="${BX + 150}" y="${BY + 56}" width="12" height="16" rx="2" fill="#0ABAB5"/>` + txt(BX + 156, BY + 82, 'Qwiic', { size: 7, mono: true }) + txt(BX + 100, BY + 15, 'SCL SDA', { size: 8, mono: true }),
  // micro:bit：圓角板、5×5 LED、A/B 鍵、金手指邊緣 0 1 2 3V GND
  microbit: (c) => `<rect x="${BX + 20}" y="${BY + 10}" width="130" height="100" rx="12" fill="${c}" opacity="0.15" stroke="${c}" stroke-width="2"/>` +
    ledGrid(BX + 65, BY + 24, c, 9, 5, '#D62828') + `<circle cx="${BX + 40}" cy="${BY + 46}" r="7" fill="#1f2933" opacity="0.8"/><circle cx="${BX + 130}" cy="${BY + 46}" r="7" fill="#1f2933" opacity="0.8"/>` +
    txt(BX + 40, BY + 49, 'A', { size: 8, fill: '#fff', weight: 700 }) + txt(BX + 130, BY + 49, 'B', { size: 8, fill: '#fff', weight: 700 }) +
    [['0', 34], ['1', 60], ['2', 86], ['3V', 112], ['GND', 138]].map(([n, x]) => `<rect x="${BX + x - 8}" y="${BY + 96}" width="16" height="14" rx="2" fill="#E8B400"/>` + txt(BX + x, BY + 106, n, { size: 7, weight: 700, mono: true })).join('') +
    Array.from({ length: 20 }, (_, i) => `<rect x="${BX + 22 + i * 6.5}" y="${BY + 90}" width="3" height="6" fill="#E8B400" opacity="0.7"/>`).join('') + txt(BX + 85, BY + 84, 'P19 P20 (edge)', { size: 7, mono: true, fill: C.mute }),
  // MATRIX Mini R4：綠板、I2C 4-pin 座、馬達／感測埠
  matrix: (c) => `<rect x="${BX + 14}" y="${BY + 14}" width="142" height="92" rx="8" fill="${c}" opacity="0.16" stroke="${c}" stroke-width="2"/>` +
    txt(BX + 85, BY + 48, 'MATRIX', { size: 13, weight: 700, fill: c }) + txt(BX + 85, BY + 62, 'Mini R4', { size: 9, fill: c }) +
    [0, 1, 2, 3].map((i) => `<rect x="${BX + 22 + i * 34}" y="${BY + 86}" width="26" height="14" rx="2" fill="#fff" stroke="${c}"/>` + txt(BX + 35 + i * 34, BY + 96, `M${i + 1}`, { size: 7, mono: true })).join('') +
    `<rect x="${BX + 150}" y="${BY + 30}" width="12" height="20" rx="2" fill="#fff" stroke="${c}"/>` + txt(BX + 156, BY + 26, 'I2C1', { size: 7, mono: true, fill: c }) +
    `<rect x="${BX + 150}" y="${BY + 58}" width="12" height="20" rx="2" fill="#fff" stroke="${c}"/>` + txt(BX + 156, BY + 86, 'I2C2', { size: 7, mono: true, fill: c }) +
    `<rect x="${BX + 2}" y="${BY + 36}" width="12" height="30" rx="2" fill="#fff" stroke="${c}"/>` + txt(BX + 8, BY + 54, 'USB', { size: 6, mono: true }),
  // mBot（mCore）：藍圓板、4 個 RJ25 埠、面板按鈕
  mbot: (c) => `<rect x="${BX + 18}" y="${BY + 8}" width="134" height="104" rx="30" fill="${c}" opacity="0.16" stroke="${c}" stroke-width="2"/>` +
    txt(BX + 85, BY + 60, 'mCore', { size: 12, weight: 700, fill: c }) + `<circle cx="${BX + 85}" cy="${BY + 34}" r="7" fill="#fff" stroke="${c}"/>` +
    [1, 2, 3, 4].map((i) => `<rect x="${BX + 24 + (i - 1) * 32}" y="${BY + 92}" width="24" height="16" rx="3" fill="#fff" stroke="${c}"/>` + txt(BX + 36 + (i - 1) * 32, BY + 104, String(i), { size: 9, mono: true, weight: 700 })).join('') +
    txt(BX + 85, BY + 84, 'RJ25 ports', { size: 8, fill: c, mono: true }),
  // SPIKE Prime hub：黃色長方體、5×5 LED、左右各 3 埠 A~F
  spike: (c) => `<rect x="${BX + 30}" y="${BY + 4}" width="110" height="112" rx="10" fill="${c}" opacity="0.9"/>` +
    `<rect x="${BX + 52}" y="${BY + 20}" width="66" height="66" rx="6" fill="#fff" opacity="0.9"/>` + ledGrid(BX + 60, BY + 28, c, 11, 7, '#1f2933') +
    `<circle cx="${BX + 85}" cy="${BY + 102}" r="7" fill="#fff" stroke="#1f2933" stroke-width="1.5"/>` +
    ['A', 'B', 'C'].map((p, i) => `<rect x="${BX + 22}" y="${BY + 16 + i * 32}" width="12" height="16" rx="2" fill="#fff" stroke="#1f2933"/>` + txt(BX + 28, BY + 28 + i * 32, p, { size: 9, weight: 700 })).join('') +
    ['D', 'E', 'F'].map((p, i) => `<rect x="${BX + 136}" y="${BY + 16 + i * 32}" width="12" height="16" rx="2" fill="#fff" stroke="#1f2933"/>` + txt(BX + 142, BY + 28 + i * 32, p, { size: 9, weight: 700 })).join(''),
  // EV3 brick：白磚、灰螢幕、按鍵、下方 1~4 埠
  ev3: (c) => `<rect x="${BX + 36}" y="${BY + 2}" width="98" height="116" rx="8" fill="#f1f3f5" stroke="#8a97a3" stroke-width="2"/>` +
    `<rect x="${BX + 48}" y="${BY + 10}" width="74" height="40" rx="3" fill="#9aa5ad"/>` + `<rect x="${BX + 54}" y="${BY + 16}" width="62" height="28" fill="#c7d0d6"/>` +
    `<rect x="${BX + 75}" y="${BY + 58}" width="20" height="20" rx="4" fill="#1f2933"/>` + [[-26, 0], [26, 0], [0, -14], [0, 14]].map(([dx, dy]) => `<rect x="${BX + 80 + dx}" y="${BY + 64 + dy}" width="10" height="8" rx="2" fill="#4b5a66"/>`).join('') +
    `<rect x="${BX + 40}" y="${BY + 96}" width="90" height="14" rx="2" fill="${c}" opacity="0.6"/>` +
    [1, 2, 3, 4].map((i) => `<rect x="${BX + 44 + (i - 1) * 22}" y="${BY + 100}" width="16" height="12" rx="2" fill="#fff" stroke="#1f2933"/>` + txt(BX + 52 + (i - 1) * 22, BY + 109, String(i), { size: 8, weight: 700, mono: true })).join(''),
};

// 產品方塊：黑板、青色邊、樂高孔排（右側）
function productBox(product, addr) {
  const x = W - 164, y = 56, w = 140, h = 88;
  return `<rect x="${x}" y="${y}" width="${w}" height="${h}" rx="12" fill="#0a111a" stroke="${C.brand}" stroke-width="1.5"/>` +
    [0, 1, 2, 3].map((i) => `<circle cx="${x + 24 + i * 30}" cy="${y + h - 9}" r="5" fill="none" stroke="#2b3a46" stroke-width="2"/>`).join('') +
    txt(x + w / 2, y + 26, 'MBC', { size: 16, weight: 700, fill: C.brand }) + txt(x + w / 2, y + 44, product, { size: product.length > 14 ? 9 : 11, fill: '#cfd8de' }) +
    (addr ? txt(x + w / 2, y + 60, addr, { size: 11, fill: '#cfd8de', mono: true }) : '');
}

export function render({ host, product = 'MBC sensor', addr = '' }) {
  const h = HOSTS[host];
  if (!h) return svg(txt(W / 2, H / 2, `wiring: ${host}`, { size: 18, fill: C.mute, mono: true }));
  let s = `<rect x="${BX}" y="${BY}" width="${BW}" height="${BH}" rx="14" fill="${h.color}" opacity="0.06"/>` + (SIL[host] ? SIL[host](h.color) : '') +
    txt(BX + BW / 2, 30, h.label, { size: 13, weight: 700 }) + productBox(product, addr);
  if (h.cable) {
    // 樂高線：接頭兩端方塊
    s += `<path d="M194 100 C 250 100, 260 100, ${W - 164} 100" stroke="${C.ink}" stroke-width="8" fill="none" stroke-linecap="round"/>` +
      `<rect x="188" y="92" width="14" height="16" rx="2" fill="#4b5a66"/><rect x="${W - 176}" y="92" width="14" height="16" rx="2" fill="#4b5a66"/>` +
      txt(W / 2 + 10, 86, h.cable, { size: 13, weight: 700 }) + txt(W / 2 + 10, 120, h.port, { size: 12, fill: C.mute, mono: true }) +
      txt(W / 2, H - 14, 'plug in like a LEGO sensor · no wiring', { size: 12, fill: C.mute });
    return svg(s);
  }
  const names = Object.keys(h.pins);
  names.forEach((n, i) => {
    const y = 62 + i * 26;
    s += `<line x1="194" y1="${y}" x2="${W - 164}" y2="${y}" stroke="${WIRE[n] || C.ink}" stroke-width="4" stroke-linecap="round"/>` +
      `<rect x="200" y="${y - 7}" width="${h.pins[n].length * 6.6 + 8}" height="14" rx="3" fill="#fff" opacity="0.9"/>` + txt(204, y + 4, h.pins[n], { size: 11, anchor: 'start', mono: true }) +
      txt(W - 158, y + 4, n, { size: 11, anchor: 'start', mono: true, weight: 700, fill: WIRE[n] || C.ink });
  });
  s += txt(W / 2, H - 14, h.uart ? 'UART 38400 8N1 via mBot adapter (RJ25)' : host === 'microbit' ? '3.3 V logic OK · power the module from 5 V · 100 kHz' : '5 V direct, no level shifter · 100 kHz', { size: 12, fill: C.mute });
  return svg(s);
}
