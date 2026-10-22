// 值示意圖 SVG 樣板（W1 補齊）：line-position、ruler、line-width、line-segments、bitmap、channels、
//   hue-ring、cam-frame、wro-spots、imu-axes、imu-faces、pad-buttons、pad-sticks、tof2-lr、distance-ruler、tof8-fan、flags
// 規則：固定 viewBox、有 width/height、不用外部字型、文字只用數字／符號／英文短標籤（中英共用）、無隨機
export const W = 480, H = 200;
export const C = { brand: '#0ABAB5', ink: '#1f2933', mute: '#8a97a3', line: '#10212b', on: '#0ABAB5', off: '#e3e8ee', warn: '#E8711E', bg: '#ffffff', spike: '#F2C200' };
export const FONT = 'Nunito, "Segoe UI", Arial, sans-serif';
export const MONO = '"JetBrains Mono", Consolas, monospace';

export const svg = (body, w = W, h = H) => `<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 ${w} ${h}" width="${w}" height="${h}" role="img" font-family='${FONT}'>\n<rect width="${w}" height="${h}" rx="12" fill="${C.bg}"/>\n${body}\n</svg>\n`;
const esc = (s) => String(s).replace(/&(?!#?\w+;)/g, '&amp;').replace(/</g, '&lt;');
export const txt = (x, y, s, o = {}) => `<text x="${x}" y="${y}" font-size="${o.size || 13}" fill="${o.fill || C.ink}" text-anchor="${o.anchor || 'middle'}"${o.mono ? ` font-family='${MONO}'` : ''}${o.weight ? ` font-weight="${o.weight}"` : ''}>${esc(s)}</text>`;
const r1 = (v) => Math.round(v * 10) / 10;
export const DEFS = `<defs><marker id="ar" markerWidth="8" markerHeight="8" refX="7" refY="4" orient="auto" markerUnits="userSpaceOnUse"><path d="M0 0 L8 4 L0 8 Z" fill="${C.ink}"/></marker><marker id="arw" markerWidth="8" markerHeight="8" refX="7" refY="4" orient="auto" markerUnits="userSpaceOnUse"><path d="M0 0 L8 4 L0 8 Z" fill="${C.warn}"/></marker><marker id="arb" markerWidth="8" markerHeight="8" refX="7" refY="4" orient="auto" markerUnits="userSpaceOnUse"><path d="M0 0 L8 4 L0 8 Z" fill="${C.brand}"/></marker></defs>`;
export const arrow = (x1, y1, x2, y2, o = {}) => `<line x1="${r1(x1)}" y1="${r1(y1)}" x2="${r1(x2)}" y2="${r1(y2)}" stroke="${o.color || C.ink}" stroke-width="${o.w || 2}" marker-end="url(#${o.marker || 'ar'})"${o.dash ? ` stroke-dasharray="${o.dash}"` : ''}/>`;
// 極座標（角度：0 = 右，逆時針為正，畫面 y 向下）
const polar = (cx, cy, r, deg) => [r1(cx + r * Math.cos(deg * Math.PI / 180)), r1(cy - r * Math.sin(deg * Math.PI / 180))];
const arc = (cx, cy, r, a0, a1, o = {}) => {
  const [x0, y0] = polar(cx, cy, r, a0), [x1, y1] = polar(cx, cy, r, a1);
  const large = Math.abs(a1 - a0) > 180 ? 1 : 0, sweep = a1 > a0 ? 0 : 1;
  return `<path d="M${x0} ${y0} A${r} ${r} 0 ${large} ${sweep} ${x1} ${y1}" fill="none" stroke="${o.color || C.warn}" stroke-width="${o.w || 2.5}"${o.marker ? ` marker-end="url(#${o.marker})"` : ''}/>`;
};
// 色相 0~1 → 固定十六進位色（不用 hsl() 以求各瀏覽器一致）
const hueHex = (h) => {
  const f6 = ((h % 1) + 1) % 1 * 6, i = Math.floor(f6), f = f6 - i, q = 1 - f;
  const c = [[1, f, 0], [q, 1, 0], [0, 1, f], [0, q, 1], [f, 0, 1], [1, 0, q]][i % 6];
  return '#' + c.map((v) => Math.round(v * 230 + 15).toString(16).padStart(2, '0')).join('');
};
const satRgb = (a) => `rgb(${Math.round(230 - 200 * a)},${Math.round(30 + 50 * (1 - a))},${Math.round(40 + 120 * (1 - a))})`;
const pm = (v) => (v > 0 ? '+' : '') + v;

// n 格感應器條（置中），回傳格子座標
function cells(n, y, on = [], opt = {}) {
  const gap = 4, cw = Math.min(40, (W - 80 - gap * (n - 1)) / n), ch = opt.h || 30;
  const x0 = (W - (cw * n + gap * (n - 1))) / 2;
  let s = '';
  for (let i = 0; i < n; i++) {
    const x = x0 + i * (cw + gap);
    const isOn = on.includes(i);
    s += `<rect x="${x}" y="${y}" width="${cw}" height="${ch}" rx="5" fill="${isOn ? C.on : C.off}" stroke="${C.mute}" stroke-width="1"/>`;
    s += txt(x + cw / 2, y + ch / 2 + 4, String(i + 1), { size: 12, fill: isOn ? '#fff' : C.ink, weight: 700 });
  }
  return { s, x0, cw, gap, ch };
}
const lr = (y) => txt(28, y, 'L', { size: 14, fill: C.mute, weight: 700 }) + txt(W - 28, y, 'R', { size: 14, fill: C.mute, weight: 700 });
const front = () => `<path d="M${W / 2 - 8} 26 L${W / 2} 10 L${W / 2 + 8} 26 Z" fill="${C.mute}"/>` + txt(W / 2, 40, 'FRONT', { size: 11, fill: C.mute });

// 水平尺：x0..x1 @ y，min..max，ticks 份，指標在 pos（0~1）
function hruler(x0, x1, y, min, max, ticks, pos, o = {}) {
  let s = `<line x1="${x0}" y1="${y}" x2="${x1}" y2="${y}" stroke="${C.ink}" stroke-width="3"/>`;
  for (let i = 0; i <= ticks; i++) {
    const x = r1(x0 + (x1 - x0) * i / ticks);
    const v = Math.round(min + (max - min) * i / ticks);
    s += `<line x1="${x}" y1="${y - 10}" x2="${x}" y2="${y + 10}" stroke="${C.ink}" stroke-width="2"/>` + txt(x, y + 28, o.signed ? pm(v) : String(v), { size: 12, mono: true });
  }
  if (pos != null) {
    const px = r1(x0 + (x1 - x0) * pos), pv = Math.round(min + (max - min) * pos);
    s += `<path d="M${px - 8} ${y - 32} L${px} ${y - 14} L${px + 8} ${y - 32} Z" fill="${C.warn}"/>` + txt(px, y - 40, `${o.signed ? pm(pv) : pv}${o.unit ? ' ' + o.unit : ''}`, { size: o.psize || 17, weight: 700, fill: C.warn, mono: true });
  }
  return s;
}

// PSX 16 鍵名稱（bit0 … bit15）
const PSX = ['SEL', 'L3', 'R3', 'START', 'UP', 'RIGHT', 'DOWN', 'LEFT', 'L2', 'R2', 'L1', 'R1', '▲', '●', '✕', '■'];
const GROUPS = { dpad: { bits: [4, 5, 6, 7], title: 'D-pad' }, shape: { bits: [12, 13, 14, 15], title: 'shape keys' }, shoulder: { bits: [8, 9, 10, 11], title: 'shoulder keys' }, func: { bits: [0, 1, 2, 3], title: 'function keys' } };

export const TEMPLATES = {
  // 線位置：n 格＋黑線＋ −n/2..+n/2 刻度
  'line-position': ({ n = 8 }) => {
    const y = 60;
    const onIdx = n === 8 ? [5] : [Math.floor(n * 0.7)];
    const { s, x0, cw, gap } = cells(n, y, onIdx);
    const span = cw * n + gap * (n - 1);
    const lx = x0 + onIdx[0] * (cw + gap) + cw / 2;
    const ticks = [-n, -n / 2, 0, n / 2, n];
    let scale = `<line x1="${x0}" y1="${y + 62}" x2="${x0 + span}" y2="${y + 62}" stroke="${C.ink}" stroke-width="2"/>`;
    for (const t of ticks) {
      const x = x0 + (t + n) / (2 * n) * span;
      scale += `<line x1="${x}" y1="${y + 56}" x2="${x}" y2="${y + 68}" stroke="${C.ink}" stroke-width="2"/>` + txt(x, y + 86, (t > 0 ? '+' : '') + t, { size: 13, mono: true });
    }
    const val = Math.round(((lx - x0) / span) * 2 * n - n);
    return svg(front() + lr(y + 20) + `<rect x="${lx - 7}" y="${y - 10}" width="14" height="50" fill="${C.line}" opacity="0.85"/>` + s + scale +
      `<path d="M${lx - 6} ${y + 50} L${lx} ${y + 58} L${lx + 6} ${y + 50} Z" fill="${C.warn}"/>` +
      txt(lx, y + 110, `= ${val > 0 ? '+' : ''}${val}`, { size: 18, weight: 700, fill: C.warn, mono: true }) +
      txt(W / 2, H - 10, `-${n} … 0 … +${n}`, { size: 12, fill: C.mute, mono: true }));
  },
  // 尺：min..max，指標在 +40%
  ruler: ({ min = -100, max = 100 }) => {
    const x0 = 50, x1 = W - 50, y = 100;
    const pos = 0.7;
    let s = `<line x1="${x0}" y1="${y}" x2="${x1}" y2="${y}" stroke="${C.ink}" stroke-width="3"/>`;
    for (let i = 0; i <= 8; i++) {
      const x = x0 + (x1 - x0) * i / 8;
      const v = Math.round(min + (max - min) * i / 8);
      s += `<line x1="${x}" y1="${y - (i % 2 ? 6 : 12)}" x2="${x}" y2="${y + (i % 2 ? 6 : 12)}" stroke="${C.ink}" stroke-width="2"/>`;
      if (i % 2 === 0) s += txt(x, y + 32, (v > 0 ? '+' : '') + v, { size: 13, mono: true });
    }
    const px = x0 + (x1 - x0) * pos, pv = Math.round(min + (max - min) * pos);
    s += `<path d="M${px - 8} ${y - 34} L${px} ${y - 16} L${px + 8} ${y - 34} Z" fill="${C.warn}"/>` + txt(px, y - 42, `${pv > 0 ? '+' : ''}${pv}`, { size: 18, weight: 700, fill: C.warn, mono: true });
    if (min < 0) s += txt(x0, y + 60, 'L', { size: 14, fill: C.mute, weight: 700 }) + txt(x1, y + 60, 'R', { size: 14, fill: C.mute, weight: 700 }) + txt(W / 2, y + 60, '0 = center', { size: 12, fill: C.mute });
    else s += txt(W / 2, y + 60, `${min} … ${max}`, { size: 12, fill: C.mute, mono: true });
    return svg((min < 0 ? front() : '') + s);
  },
  // 線寬：壓到線的格數
  'line-width': ({ n = 8 }) => {
    const y = 70;
    const on = [Math.floor(n / 2) - 1, Math.floor(n / 2), Math.floor(n / 2) + 1];
    const { s, x0, cw, gap } = cells(n, y, on);
    const a = x0 + on[0] * (cw + gap), b = x0 + on[on.length - 1] * (cw + gap) + cw;
    return svg(front() + lr(y + 20) + `<rect x="${a}" y="${y - 12}" width="${b - a}" height="54" fill="${C.line}" opacity="0.18"/>` + s +
      `<line x1="${a}" y1="${y + 56}" x2="${b}" y2="${y + 56}" stroke="${C.warn}" stroke-width="3"/>` +
      txt((a + b) / 2, y + 80, `width = ${on.length}`, { size: 18, weight: 700, fill: C.warn, mono: true }) +
      txt(W / 2, H - 12, `0 … ${n}   (0 = no line)`, { size: 12, fill: C.mute, mono: true }));
  },
  // 線段數：一條線 vs 分岔
  'line-segments': ({ n = 8 }) => {
    const a = cells(n, 40, [3, 4], { h: 26 });
    const b = cells(n, 120, [1, 2, 5, 6], { h: 26 });
    return svg(lr(58) + a.s + txt(W / 2, 90, 'segments = 1', { size: 15, weight: 700, fill: C.warn, mono: true }) +
      b.s + txt(W / 2, 170, 'segments = 2  (fork / cross)', { size: 15, weight: 700, fill: C.warn, mono: true }) +
      txt(W / 2, H - 8, '0 = lost', { size: 12, fill: C.mute, mono: true }));
  },
  // 位元圖：bit0 = 第 1 路
  bitmap: ({ n = 8 }) => {
    const y = 60;
    const on = n >= 4 ? [3, 4] : [0];
    const { s, x0, cw, gap } = cells(n, y, on);
    let bits = '';
    for (let i = 0; i < n; i++) bits += txt(x0 + i * (cw + gap) + cw / 2, y + 52, `bit${i}`, { size: 11, fill: C.mute, mono: true });
    const bin = Array.from({ length: n }, (_, i) => (on.includes(n - 1 - i) ? '1' : '0')).join('');
    const val = parseInt(bin, 2);
    return svg(front() + lr(y + 20) + s + bits +
      txt(W / 2, y + 90, `0b${bin} = ${val}`, { size: 18, weight: 700, fill: C.warn, mono: true }) +
      txt(W / 2, H - 10, n === 2 ? 'bit0 = L, bit1 = R' : `bit0 = ch1 (L) … bit${n - 1} = ch${n} (R)`, { size: 12, fill: C.mute, mono: true }));
  },
  // 各路光值長條圖
  channels: ({ n = 8 }) => {
    const vals = Array.from({ length: n }, (_, i) => [92, 88, 80, 12, 8, 75, 90, 95, 93, 85, 20, 15, 80, 90, 94, 96][i % 16]);
    const gap = 6, bw = Math.min(36, (W - 100 - gap * (n - 1)) / n), x0 = (W - (bw * n + gap * (n - 1))) / 2 + 10, base = 150, hmax = 100;
    let s = `<line x1="${x0 - 14}" y1="${base}" x2="${x0 + bw * n + gap * (n - 1) + 6}" y2="${base}" stroke="${C.ink}" stroke-width="2"/>`;
    s += `<line x1="${x0 - 14}" y1="${base}" x2="${x0 - 14}" y2="${base - hmax}" stroke="${C.ink}" stroke-width="2"/>` + txt(x0 - 24, base + 4, '0', { size: 11, mono: true, anchor: 'end' }) + txt(x0 - 24, base - hmax + 4, '100', { size: 11, mono: true, anchor: 'end' });
    for (let i = 0; i < n; i++) {
      const x = x0 + i * (bw + gap), h = vals[i];
      s += `<rect x="${x}" y="${base - h}" width="${bw}" height="${h}" rx="4" fill="${h < 50 ? C.line : C.on}"/>`;
      s += txt(x + bw / 2, base - h - 5, String(h), { size: 10, mono: true }) + txt(x + bw / 2, base + 16, String(i + 1), { size: 11, fill: C.mute, weight: 700 });
    }
    return svg(front() + s + txt(W / 2, H - 8, 'reflect 0 (black) … 100 (white), ch1 = L', { size: 12, fill: C.mute, mono: true }));
  },

  // ---------------------------------------------------------------- 神攝手
  // 色相環：0~max 繞一圈（0 = 紅、1/3 = 綠、2/3 = 藍），中央灰點 = −1
  'hue-ring': ({ max = 100 }) => {
    const cx = 130, cy = 100, R = 72, r = 46, N = 36;
    let s = '';
    for (let i = 0; i < N; i++) {
      const a0 = 90 - i * 360 / N, a1 = 90 - (i + 1) * 360 / N - 0.6;
      const [ax, ay] = polar(cx, cy, R, a0), [bx, by] = polar(cx, cy, R, a1), [cx1, cy1] = polar(cx, cy, r, a1), [dx, dy] = polar(cx, cy, r, a0);
      s += `<path d="M${ax} ${ay} A${R} ${R} 0 0 1 ${bx} ${by} L${cx1} ${cy1} A${r} ${r} 0 0 0 ${dx} ${dy} Z" fill="${hueHex(i / N)}"/>`;
    }
    for (const f of [0, 1 / 6, 1 / 3, 1 / 2, 2 / 3, 5 / 6]) {
      const [tx, ty] = polar(cx, cy, R + 14, 90 - f * 360);
      s += txt(tx, ty + 4, String(Math.round(max * f)), { size: 12, mono: true, weight: 700 });
    }
    s += `<circle cx="${cx}" cy="${cy}" r="22" fill="#9aa5ad" stroke="#fff" stroke-width="3"/>` + txt(cx, cy + 4, '-1', { size: 14, weight: 700, fill: '#fff', mono: true });
    const lx = 262;
    const row = (y, color, label, note) => `<rect x="${lx}" y="${y - 12}" width="22" height="16" rx="3" fill="${color}" stroke="${C.mute}"/>` + txt(lx + 32, y, label, { size: 13, mono: true, weight: 700, anchor: 'start' }) + txt(lx + 32, y + 15, note, { size: 11, fill: C.mute, anchor: 'start' });
    s += row(42, hueHex(0), `0 / ${max}`, 'red') + row(80, hueHex(1 / 6), String(Math.round(max / 6)), 'yellow') + row(118, hueHex(1 / 3), String(Math.round(max / 3)), 'green') + row(156, hueHex(2 / 3), String(Math.round(max * 2 / 3)), 'blue');
    s += txt(lx + 100, 190, '-1 = gray / white / black (S < 10)', { size: 11, fill: C.mute, mono: true });
    return svg(s);
  },
  // 畫面框：X 0~w（左 0）、Y 0~h（下 0），色塊框，show = x | y | w | h | area
  'cam-frame': ({ w = 100, h = 75, show = 'x' }) => {
    const fx = 70, fy = 20, fw = 300, fh = 140;
    const bx = 60, by = 45, bw = 26, bh = 20; // 色塊中心與大小（畫面座標）
    const X = (v) => r1(fx + v / w * fw), Y = (v) => r1(fy + fh - v / h * fh);
    const l = X(bx - bw / 2), t = Y(by + bh / 2), rgt = X(bx + bw / 2), btm = Y(by - bh / 2);
    let s = DEFS + `<rect x="${fx}" y="${fy}" width="${fw}" height="${fh}" rx="6" fill="#f3f6f8" stroke="${C.ink}" stroke-width="2"/>`;
    s += `<rect x="${l}" y="${t}" width="${r1(rgt - l)}" height="${r1(btm - t)}" fill="${show === 'area' ? C.warn : '#D62828'}" opacity="${show === 'area' ? 0.6 : 0.55}" stroke="#D62828" stroke-width="2"/>`;
    s += `<circle cx="${X(bx)}" cy="${Y(by)}" r="3" fill="#fff"/>`;
    s += txt(fx, fy + fh + 15, '0', { size: 11, mono: true }) + txt(fx + fw, fy + fh + 15, String(w), { size: 11, mono: true }) + txt(fx + fw / 2, fy + fh + 15, 'X', { size: 11, mono: true, fill: C.mute });
    s += txt(fx - 14, fy + fh + 4, '0', { size: 11, mono: true, anchor: 'end' }) + txt(fx - 14, fy + 8, String(h), { size: 11, mono: true, anchor: 'end' }) + txt(fx - 14, fy + fh / 2 + 4, 'Y', { size: 11, mono: true, fill: C.mute, anchor: 'end' });
    const lab = (x, y, t2, o = {}) => txt(r1(x), r1(y), t2, { size: 16, weight: 700, fill: C.warn, mono: true, ...o });
    if (show === 'x') {
      s += `<line x1="${X(bx)}" y1="${Y(by)}" x2="${X(bx)}" y2="${fy + fh}" stroke="${C.warn}" stroke-width="2" stroke-dasharray="5 4"/>` + arrow(fx, fy + fh - 8, X(bx), fy + fh - 8, { color: C.warn, marker: 'arw' }) + lab((X(bx) + fx) / 2, fy + fh - 16, `X = ${bx}`);
    } else if (show === 'y') {
      s += `<line x1="${fx}" y1="${Y(by)}" x2="${X(bx)}" y2="${Y(by)}" stroke="${C.warn}" stroke-width="2" stroke-dasharray="5 4"/>` + arrow(fx + 10, fy + fh, fx + 10, Y(by), { color: C.warn, marker: 'arw' }) + lab(fx + 26, (fy + fh + Y(by)) / 2 + 5, `Y = ${by}`, { anchor: 'start' });
    } else if (show === 'w') {
      s += `<line x1="${l}" y1="${t - 10}" x2="${rgt}" y2="${t - 10}" stroke="${C.warn}" stroke-width="3"/>` + lab((l + rgt) / 2, t - 16, `W = ${bw}`);
    } else if (show === 'h') {
      s += `<line x1="${rgt + 10}" y1="${t}" x2="${rgt + 10}" y2="${btm}" stroke="${C.warn}" stroke-width="3"/>` + lab(rgt + 18, (t + btm) / 2 + 5, `H = ${bh}`, { anchor: 'start' });
    } else {
      s += lab((l + rgt) / 2, t - 8, `area = W × H = ${bw * bh}`);
    }
    s += txt(W - 50, fy + fh - 4, show === 'area' ? `0 … ${w * h}` : show === 'y' || show === 'h' ? `0 … ${h}` : `0 … ${w}`, { size: 11, fill: C.mute, mono: true, anchor: 'end' });
    s += txt(W / 2, H - 5, 'blob = largest patch of the target colour · 0 = not found', { size: 11, fill: C.mute });
    return svg(s);
  },
  // WRO 12 色點：S1~S12（位置在相機選單設定），show = color | vivid
  'wro-spots': ({ n = 12, show = 'color' }) => {
    const fx = 30, fy = 22, fw = 240, fh = 156;
    let s = `<rect x="${fx}" y="${fy}" width="${fw}" height="${fh}" rx="6" fill="#f3f6f8" stroke="${C.ink}" stroke-width="2"/>`;
    const cols = 4, rows = Math.ceil(n / cols);
    const hues = [0, 0.08, 0.17, 0.33, 0.5, 0.62, 0.75, 0.9, 0.25, 0.42, 0.58, 0.83];
    const sats = [250, 200, 160, 120, 90, 60, 230, 180, 140, 100, 1, 0];
    for (let i = 0; i < n; i++) {
      const cx = r1(fx + fw * (i % cols + 0.5) / cols), cy = r1(fy + fh * (Math.floor(i / cols) + 0.5) / rows);
      let fill, dark;
      if (show === 'color') { fill = i === 10 ? '#f4f4f4' : i === 11 ? '#111' : hueHex(hues[i]); dark = i === 10; }
      else { fill = i === 10 ? '#111' : i === 11 ? '#f4f4f4' : satRgb(sats[i] / 255); dark = i === 11; }
      const off = i === 11 && show === 'color';
      s += off ? `<circle cx="${cx}" cy="${cy}" r="15" fill="none" stroke="${C.mute}" stroke-width="2" stroke-dasharray="4 3"/>` : `<circle cx="${cx}" cy="${cy}" r="15" fill="${fill}" stroke="${C.ink}" stroke-width="1.5"/>`;
      s += txt(cx, cy + 4, `S${i + 1}`, { size: 10, weight: 700, fill: off || dark ? C.ink : '#fff', mono: true });
    }
    const lx = 290;
    let g = '';
    if (show === 'color') {
      for (let i = 0; i < 24; i++) g += `<rect x="${lx + i * 6}" y="36" width="6.5" height="16" fill="${hueHex(i / 24)}"/>`;
      s += g + txt(lx, 30, '0', { size: 10, mono: true, anchor: 'start' }) + txt(lx + 144, 30, '239', { size: 10, mono: true, anchor: 'end' }) + txt(lx + 72, 66, 'hue (colour)', { size: 11, fill: C.mute });
      s += `<rect x="${lx}" y="82" width="22" height="16" rx="3" fill="#f4f4f4" stroke="${C.mute}"/>` + txt(lx + 30, 94, '240~245 = white', { size: 11, anchor: 'start', mono: true });
      s += `<rect x="${lx}" y="108" width="22" height="16" rx="3" fill="#111"/>` + txt(lx + 30, 120, '246~250 = black', { size: 11, anchor: 'start', mono: true });
      s += `<circle cx="${lx + 11}" cy="142" r="8" fill="none" stroke="${C.mute}" stroke-width="2" stroke-dasharray="4 3"/>` + txt(lx + 30, 146, '255 = spot off', { size: 11, anchor: 'start', mono: true });
      s += txt(lx + 72, 178, 'ids 10~1B = S1~S12', { size: 11, fill: C.mute, mono: true });
    } else {
      for (let i = 0; i < 24; i++) g += `<rect x="${lx + i * 6}" y="36" width="6.5" height="16" fill="${satRgb(i / 23)}"/>`;
      s += g + txt(lx, 30, '0', { size: 10, mono: true, anchor: 'start' }) + txt(lx + 144, 30, '255', { size: 10, mono: true, anchor: 'end' }) + txt(lx + 72, 66, 'saturation (vividness)', { size: 11, fill: C.mute });
      s += `<rect x="${lx}" y="82" width="22" height="16" rx="3" fill="#111"/>` + txt(lx + 30, 94, 'black = 1', { size: 11, anchor: 'start', mono: true });
      s += `<rect x="${lx}" y="108" width="22" height="16" rx="3" fill="#f4f4f4" stroke="${C.mute}"/>` + txt(lx + 30, 120, 'white = real S', { size: 11, anchor: 'start', mono: true });
      s += txt(lx + 30, 146, 'spot off = 0', { size: 11, anchor: 'start', mono: true });
      s += txt(lx + 72, 178, 'ids 20~2B = S1~S12', { size: 11, fill: C.mute, mono: true });
    }
    return svg(s);
  },

  // ---------------------------------------------------------------- IMU
  // 板子軸向：axis = yaw | pitch；kind = gyro | accel | mag
  'imu-axes': ({ axis, kind }) => {
    let s = DEFS;
    // 左：板子俯視圖（X 朝前＝上、Y 朝左、Z 朝上＝出紙面）
    const bx = 60, by = 50, bw = 120, bh = 100, cx = bx + bw / 2, cy = by + bh / 2;
    s += `<rect x="${bx}" y="${by}" width="${bw}" height="${bh}" rx="10" fill="#0a111a"/>` +
      [0, 1, 2].map((i) => `<circle cx="${bx + 20 + i * 40}" cy="${by + bh - 14}" r="5" fill="none" stroke="#2b3a46" stroke-width="2"/>`).join('') +
      `<rect x="${cx - 12}" y="${cy - 12}" width="24" height="24" rx="3" fill="#2b3a46"/>` + `<circle cx="${cx - 8}" cy="${cy - 8}" r="2" fill="${C.brand}"/>`;
    s += arrow(cx, cy, cx, by - 18, { color: C.brand, marker: 'arb', w: 3 }) + txt(cx + 12, by - 10, 'X', { size: 14, weight: 700, fill: C.brand, mono: true }) +
      arrow(cx, cy, bx - 22, cy, { color: C.brand, marker: 'arb', w: 3 }) + txt(bx - 22, cy - 10, 'Y', { size: 14, weight: 700, fill: C.brand, mono: true }) +
      `<circle cx="${cx}" cy="${cy}" r="6" fill="#fff" stroke="${C.brand}" stroke-width="2"/><circle cx="${cx}" cy="${cy}" r="2" fill="${C.brand}"/>` + txt(cx + 16, cy + 16, 'Z', { size: 14, weight: 700, fill: C.brand, mono: true });
    s += txt(cx, by + bh + 30, 'top view · X = front · Z = up', { size: 11, fill: C.mute });
    const rx = 330; // 右半說明區中心
    if (axis === 'yaw') {
      s += arc(rx, 100, 48, 200, -140, { marker: 'arw', w: 3 }) + `<circle cx="${rx}" cy="100" r="7" fill="#fff" stroke="${C.brand}" stroke-width="2"/><circle cx="${rx}" cy="100" r="2.5" fill="${C.brand}"/>`;
      s += txt(rx, 40, 'yaw axis = the axis pointing up', { size: 12, weight: 700 }) + txt(rx, 170, '+ = same sign as heading', { size: 12, fill: C.warn, mono: true }) + txt(rx, 188, 'unit 0.1 °/s', { size: 11, fill: C.mute, mono: true });
    } else if (axis === 'pitch') {
      const ox = rx - 60, oy = 120, len = 110, th = 22;
      const [ex, ey] = polar(ox, oy, len, th);
      s += `<line x1="${ox - 20}" y1="${oy}" x2="${ox + len + 10}" y2="${oy}" stroke="${C.mute}" stroke-width="2" stroke-dasharray="6 4"/>` + txt(ox + len + 10, oy + 16, 'level', { size: 11, fill: C.mute, anchor: 'end' });
      s += `<g transform="rotate(${-th} ${ox} ${oy})"><rect x="${ox}" y="${oy - 6}" width="${len - 8}" height="12" rx="4" fill="#0a111a"/></g>`;
      s += arrow(ox, oy, ex, ey, { color: C.brand, marker: 'arb', w: 3 }) + txt(ex + 10, ey, 'X', { size: 14, weight: 700, fill: C.brand, mono: true });
      s += arc(ox, oy, 60, 0, th, { w: 2.5 }) + txt(ox + 76, oy - 10, `pitch = +${th}°`, { size: 14, weight: 700, fill: C.warn, mono: true, anchor: 'start' });
      s += txt(rx, 40, 'side view · X axis vs level', { size: 12, weight: 700 }) + txt(rx, 172, 'nose up = +   ·   pitch rate 0.1 °/s', { size: 11, fill: C.mute, mono: true });
    } else {
      const K = { gyro: { title: 'gyro X / Y / Z', unit: '0.1 °/s', ids: '10 · 11 · 12', curved: true }, accel: { title: 'accel X / Y / Z', unit: 'mg  (1000 = 1 g)', ids: '13 · 14 · 15' }, mag: { title: 'mag X / Y / Z', unit: 'mG', ids: '16 · 17 · 18' } }[kind] || { title: String(kind), unit: '', ids: '' };
      const ox = rx - 10, oy = 134;
      s += arrow(ox, oy, ox + 70, oy - 36, { color: C.brand, marker: 'arb', w: 3 }) + txt(ox + 84, oy - 38, 'X', { size: 14, weight: 700, fill: C.brand, mono: true });
      s += arrow(ox, oy, ox - 70, oy - 18, { color: C.brand, marker: 'arb', w: 3 }) + txt(ox - 84, oy - 18, 'Y', { size: 14, weight: 700, fill: C.brand, mono: true });
      s += arrow(ox, oy, ox, oy - 72, { color: C.brand, marker: 'arb', w: 3 }) + txt(ox + 12, oy - 70, 'Z', { size: 14, weight: 700, fill: C.brand, mono: true });
      if (K.curved) s += arc(ox + 35, oy - 18, 14, 250, -60, { marker: 'arw', w: 2 }) + arc(ox - 35, oy - 9, 14, 250, -60, { marker: 'arw', w: 2 }) + arc(ox, oy - 52, 14, 250, -60, { marker: 'arw', w: 2 });
      s += `<circle cx="${ox}" cy="${oy}" r="5" fill="${C.ink}"/>`;
      s += txt(rx, 28, K.title, { size: 13, weight: 700 }) + txt(rx, 164, `ids ${K.ids}`, { size: 12, mono: true, fill: C.warn, weight: 700 }) + txt(rx, 184, `unit ${K.unit} · int16`, { size: 11, fill: C.mute, mono: true });
    }
    return svg(s);
  },
  // 朝上的面：0 +X、1 +Y、2 +Z、3 −X、4 −Y、5 −Z
  'imu-faces': () => {
    const faces = ['+X', '+Y', '+Z', '-X', '-Y', '-Z'];
    let s = DEFS + arrow(W / 2 - 110, 30, W / 2 - 110, 12, { w: 2 }) + txt(W / 2 - 96, 24, 'UP', { size: 11, fill: C.mute, anchor: 'start' });
    const cw = 68, gap = 8, x0 = (W - (cw * 6 + gap * 5)) / 2, y = 44, ch = 110;
    faces.forEach((f, i) => {
      const x = x0 + i * (cw + gap), cx = x + cw / 2;
      s += `<rect x="${x}" y="${y}" width="${cw}" height="${ch}" rx="8" fill="${C.off}"/>`;
      s += txt(cx, y + 22, String(i), { size: 18, weight: 700, fill: C.warn, mono: true });
      const k = i % 3, neg = i >= 3;
      if (k === 0) s += `<rect x="${cx - 8}" y="${y + 38}" width="16" height="48" rx="3" fill="#0a111a"/>` + `<rect x="${cx - 8}" y="${neg ? y + 80 : y + 38}" width="16" height="6" fill="${C.brand}"/>`;
      else if (k === 1) s += `<rect x="${cx - 22}" y="${y + 50}" width="44" height="24" rx="3" fill="#0a111a"/>` + `<rect x="${neg ? cx + 16 : cx - 22}" y="${y + 50}" width="6" height="24" fill="${C.brand}"/>`;
      else s += `<g transform="skewX(-20) translate(${r1((y + 64) * Math.tan(20 * Math.PI / 180))} 0)"><rect x="${cx - 20}" y="${y + 50}" width="40" height="28" rx="4" fill="#0a111a"/><rect x="${cx - 20}" y="${neg ? y + 50 : y + 73}" width="40" height="5" fill="${C.brand}"/></g>`;
      s += txt(cx, y + 102, f, { size: 15, weight: 700, mono: true });
    });
    s += txt(W / 2, H - 14, 'value = which board axis points up (teal edge = + end of that axis)', { size: 11, fill: C.mute });
    return svg(s);
  },

  // ---------------------------------------------------------------- 掌控者
  // 16 鍵 PSX bit 圖；group = dpad | shape | shoulder | func | sys
  'pad-buttons': ({ layout, group }) => {
    let s = '';
    if (group === 'sys') {
      const cw = 110, x0 = (W - cw * 2 - 16) / 2;
      ['PS', 'CAPTURE'].forEach((n, i) => {
        const x = x0 + i * (cw + 16);
        s += `<rect x="${x}" y="60" width="${cw}" height="46" rx="8" fill="${i === 0 ? C.on : C.off}" stroke="${C.mute}"/>` + txt(x + cw / 2, 89, n, { size: 15, weight: 700, fill: i === 0 ? '#fff' : C.ink });
        s += txt(x + cw / 2, 128, `bit${i} = ${1 << i}`, { size: 13, mono: true, fill: C.mute });
      });
      return svg(s + txt(W / 2, 30, 'system keys', { size: 13, weight: 700 }) + txt(W / 2, 166, 'PS pressed → 1', { size: 16, weight: 700, fill: C.warn, mono: true }) + txt(W / 2, H - 10, 'bit1 = capture (PS5: mute) · PS2 always 0', { size: 11, fill: C.mute }));
    }
    const g = GROUPS[group];
    const cw = 50, gap = 5, x0 = (W - (cw * 8 + gap * 7)) / 2;
    const pressed = g ? [g.bits[0], g.bits[1]] : [4, 14];
    for (let b = 0; b < 16; b++) {
      const row = b < 8 ? 0 : 1, col = b % 8;
      const x = x0 + col * (cw + gap), y = 34 + row * 62;
      const inG = !g || g.bits.includes(b);
      const on = pressed.includes(b);
      s += `<rect x="${x}" y="${y}" width="${cw}" height="30" rx="6" fill="${on ? C.on : inG ? '#fff' : C.off}" stroke="${inG ? C.ink : C.mute}" stroke-width="${inG && g ? 2 : 1}" opacity="${inG ? 1 : 0.5}"/>`;
      s += txt(x + cw / 2, y + 20, PSX[b], { size: PSX[b].length > 3 ? 10 : 13, weight: 700, fill: on ? '#fff' : inG ? C.ink : C.mute });
      s += txt(x + cw / 2, y + 44, g ? (inG ? `= ${1 << (b - g.bits[0])}` : `bit${b}`) : `bit${b}`, { size: 10, mono: true, fill: inG && g ? C.warn : C.mute, weight: g && inG ? 700 : 400 });
    }
    if (g) {
      const w0 = 1 << (pressed[0] - g.bits[0]), w1 = 1 << (pressed[1] - g.bits[0]);
      s += txt(W / 2, 170, `${PSX[pressed[0]]} + ${PSX[pressed[1]]} = ${w0} + ${w1} = ${w0 + w1}`, { size: 15, weight: 700, fill: C.warn, mono: true });
      s += txt(W / 2, 20, `${g.title}: bit${g.bits[0]}~bit${g.bits[3]} of the 16-key value → 0~15`, { size: 12, weight: 700 });
    } else {
      s += txt(W / 2, 170, `UP + ✕ = 2^4 + 2^14 = ${(1 << 4) + (1 << 14)}`, { size: 15, weight: 700, fill: C.warn, mono: true });
      s += txt(W / 2, 20, '16 keys, PSX order · 1 = pressed · 0 when not connected', { size: 12, weight: 700 });
    }
    s += txt(W / 2, H - 8, 'SEL = SELECT / SHARE · START = OPTIONS', { size: 10, fill: C.mute });
    return svg(s);
  },
  // 搖桿：三把尺 ±100（I2C/mBlock）、0~99（EV3/SPIKE/MATRIX）、1~9（SPIKE app/Pybricks）
  'pad-sticks': () => {
    let s = DEFS;
    const cx = 80, cy = 100, R = 48;
    s += `<circle cx="${cx}" cy="${cy}" r="${R}" fill="${C.off}" stroke="${C.mute}"/>` + `<circle cx="${cx + 18}" cy="${cy - 14}" r="16" fill="#0a111a"/>`;
    s += arrow(cx, cy, cx + R + 18, cy, { color: C.brand, marker: 'arb', w: 2.5 }) + txt(cx + R + 20, cy - 8, '+X', { size: 12, weight: 700, fill: C.brand, mono: true, anchor: 'start' });
    s += arrow(cx, cy, cx, cy - R - 18, { color: C.brand, marker: 'arb', w: 2.5 }) + txt(cx + 8, cy - R - 20, '+Y', { size: 12, weight: 700, fill: C.brand, mono: true, anchor: 'start' });
    s += txt(cx, cy + R + 28, 'right / up = +', { size: 11, fill: C.mute });
    const x0 = 190, x1 = W - 30;
    const bar = (y, min, max, mid, label, signed) => {
      let t = `<line x1="${x0}" y1="${y}" x2="${x1}" y2="${y}" stroke="${C.ink}" stroke-width="3"/>`;
      [[0, min], [0.5, mid], [1, max]].forEach(([f, v]) => { const x = r1(x0 + (x1 - x0) * f); t += `<line x1="${x}" y1="${y - 8}" x2="${x}" y2="${y + 8}" stroke="${C.ink}" stroke-width="2"/>` + txt(x, y + 22, signed ? pm(v) : String(v), { size: 12, mono: true, weight: f === 0.5 ? 700 : 400, fill: f === 0.5 ? C.warn : C.ink }); });
      const px = r1(x0 + (x1 - x0) * 0.75);
      t += `<circle cx="${px}" cy="${y}" r="5" fill="${C.warn}"/>`;
      t += txt(x0, y - 12, label, { size: 11, fill: C.mute, anchor: 'start' });
      return t;
    };
    s += bar(42, -100, 100, 0, 'I2C · mBlock · micro:bit', true) + bar(104, 0, 99, 50, 'EV3 · SPIKE mode 4 · MATRIX', false) + bar(166, 1, 9, 5, 'SPIKE app · Pybricks', false);
    s += txt(cx, cy + R + 44, 'same position (75 %)', { size: 10, fill: C.mute }) + txt(cx, cy + R + 56, 'on all three scales', { size: 10, fill: C.mute });
    return svg(s);
  },

  // ---------------------------------------------------------------- 測距者
  // 兩顆：左右各一束
  'tof2-lr': () => {
    let s = DEFS + front();
    const mx = W / 2 - 70, my = 150, mw = 140, mh = 34;
    s += `<rect x="${mx}" y="${my}" width="${mw}" height="${mh}" rx="8" fill="#0a111a"/>` + txt(W / 2, my + 22, 'MBC', { size: 12, weight: 700, fill: C.brand });
    const lx = mx + 30, rx = mx + mw - 30;
    s += `<rect x="${lx - 10}" y="${my - 6}" width="20" height="12" rx="2" fill="${C.brand}"/><rect x="${rx - 10}" y="${my - 6}" width="20" height="12" rx="2" fill="${C.brand}"/>`;
    s += `<rect x="${lx - 60}" y="70" width="110" height="10" rx="3" fill="${C.line}"/>` + `<rect x="${rx - 50}" y="46" width="110" height="10" rx="3" fill="${C.line}"/>`;
    s += `<path d="M${lx - 10} ${my - 6} L${lx - 22} 80 L${lx + 22} 80 L${lx + 10} ${my - 6} Z" fill="${C.brand}" opacity="0.18"/>` + `<path d="M${rx - 10} ${my - 6} L${rx - 24} 56 L${rx + 24} 56 L${rx + 10} ${my - 6} Z" fill="${C.brand}" opacity="0.18"/>`;
    s += arrow(lx, my - 8, lx, 82, { color: C.warn, marker: 'arw', w: 2.5 }) + arrow(rx, my - 8, rx, 58, { color: C.warn, marker: 'arw', w: 2.5 });
    s += txt(lx - 30, 118, 'L', { size: 16, weight: 700, fill: C.warn, mono: true }) + txt(lx - 30, 134, '25 cm', { size: 12, mono: true }) + txt(lx - 30, 148, '250 mm', { size: 11, mono: true, fill: C.mute });
    s += txt(rx + 32, 100, 'R', { size: 16, weight: 700, fill: C.warn, mono: true }) + txt(rx + 32, 116, '40 cm', { size: 12, mono: true }) + txt(rx + 32, 130, '400 mm', { size: 11, mono: true, fill: C.mute });
    s += txt(60, 30, 'nearer = 25', { size: 12, mono: true, anchor: 'start' }) + txt(60, 46, 'avg mm = 325', { size: 12, mono: true, anchor: 'start' });
    s += txt(W - 20, 30, '120 cm / 1200 mm', { size: 11, mono: true, anchor: 'end', fill: C.mute }) + txt(W - 20, 46, '= nothing / sensor failed', { size: 11, anchor: 'end', fill: C.mute });
    s += txt(W / 2, H - 6, 'seen from behind the robot: L = sensor 0, R = sensor 1', { size: 11, fill: C.mute });
    return svg(s);
  },
  // 距離尺：0~max（cm 或 mm）
  'distance-ruler': ({ max = 120 }) => {
    const unit = max > 300 ? 'mm' : 'cm';
    let s = DEFS;
    s += `<rect x="30" y="40" width="46" height="30" rx="6" fill="#0a111a"/>` + `<rect x="76" y="48" width="8" height="14" rx="2" fill="${C.brand}"/>`;
    s += `<rect x="${W - 60}" y="30" width="12" height="50" rx="3" fill="${C.line}"/>`;
    s += arrow(84, 55, W - 62, 55, { color: C.warn, marker: 'arw', w: 2.5 }) + txt(W / 2 + 10, 48, `distance (${unit})`, { size: 12, fill: C.warn, weight: 700 });
    s += hruler(60, W - 40, 128, 0, max, 6, 0.35, { unit, psize: 16 });
    s += txt(W / 2, H - 8, `${max} ${unit} = nothing in range / sensor failed`, { size: 11, fill: C.mute, mono: true });
    return svg(s);
  },
  // 八顆扇形：1 正左 … 4/5 正前 … 8 正右；方向 −7~+7
  'tof8-fan': ({ n = 8 }) => {
    let s = DEFS;
    const cx = W / 2, cy = 168, L = 100;
    const angles = [180, 150, 120, 94, 86, 60, 30, 0]; // 每顆方向（度，0 = 右、90 = 前）
    const dirs = [-7, -5, -3, -1, 1, 3, 5, 7];
    const dist = [60, 75, 92, 100, 100, 85, 42, 90]; // 示意距離（畫面長度）
    s += `<path d="M${cx - L - 8} ${cy} A${L + 8} ${L + 8} 0 0 1 ${cx + L + 8} ${cy} Z" fill="${C.brand}" opacity="0.07"/>`;
    for (let i = 0; i < n; i++) {
      const a = angles[i], d = dist[i];
      const [ex, ey] = polar(cx, cy, d, a);
      const near = i === 6;
      s += `<line x1="${cx}" y1="${cy}" x2="${ex}" y2="${ey}" stroke="${near ? C.warn : C.brand}" stroke-width="${near ? 4 : 2.5}" stroke-linecap="round" opacity="${near ? 1 : 0.8}"/>`;
      const [lx, ly] = polar(cx, cy, L + 18, a === 94 ? 97 : a === 86 ? 83 : a);
      s += txt(lx, ly + 4, String(i + 1), { size: 12, weight: 700, fill: near ? C.warn : C.ink });
      const [dx, dy] = polar(cx, cy, L + 34, a === 94 ? 99 : a === 86 ? 81 : a);
      s += txt(dx, dy + 4, pm(dirs[i]), { size: 10, mono: true, fill: C.mute });
    }
    const [ox, oy] = polar(cx, cy, 50, 30);
    s += `<rect x="${ox - 6}" y="${oy - 14}" width="12" height="28" rx="3" fill="${C.line}" transform="rotate(-60 ${ox} ${oy})"/>`;
    s += `<rect x="${cx - 36}" y="${cy - 9}" width="72" height="20" rx="6" fill="#0a111a"/>` + txt(cx, cy + 5, 'MBC', { size: 11, weight: 700, fill: C.brand });
    s += txt(22, 20, 'L', { size: 14, weight: 700, fill: C.mute }) + txt(W - 22, 20, 'R', { size: 14, weight: 700, fill: C.mute }) + txt(cx, 14, 'FRONT = 4 + 5', { size: 11, fill: C.mute });
    s += txt(W - 16, 44, 'nearest = 7', { size: 13, weight: 700, fill: C.warn, mono: true, anchor: 'end' }) + txt(W - 16, 60, 'dir = +5', { size: 13, weight: 700, fill: C.warn, mono: true, anchor: 'end' });
    s += txt(cx, H - 6, 'dir -7 … 0 … +7 (- = left) · ±8 = nothing around', { size: 11, mono: true, fill: C.mute });
    return svg(s);
  },

  // ---------------------------------------------------------------- 共用
  // F3 旗標條：bits = [{bit, key}]，bit0 在左
  flags: ({ bits = [] } = {}) => {
    const cw = 50, gap = 5, x0 = (W - (cw * 8 + gap * 7)) / 2, y = 50;
    const byBit = Object.fromEntries(bits.map((b) => [b.bit, b]));
    let s = '';
    for (let b = 0; b < 8; b++) {
      const x = x0 + b * (cw + gap), def = byBit[b], on = b === 0;
      s += `<rect x="${x}" y="${y}" width="${cw}" height="34" rx="6" fill="${on ? C.on : def ? '#fff' : C.off}" stroke="${def ? C.ink : C.mute}"/>`;
      s += txt(x + cw / 2, y + 22, on ? '1' : '0', { size: 16, weight: 700, mono: true, fill: on ? '#fff' : def ? C.ink : C.mute });
      s += txt(x + cw / 2, y - 8, `bit${b}`, { size: 10, mono: true, fill: C.mute });
      if (def) {
        const parts = String(def.key).replace(/([a-z])([A-Z])/g, '$1 $2').split(' ');
        parts.slice(0, 2).forEach((p, i) => { s += txt(x + cw / 2, y + 52 + i * 13, p.length > 8 ? p.slice(0, 7) + '.' : p, { size: 10, fill: C.ink }); });
      } else s += txt(x + cw / 2, y + 52, '-', { size: 10, fill: C.mute });
    }
    s += txt(W / 2, 24, 'status F3 (bit field)', { size: 13, weight: 700 });
    s += txt(W / 2, 150, 'ready = bit0 = 1', { size: 15, weight: 700, fill: C.warn, mono: true });
    s += txt(W / 2, H - 10, 'value = Σ 2^bit · read id F3', { size: 11, fill: C.mute, mono: true });
    return svg(s);
  },
};

// 還沒畫的樣板：帶 tpl 名的簡單框
export function generic(tpl, params = {}) {
  const p = Object.entries(params).map(([k, v]) => `${k}=${JSON.stringify(v)}`).join('  ');
  return svg(`<rect x="20" y="20" width="${W - 40}" height="${H - 40}" rx="10" fill="none" stroke="${C.mute}" stroke-dasharray="6 4"/>` +
    txt(W / 2, H / 2 - 6, tpl, { size: 20, weight: 700, fill: C.mute, mono: true }) + txt(W / 2, H / 2 + 20, p || ' ', { size: 12, fill: C.mute, mono: true }));
}

export function render(tpl, params) {
  return TEMPLATES[tpl] ? TEMPLATES[tpl](params || {}) : generic(tpl, params);
}
