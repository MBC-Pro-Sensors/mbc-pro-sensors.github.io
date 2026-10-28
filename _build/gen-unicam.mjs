// 神攝手（unicam，韌體 v1）產生器：讀 _build/protocol/unicam/cam_universal_layout.json（單一來源，複製自相機韌體的 tests\spec_layout.json）
//   sensors/unicam/reference.md、en/sensors/unicam/reference.md   逐格對照（每個上位機一節、每個「相機模式・子選項」一張摺疊表）
//   sensors/unicam/index.md（＋en）的標記片段：
//     <!-- gen:unicam-counts -->…<!-- /gen -->  各上位機給幾色總表
//     <!-- gen:unicam-hosts -->…<!-- /gen -->   ① 選你的主機（格子）
//   images/sensors/unicam/*.svg                                    示意圖（中英共用，圖上只有數字與 X／Y 這類符號）
//
// 用法：node _build/gen-unicam.mjs           產生（有差異才覆寫）
//       node _build/gen-unicam.mjs --check   重產比對；不一致（或副本跟相機那邊的 spec_layout.json 不同）時結束碼 1
//       node _build/gen-unicam.mjs --sync    先從相機資料夾重新複製 spec_layout.json，再產生
//
// 規格改了：先在相機那邊改說明網頁並重新匯出 spec_layout.json，再跑 --sync。不要手改 cam_universal_layout.json 或產生的頁面。
import fs from 'node:fs';
import path from 'node:path';
import { ROOT, makeWriter } from './gen-lib.mjs';

const SRC = 'D:\\AI_Agent_AI助理\\02_MBC產品開發\\02_相機感應器\\06_通用版\\通用版v1\\tests\\spec_layout.json';
const LAYOUT_REL = '_build/protocol/unicam/cam_universal_layout.json';
const argv = process.argv.slice(2);
const CHECK = argv.includes('--check');
const SYNC = argv.includes('--sync');
const w = makeWriter({ check: CHECK, label: 'gen-unicam' });

// ---------------------------------------------------------------- 單一來源
const wrap = (entries, copied) => JSON.stringify({
  _說明: '神攝手（韌體 v1）逐格對照（網站的單一來源）。複製自相機韌體資料夾 tests\\spec_layout.json（說明網頁第 39 版匯出，規格定案 2026-10-28）。不要手改：規格改了先在相機那邊重新匯出，再跑 node _build/gen-unicam.mjs --sync。',
  source: SRC,
  copied,
  entries,
}, null, 1) + '\n';
const layoutAbs = path.join(ROOT, LAYOUT_REL);
if (SYNC) {
  if (!fs.existsSync(SRC)) { console.error(`找不到來源 ${SRC}`); process.exit(2); }
  fs.mkdirSync(path.dirname(layoutAbs), { recursive: true });
  fs.writeFileSync(layoutAbs, wrap(JSON.parse(fs.readFileSync(SRC, 'utf8')), new Date().toISOString().slice(0, 10)));
  console.log(`已複製 ${SRC} → ${LAYOUT_REL}`);
}
const LAYOUT = JSON.parse(fs.readFileSync(layoutAbs, 'utf8'));
const D = LAYOUT.entries;
let srcDiff = false;
if (fs.existsSync(SRC) && JSON.stringify(JSON.parse(fs.readFileSync(SRC, 'utf8'))) !== JSON.stringify(D)) {
  srcDiff = true;
  console.log(`${LAYOUT_REL} 跟來源 spec_layout.json 不一樣：跑 node _build/gen-unicam.mjs --sync`);
}

// ---------------------------------------------------------------- 名稱
const HOSTS = [
  { id: 'pyb', pg: 'spike', page: 'spike/index.md',
    short: { zh: 'Pybricks 函式庫', en: 'Pybricks library' },
    title: { zh: 'Pybricks 函式庫（SPIKE，讀模式 9／6）', en: 'Pybricks library (SPIKE, modes 9 / 6)' },
    note: {
      zh: '裝 MBC Pybricks 函式庫後讀模式 9：7 個 int16＝14 byte，一個 byte 一個值。座標用 160×120 像素（Y 上＝小），0＝沒有目標；會有負數的值（循線偏移、角度、左右緣）是有號 byte（二補數）。最多色層每色 1 byte＝16×12 格的格號 1～192，函式庫自動拆成 X、Y；最多色、多色兩層的數量放模式 6（要數量時才讀）；模式 8 是裝置 ID。這一代神攝手的模式 9 排法跟 Kito 版不同，Kito 的 camLibForKitoMode9 不能直接用。',
      en: 'With the MBC Pybricks library the hub reads mode 9: 7 int16 = 14 bytes, one value per byte. Coordinates are 160×120 pixels (Y: small at top), 0 = no target; values that can be negative (line offsets, angle, edges) are signed bytes (two\'s complement). In the Max-colors level each color is one byte = cell number 1~192 on a 16×12 grid, split into X and Y by the library; counts for the Max and Multi levels are in mode 6 (read only when needed); mode 8 is the device ID. This generation\'s mode 9 layout differs from the Kito edition, so the Kito camLibForKitoMode9 cannot be reused.' } },
  { id: 'edu', pg: 'spike', page: 'spike/index.md',
    short: { zh: 'App 教育版', en: 'SPIKE App' },
    title: { zh: 'SPIKE App 教育版（SPIKE Prime 黃色主機）', en: 'SPIKE App (SPIKE Prime, yellow hub)' },
    note: {
      zh: 'SPIKE App（3.x）把相機當成顏色感應器，讀模式 0：「顏色」「反射光」各 1 byte，「原始紅／綠／藍」各 int16，這 8 byte 積木讀得到。標準、精細兩層照 Kito（16×12 格，X＋Y×256：取餘數、÷256）；最多色、多色兩層用 8×6 格、一格兩色（X×10＋Y 再 ×100 合併）。Y 上＝小。byte 8～15 積木讀不到，要用 App 的 Python（待上機確認）。',
      en: 'The SPIKE App (3.x) sees the camera as a Color Sensor and reads mode 0: "color" and "reflected light" are one byte each, "raw red / green / blue" are int16 each; the blocks can read these 8 bytes. The Standard and Fine levels follow Kito (16×12 grid, X + Y×256: mod 256, ÷256); the Max and Multi levels use an 8×6 grid with two colors per slot (X×10+Y, merged ×100). Y: small at top. Bytes 8~15 are not readable from blocks, only from the app\'s Python (to be confirmed on hardware).' } },
  { id: 'toy', pg: 'spike', page: 'spike/index.md',
    short: { zh: 'App 家用版', en: 'Robot Inventor App' },
    title: { zh: 'SPIKE App 家用版（Robot Inventor 藍綠色主機）', en: 'Robot Inventor App (home edition, teal hub)' },
    note: {
      zh: 'Robot Inventor App 讀模式 0，共 8 byte；主機送 0x25 時相機自動換成家用版排法。「顏色」只到 0～10、「反射光」最多 126、「原始紅綠藍」0～255，每格放 1 個值。',
      en: 'The Robot Inventor App reads mode 0, 8 bytes in total; the camera switches to the home-edition layout by itself when the hub sends 0x25. "Color" only goes to 0~10, "reflected light" up to 126 and "raw red / green / blue" 0~255, one value per slot.' } },
  { id: 'pyblk', pg: 'spike', page: 'spike/index.md',
    short: { zh: 'Pybricks 積木', en: 'Pybricks blocks' },
    title: { zh: 'Pybricks 內建積木（不裝函式庫）', en: 'Pybricks built-in blocks (no library)' },
    note: {
      zh: '不裝 MBC 函式庫，只用 Pybricks 內建的顏色感應器積木：reflection()（模式 5）和 ambient()（模式 7）。顏色追蹤跟 Kito 版一樣：reflection()＝色 1 的 X×100＋Y、ambient()＝色 2 的 X.Y（40×30 格）。reflection() 超過 3199 會顯示成負數，加 6399 修回。',
      en: 'No MBC library, only the Pybricks built-in color sensor blocks: reflection() (mode 5) and ambient() (mode 7). Color tracking works like the Kito edition: reflection() = color 1 as X×100+Y, ambient() = color 2 as X.Y (40×30 grid). reflection() above 3199 shows up negative; add 6399 to fix it.' } },
  { id: 'ev3', pg: 'ev3', page: 'ev3/index.md',
    short: { zh: 'EV3', en: 'EV3' },
    title: { zh: 'EV3（EV3-G／EV3 Classroom／EV3 MicroPython）', en: 'EV3 (EV3-G / EV3 Classroom / EV3 MicroPython)' },
    note: {
      zh: '相機在 EV3 顯示成原廠紅外線感應器（type 33）。模式 0 近距＝相機模式編號、模式 1 信標 8 個值、模式 2 遙控 4 個值，每個值 int8（−127～100）。子選項 ①、照順序給色號、循線會用到遙控（模式 2），其他層只讀信標；換模式約 0.5 秒。位置用 8×6 格（X×10＋Y，Y 下＝小）。模式 5＝裝置 ID。',
      en: 'On the EV3 the camera shows up as a stock Infrared Sensor (type 33). Mode 0 proximity = camera mode number, mode 1 beacon = 8 values, mode 2 remote = 4 values, each an int8 (−127~100). Sub-option ①, Color order and Line following also use the remote mode (mode 2); the other levels only need the beacon mode. Switching modes takes about 0.5 s. Positions use an 8×6 grid (X×10+Y, Y: small at the bottom). Mode 5 = device ID.' } },
  { id: 'mx', pg: 'matrix', page: 'matrix/mvision.md',
    short: { zh: 'MATRIX', en: 'MATRIX' },
    title: { zh: 'MATRIX Mini R4（mVision 10 格）', en: 'MATRIX Mini R4 (mVision, 10 slots)' },
    note: {
      zh: 'MATRIX Mini R4 用 MATRIXblock 的 mVision 積木讀：10 格 uint16（表上的「格 1」＝積木的「資料 0」）。一格放 2 個 byte 值（A＋B×256，取餘數、÷256 拆）或一個像素座標；座標用 160×120 像素（Y 上＝小），0＝沒有目標；會有負數的值加 100 送。',
      en: 'The MATRIX Mini R4 reads the camera with the MATRIXblock mVision blocks: 10 uint16 slots ("slot 1" below = "data 0" in the block). A slot holds two byte values (A + B×256: mod 256, ÷256) or one pixel coordinate; coordinates are 160×120 pixels (Y: small at top), 0 = no target; values that can be negative are sent +100.' } },
  { id: 'i2c', pg: 'arduino', page: 'arduino/index.md',
    short: { zh: 'I2C', en: 'I2C' },
    title: { zh: 'I2C 0x4C（Arduino／micro:bit）', en: 'I2C 0x4C (Arduino / micro:bit)' },
    note: {
      zh: 'I2C 位址 0x4C（Arduino、ESP32、UNO R4、micro:bit）。暫存器連續讀，不受格數限制：寫值編號 → 等 2 ms → 讀 2×k byte（int16 小端），一次最多 16 個值。表上的「值 1」＝編號 0x10、「值 2」＝0x11，依序往下。座標用 160×120 像素（Y 上＝小），0＝沒有目標；負數照原值送。每一層都給 16 色／16 點，層級只決定給哪些值。mBlock（mBot 等 Makeblock 主板）讀同一張值表：UART 38400，送 1 byte [編號] → 回 [值低][值高][編號^值低^值高]。',
      en: 'I2C address 0x4C (Arduino, ESP32, UNO R4, micro:bit). Registers are read in bursts with no slot limit: write the value number → wait 2 ms → read 2×k bytes (int16, little-endian), up to 16 values per read. "Value 1" below = register 0x10, "value 2" = 0x11 and so on. Coordinates are 160×120 pixels (Y: small at top), 0 = no target; negative values are sent as they are. Every level gives 16 colors / 16 points; the level only decides which values. mBlock (mBot and other Makeblock boards) reads the same value table over UART 38400: send 1 byte [id] → reply [low][high][id^low^high].' } },
];
const HOST = Object.fromEntries(HOSTS.map((h) => [h.id, h]));
const MODE = { 1: { zh: '顏色追蹤', en: 'Color tracking' }, 2: { zh: '座標取色', en: 'Color at points' }, 3: { zh: '照順序給色號', en: 'Color order' }, 4: { zh: '循線', en: 'Line following' } };
const SUBS = [
  [1, 1, { zh: '① 最多色', en: '① Max colors' }],
  [1, 2, { zh: '② 多色', en: '② Multi-color' }],
  [1, 3, { zh: '③ 標準（XY＋面積）', en: '③ Standard (XY + area)' }],
  [1, 4, { zh: '④ 精細（XYWH）', en: '④ Fine (XYWH)' }],
  [2, 1, { zh: '① 色相', en: '① Hue' }],
  [2, 2, { zh: '② 色相＋飽和', en: '② Hue + saturation' }],
  [2, 3, { zh: '③ 色相＋飽和＋明度', en: '③ Hue + saturation + value' }],
  [3, 0, { zh: '（沒有子選項）', en: '(no sub-options)' }],
  [4, 1, { zh: '①②③ 黑線／白線／色線', en: '①②③ black / white / colored line' }],
];
const T = (lang, zh, en) => (lang === 'en' ? en : zh);
const L = (o, lang) => o[lang];
const pre = (lang) => (lang === 'en' ? '/en' : '');
const entry = (h, m, s) => {
  const e = D.find((x) => x.host === h && x.mode === m && x.sub === s);
  if (!e) throw new Error(`缺 ${h} ${m}-${s}`);
  return e;
};
const code = (m, s) => `${m}${s}`;
const anchor = (h, m, s) => `${h}-${code(m, s)}`;
// 循線三個子選項的排法要一樣（頁面只列一張）
for (const h of HOSTS) {
  const a = [1, 2, 3].map((s) => JSON.stringify(entry(h.id, 4, s).rows));
  if (a[0] !== a[1] || a[1] !== a[2]) throw new Error(`${h.id} 循線三個子選項排法不同，頁面要改成分開列`);
}
if (D.length !== 77) throw new Error(`entries 應有 77 筆，現在 ${D.length}`);

// ---------------------------------------------------------------- 英文對照（照順序替換；最後檢查不能留下中文）
const EN_RULES = [
  // 整句
  [/色 1～14 的數量各 3 bit（0～7，7＝7 塊以上）＋找到幾色 4 bit，共 46 bit 放 6 B（小端），函式庫自動拆/g, 'counts of colors 1~14, 3 bits each (0~7, 7 = 7 or more) + colors found, 4 bits: 46 bits in 6 B (little-endian), split by the library'],
  [/切換模式與子選項；設定目標色、最小面積門檻、取樣點位置/g, 'switch camera mode and sub-option; set target colors, minimum area and sample-point positions'],
  [/相機模式編號（例 13＝顏色追蹤 ③ 標準）/g, 'camera mode number (e.g. 13 = Color tracking ③ Standard)'],
  [/SPIKE read\(6\)：要數量時才讀（換模式多等 30 ms；模式 8 是裝置 ID）/g, 'SPIKE read(6): only when you need counts (switching modes adds 30 ms; mode 8 is the device ID)'],
  [/App Python device\.data\(\)：byte 8～15/g, 'App Python device.data(): bytes 8~15'],
  [/讀（值編號照順序）/g, 'read (value numbers in order)'],
  [/^寫$/g, 'write'],
  [/mVision 10 格/g, 'mVision, 10 slots'],
  [/官方積木（模式 0）/g, 'stock blocks (mode 0)'],
  [/模式 (\d+) 近距（1 個值）/g, 'mode $1 proximity (1 value)'],
  [/模式 (\d+) 信標（(\d+) 個值）/g, 'mode $1 beacon ($2 values)'],
  [/模式 (\d+) 遙控（(\d+) 個值）/g, 'mode $1 remote ($2 values)'],
  [/模式 (\d+)/g, 'mode $1'],
  [/（不用，送 0）/g, '(unused, sends 0)'],
  [/（＋(\d+)）/g, ' (+$1)'],
  [/（0 無 1 十字 2 T 3 左 4 右 5 盡頭）/g, ' (0 none, 1 cross, 2 T, 3 left, 4 right, 5 dead end)'],
  [/（近 1＋中 2＋遠 4）/g, ' (near 1 + middle 2 + far 4)'],
  [/（0 有線 1 左 2 右）/g, ' (0 on the line, 1 left, 2 right)'],
  [/（EV3：正＝色相、負＝黑白）/g, ' (EV3: positive = hue, negative = black / white)'],
  [/（0～7，7＝7 塊以上）/g, ' (0~7, 7 = 7 or more)'],
  [/函式庫拆成 X、Y/g, 'the library splits it into X, Y'],
  [/上＝小/g, 'small at top'],
  [/(\d+)×(\d+) 格/g, '$1×$2 grid'],
  [/(\d+) 格(?!號)/g, '$1 cells'],
  [/佔畫面 %/g, '% of frame'],
  [/像素/g, 'px'],
  [/格號/g, 'cell number'],
  [/色相碼 除以 100 的餘數/g, 'hue code mod 100'],
  [/百位/g, 'hundreds'],
  [/左起第 (\d+) 塊色相/g, 'hue of block $1 from the left'],
  [/總塊數/g, 'total blocks'],
  [/找到幾色/g, 'colors found'],
  [/近處偏移/g, 'near offset'], [/中段偏移/g, 'middle offset'], [/遠處偏移/g, 'far offset'],
  [/線角度/g, 'line angle'], [/近處線寬/g, 'near line width'], [/遠處線寬/g, 'far line width'],
  [/有線旗標/g, 'line-found flags'], [/路口類型/g, 'junction type'], [/路口距離/g, 'junction distance'],
  [/近處線左緣/g, 'near line left edge'], [/近處線右緣/g, 'near line right edge'],
  [/穿過的彩色標記線色號/g, 'color number of the marker line crossed'], [/標記線累計條數/g, 'marker lines crossed so far'],
  [/最後離線方向/g, 'side the line was last lost'],
  [/(\d+) 點原始色相/g, '$1 point(s) raw hue'],
  [/(\d+) 點色相/g, '$1 points hue'],
  [/(\d+) 塊色相碼/g, '$1 blocks hue code'],
  [/^1 色 /g, '1 color '],
  [/(\d+) 色( |$)/g, '$1 colors$2'],
  [/色 (\d+)/g, 'color $1'],
  [/色相碼/g, 'hue code'], [/色相/g, 'hue'], [/飽和度/g, 'saturation'], [/飽和明度/g, 'sat/value'], [/飽和/g, 'sat'], [/明度/g, 'value'],
  [/數量/g, 'count'], [/面積/g, 'area'], [/寬/g, 'width'], [/高/g, 'height'],
  // 位置欄
  [/通道 (\d+) 方向/g, 'channel $1 heading'], [/通道 (\d+) 距離/g, 'channel $1 distance'], [/通道 (\d+)/g, 'channel $1'],
  [/^值 (\d+)/g, 'value $1'], [/^值$/g, 'value'],
  [/^顏色$/g, 'color'], [/^反射光$/g, 'reflected light'], [/^原始紅$/g, 'raw red'], [/^原始綠$/g, 'raw green'], [/^原始藍$/g, 'raw blue'],
  [/^byte (\d+)～(\d+)$/g, 'bytes $1~$2'], [/^解出$/g, 'decoded'], [/^格 (\d+)$/g, 'slot $1'],
  // 範圍欄
  [/÷(\d+) 拆開/g, 'split ÷$1'], [/同 Kito/g, 'same as Kito'], [/餘數、÷256/g, 'mod 256, ÷256'],
  // 標點
  [/（/g, ' ('], [/）/g, ')'], [/＋/g, ' + '], [/＝/g, ' = '], [/，/g, ', '], [/；/g, '; '], [/、/g, ', '], [/：/g, ': '], [/～/g, '~'],
];
const untranslated = new Set();
function en(s) {
  if (s == null) return s;
  let t = s;
  for (const [re, to] of EN_RULES) t = t.replace(re, to);
  t = t.replace(/\s+/g, ' ').replace(/\(\s+/g, '(').replace(/\s+\)/g, ')').replace(/\s+,/g, ',').trim();
  if (/[\u3000-\u303f\u3400-\u9fff\uff00-\uffef]/.test(t)) untranslated.add(`${s} → ${t}`);
  return /^(mVision|reflection\(|ambient\()/.test(t) ? t : t.charAt(0).toUpperCase() + t.slice(1);
}
const tx = (s, lang) => (lang === 'en' ? en(s) : s);

// ---------------------------------------------------------------- 表格
const esc = (s) => String(s ?? '').replace(/\|/g, '\\|');
function posText(h, p, lang) {
  if (!p) return '';
  const m = h === 'i2c' && p.match(/^值 (\d+)$/);
  if (m) return `${tx(p, lang)} · \`0x${(0x0f + +m[1]).toString(16).toUpperCase()}\``;
  return tx(p, lang);
}
function layoutTable(e, lang) {
  const out = [`| ${T(lang, '位置', 'Position')} | ${T(lang, '內容', 'Content')} | ${T(lang, '範圍', 'Range')} |`, '| :--- | :--- | :--- |'];
  for (const r of e.rows) {
    if (r.g) out.push(`| **${esc(tx(r.g, lang))}** | | |`);
    else if (r.t) out.push(`| ${esc(posText(e.host, r.p, lang))} | ${esc(tx(r.t, lang))} | |`);
    else out.push(`| ${esc(posText(e.host, r.p, lang))} | ${esc(tx(r.lab, lang))} | ${esc(tx(r.rng, lang))} |`);
  }
  return out.join('\n');
}
// 循線：這個上位機給幾個值（不算模式編號與不用的格）
function lineValueCount(h) {
  const keys = new Set();
  for (const r of entry(h, 4, 1).rows) {
    const ks = Array.isArray(r.k) ? r.k : r.k ? [r.k] : [];
    for (const k of ks) if (k !== 'fn') keys.add(k.replace(/\+100$/, ''));
  }
  return keys.size;
}
function countsTable(lang) {
  const ref = `${pre(lang)}/sensors/unicam/reference.md`;
  const head = `| ${T(lang, '相機模式・子選項', 'Camera mode · sub-option')} | ${HOSTS.map((h) => L(h.short, lang)).join(' | ')} |`;
  const rows = [head, `| :--- | ${HOSTS.map(() => ':---:').join(' | ')} |`];
  for (const [m, s, sub] of SUBS) {
    const label = `${m} ${L(MODE[m], lang)} ${m === 3 ? '' : L(sub, lang)}`.trim();
    rows.push(`| ${label} | ${HOSTS.map((h) => {
      const e = entry(h.id, m, s);
      const txt = m === 4 ? T(lang, `${lineValueCount(h.id)} 個值`, `${lineValueCount(h.id)} values`) : tx(e.lab, lang);
      return `[${txt}](${ref}#${anchor(h.id, m, s)})`;
    }).join(' | ')} |`);
  }
  return rows.join('\n');
}

// ---------------------------------------------------------------- 讀值參考頁
const AI_NOTE = '<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->';
const GEN_NOTE = '<!-- 本頁由 _build/gen-unicam.mjs 從 _build/protocol/unicam/cam_universal_layout.json 產生，請勿手改；規格改了跑 node _build/gen-unicam.mjs --sync -->';
const notVerified = (lang) => T(lang,
  '> [!WARNING]\n> **韌體 v1 尚未上機驗證**：數值排法已定案並通過電腦端測試，實機數值（fps、循線門檻、I2C 腳位）上機後可能調整。',
  '> [!WARNING]\n> **Firmware v1 not yet hardware-verified**: the value layout is final and passes the computer-side tests, but real-hardware numbers (fps, line-following thresholds, I2C pins) may change after bench testing.');
function referencePage(lang) {
  const P = pre(lang);
  const out = [AI_NOTE, GEN_NOTE];
  out.push(`# ${T(lang, '讀值參考：逐格對照', 'Value reference: slot by slot')}`, '');
  out.push(notVerified(lang), '');
  out.push(T(lang,
    `神攝手在每個上位機、每個「相機模式・子選項」送什麼值、放在哪一格。先在相機上選好模式（看 [產品首頁](/sensors/unicam/index.md) 的「相機上的操作」），再到這裡找你的上位機那一節，點開對應的表。各平台怎麼寫程式讀，看 [SPIKE](/sensors/unicam/spike/index.md)、[EV3](/sensors/unicam/ev3/index.md)、[MATRIX](/sensors/unicam/matrix/mvision.md)、[Arduino](/sensors/unicam/arduino/index.md)、[micro:bit](/sensors/unicam/microbit/index.md)。`,
    `What the Sharpshooter Vision sends on each host for each "camera mode · sub-option", and in which slot. Pick the mode on the camera first (see the [product page](/en/sensors/unicam/index.md)), then find your host below and open the matching table. For code on each host see [SPIKE](/en/sensors/unicam/spike/index.md), [EV3](/en/sensors/unicam/ev3/index.md), [MATRIX](/en/sensors/unicam/matrix/mvision.md), [Arduino](/en/sensors/unicam/arduino/index.md) and [micro:bit](/en/sensors/unicam/microbit/index.md).`), '');
  out.push(`## ${T(lang, '怎麼看這些表', 'How to read the tables')}`, '');
  out.push(T(lang,
    [
      '- **0＝沒有目標**：座標、格號、面積是 0 就是沒找到（有找到時 X、Y 至少送 1）。色相碼的 0 是紅色，要搭配面積或總塊數判斷。',
      '- **「÷100 拆開」**：兩個值合成一格 A×100＋B，A＝讀到的數 ÷100（取整數）、B＝除以 100 的餘數。「÷10 拆開」同理。',
      '- **「餘數、÷256」**：一格＝A＋B×256，A＝讀到的數除以 256 的餘數、B＝÷256（取整數）。',
      '- **「A.BB」**：Pybricks `ambient()` 顯示成小數，整數部分＝A、小數兩位＝B。',
      '- **「（＋100）」**：會有負數的值加 100 再送，讀到後減 100。',
      '- 座標：像素 X 0～159（左→右）、Y 0～119（上→下）；EV3 的 Y 是「下＝小」。各種格子（16×12、8×6、40×30）的換算看 [產品首頁的數值規則](/sensors/unicam/index.md)。',
    ].join('\n'),
    [
      '- **0 = no target**: a coordinate, cell number or area of 0 means nothing was found (when something is found X and Y are at least 1). Hue code 0 is red, so check the area or the total blocks as well.',
      '- **"split ÷100"**: two values share one slot as A×100 + B; A = reading ÷ 100 (integer part), B = remainder of ÷100. "split ÷10" works the same way.',
      '- **"mod 256, ÷256"**: slot = A + B×256; A = reading mod 256, B = reading ÷ 256 (integer part).',
      '- **"A.BB"**: Pybricks `ambient()` shows a decimal: integer part = A, two decimals = B.',
      '- **"(+100)"**: values that can be negative are sent +100; subtract 100 after reading.',
      '- Coordinates: pixel X 0~159 (left → right), Y 0~119 (top → bottom); on EV3, Y is small at the bottom. Grid conversions (16×12, 8×6, 40×30) are on the [product page](/en/sensors/unicam/index.md).',
    ].join('\n')), '');
  out.push('<a id="counts"></a>', '');
  out.push(`## ${T(lang, '各上位機給幾色', 'Colors / points per host')}`, '');
  out.push(T(lang, '點表格裡的數字直接跳到那張表。子選項＝層級：層級越精細，每色給的值越多、色數越少。', 'Click a number to jump to its table. Sub-option = level: the finer the level, the more values per color and the fewer colors.'), '');
  out.push(countsTable(lang), '');
  for (const h of HOSTS) {
    out.push(`<a id="host-${h.id}"></a>`, '');
    out.push(`## ${L(h.title, lang)}`, '');
    out.push(L(h.note, lang), '');
    for (const [m, s, sub] of SUBS) {
      const e = entry(h.id, m, s);
      const count = m === 4 ? T(lang, `${lineValueCount(h.id)} 個值`, `${lineValueCount(h.id)} values`) : tx(e.lab, lang);
      const sum = `${T(lang, '相機模式', 'Camera mode')} ${m} ${L(MODE[m], lang)}${m === 3 ? '' : ' ' + L(sub, lang)}${T(lang, '：', ': ')}${count}`;
      out.push(`<details class="plat pg-${h.pg}" id="${anchor(h.id, m, s)}">`, `<summary><span class="plat-dot"></span>${sum}</summary>`, '');
      out.push(T(lang, `模式編號 ${m === 4 ? '41／42／43' : code(m, s)}。`, `Mode number ${m === 4 ? '41 / 42 / 43' : code(m, s)}.`), '');
      out.push('<div>', '', layoutTable(e, lang), '', '</div>', '', '</details>', '');
    }
  }
  out.push(`## ${T(lang, '本頁依據', 'Sources')}`, '');
  out.push(T(lang,
    `- 逐格對照：\`cam_universal_layout.json\`（在 \`_build/protocol/unicam/\`，複製自相機韌體資料夾的 \`tests\\spec_layout.json\`，規格定案 2026-10-28，韌體每次建置都跟它逐 byte 比對）`,
    `- Slot layout: \`cam_universal_layout.json\` (in \`_build/protocol/unicam/\`, copied from \`tests\\spec_layout.json\` in the camera firmware folder, spec frozen 2026-10-28; the firmware is checked against it byte by byte on every build)`));
  out.push(T(lang, `- 產生器：\`_build/gen-unicam.mjs\``, `- Generator: \`_build/gen-unicam.mjs\``), '');
  return out.join('\n');
}

// ---------------------------------------------------------------- 產品頁：① 選你的主機（格子）
const TILES = [
  { pg: 'spike', page: 'spike/index.md', img: '/images/illust/host-spike.webp', name: 'SPIKE',
    tag: { zh: 'SPIKE Prime（黃色主機）與 Robot Inventor（藍綠色主機）', en: 'SPIKE Prime (yellow hub) and Robot Inventor (teal hub)' },
    sw: { zh: 'Pybricks 函式庫・Pybricks 積木・官方 App', en: 'Pybricks library · Pybricks blocks · LEGO app' } },
  { pg: 'ev3', page: 'ev3/index.md', img: '/images/illust/host-ev3.webp', name: 'EV3',
    tag: { zh: 'MINDSTORMS EV3，顯示成紅外線感應器', en: 'MINDSTORMS EV3, shows up as an Infrared Sensor' },
    sw: { zh: 'EV3-G・EV3 Classroom・EV3 MicroPython', en: 'EV3-G · EV3 Classroom · EV3 MicroPython' } },
  { pg: 'matrix', page: 'matrix/mvision.md', img: '/images/illust/host-matrix.webp', name: 'MATRIX',
    tag: { zh: 'MATRIX Mini R4，mVision 攝影機積木', en: 'MATRIX Mini R4, mVision camera blocks' },
    sw: { zh: 'mVision 積木', en: 'mVision blocks' } },
  { pg: 'arduino', page: 'arduino/index.md', img: '/images/illust/host-arduino.webp', name: 'Arduino',
    tag: { zh: 'UNO、Nano、UNO R4、ESP32，用 I2C（0x4C）', en: 'UNO, Nano, UNO R4, ESP32 over I2C (0x4C)' },
    sw: { zh: 'Arduino IDE', en: 'Arduino IDE' } },
  { pg: 'microbit', page: 'microbit/index.md', img: '/images/illust/host-microbit.webp', name: 'micro:bit',
    tag: { zh: 'micro:bit V1／V2，用 I2C（0x4C）', en: 'micro:bit V1 / V2 over I2C (0x4C)' },
    sw: { zh: 'MakeCode', en: 'MakeCode' } },
  { pg: 'mblock', page: 'mblock/index.md', img: '/images/illust/host-mblock.webp', name: 'mBlock',
    tag: { zh: 'mBot、Ranger、Orion、MegaPi，RJ25 串口 38400', en: 'mBot, Ranger, Orion, MegaPi over RJ25 serial 38400' },
    sw: { zh: 'mBlock 5（MBC 感應器擴展 v2.1.0）', en: 'mBlock 5 (MBC Pro Sensors extension v2.1.0)' } },
];
function hostTiles(lang) {
  const P = pre(lang);
  const tiles = TILES.map((t) => `<a class="pick-tile pg-${t.pg}" href="${P}/sensors/unicam/${t.page}"><span class="pt-img"><img src="${t.img}" alt="" loading="lazy"></span><span class="pt-name">${t.name}</span><span class="pt-tag">${L(t.tag, lang)}</span><span class="pt-sw">${L(t.sw, lang)}</span></a>`);
  return `<div class="pick" id="pick-host"><h2 class="pick-title">${T(lang, '① 選你的主機', '① Pick your host')}</h2><p class="pick-sub">${T(lang, '每個主機一頁：怎麼接、怎麼讀、最小的程式寫法。逐格對照在「讀值參考」。', 'One page per host: wiring, reading and the smallest working code. The slot-by-slot layout is on the value reference page.')}</p>\n<div class="pick-grid pick-hosts">\n${tiles.join('\n')}\n</div></div>`;
}

// ---------------------------------------------------------------- 示意圖（SVG）
const FONT = `font-family='Nunito, "Segoe UI", Arial, sans-serif'`;
const svg = (wd, ht, body, label) => `<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 ${wd} ${ht}" width="${wd}" height="${ht}" role="img" aria-label="${label}" ${FONT}>\n<rect width="${wd}" height="${ht}" rx="12" fill="#ffffff"/>\n${body}\n</svg>\n`;
const C = { red: '#D64545', yellow: '#D9A400', green: '#2F9E5B', blue: '#2F6FD6', cyan: '#19A7B5', purple: '#8E44AD', ink: '#1f2933', mute: '#5b6b78', rule: '#c9d3dc', brand: '#0a7c78', frame: '#f6f8fa' };
const r1 = (n) => Math.round(n * 10) / 10;
// 160×120 畫面放大 S 倍，左上角在 (ox, oy)
function frame(ox, oy, S, fill = C.frame) {
  return `<rect x="${ox}" y="${oy}" width="${160 * S}" height="${120 * S}" rx="6" fill="${fill}" stroke="${C.rule}" stroke-width="1.5"/>`;
}
function axes(ox, oy, S) {
  const W = 160 * S, H = 120 * S;
  return `<g fill="${C.mute}" font-size="13">` +
    `<line x1="${ox}" y1="${oy - 12}" x2="${ox + W}" y2="${oy - 12}" stroke="${C.mute}" stroke-width="1.5" marker-end="url(#ah)"/>` +
    `<text x="${ox}" y="${oy - 18}">0</text><text x="${ox + W - 34}" y="${oy - 18}">X 159</text>` +
    `<line x1="${ox - 12}" y1="${oy}" x2="${ox - 12}" y2="${oy + H}" stroke="${C.mute}" stroke-width="1.5" marker-end="url(#ah)"/>` +
    `<text x="${ox - 28}" y="${oy + 12}">0</text><text x="${ox - 40}" y="${oy + H - 2}" transform="rotate(-90 ${ox - 22} ${oy + H - 30})">Y 119</text></g>`;
}
const ARROW = `<defs><marker id="ah" viewBox="0 0 10 10" refX="9" refY="5" markerWidth="7" markerHeight="7" orient="auto-start-reverse"><path d="M0 0 L10 5 L0 10 z" fill="${C.mute}"/></marker></defs>`;

function figColorTrack() {
  const ox = 56, oy = 40, S = 2.5;
  const P = (x) => ox + x * S, Q = (y) => oy + y * S;
  const blobs = [
    { c: C.red, n: 1, x: 52, y: 70, w: 34, h: 26 },
    { c: C.yellow, n: 2, x: 118, y: 36, w: 22, h: 18 },
    { c: C.blue, n: 3, x: 122, y: 92, w: 16, h: 14 },
  ];
  let s = ARROW + frame(ox, oy, S) + axes(ox, oy, S);
  for (const b of blobs) {
    s += `<rect x="${P(b.x - b.w / 2)}" y="${Q(b.y - b.h / 2)}" width="${b.w * S}" height="${b.h * S}" rx="8" fill="${b.c}" fill-opacity="0.88"/>`;
    s += `<text x="${P(b.x)}" y="${Q(b.y) + 6}" text-anchor="middle" font-size="18" font-weight="700" fill="#fff">${b.n}</text>`;
  }
  const b = blobs[0];
  // 色 1：中心十字、到軸的虛線、寬高尺寸線
  s += `<g stroke="${C.ink}" stroke-width="1.4" fill="none">` +
    `<line x1="${P(b.x)}" y1="${oy}" x2="${P(b.x)}" y2="${Q(b.y) - 10}" stroke-dasharray="4 4"/>` +
    `<line x1="${ox}" y1="${Q(b.y)}" x2="${P(b.x) - 12}" y2="${Q(b.y)}" stroke-dasharray="4 4"/>` +
    `<line x1="${P(b.x - b.w / 2)}" y1="${Q(b.y + b.h / 2) + 14}" x2="${P(b.x + b.w / 2)}" y2="${Q(b.y + b.h / 2) + 14}"/>` +
    `<line x1="${P(b.x - b.w / 2)}" y1="${Q(b.y + b.h / 2) + 8}" x2="${P(b.x - b.w / 2)}" y2="${Q(b.y + b.h / 2) + 20}"/><line x1="${P(b.x + b.w / 2)}" y1="${Q(b.y + b.h / 2) + 8}" x2="${P(b.x + b.w / 2)}" y2="${Q(b.y + b.h / 2) + 20}"/>` +
    `<line x1="${P(b.x + b.w / 2) + 14}" y1="${Q(b.y - b.h / 2)}" x2="${P(b.x + b.w / 2) + 14}" y2="${Q(b.y + b.h / 2)}"/>` +
    `<line x1="${P(b.x + b.w / 2) + 8}" y1="${Q(b.y - b.h / 2)}" x2="${P(b.x + b.w / 2) + 20}" y2="${Q(b.y - b.h / 2)}"/><line x1="${P(b.x + b.w / 2) + 8}" y1="${Q(b.y + b.h / 2)}" x2="${P(b.x + b.w / 2) + 20}" y2="${Q(b.y + b.h / 2)}"/>` +
    `</g>`;
  s += `<g font-size="15" font-weight="700" fill="${C.ink}">` +
    `<text x="${P(b.x) + 6}" y="${oy + 16}">X</text><text x="${ox + 6}" y="${Q(b.y) - 6}">Y</text>` +
    `<text x="${P(b.x)}" y="${Q(b.y + b.h / 2) + 36}" text-anchor="middle">W</text><text x="${P(b.x + b.w / 2) + 26}" y="${Q(b.y) + 5}">H</text></g>`;
  return svg(480, 380, s, 'Color tracking: X, Y, W, H of each color');
}

function figSamplePoints() {
  const ox = 40, oy = 30, S = 2.5;
  const P = (x) => ox + x * S, Q = (y) => oy + y * S;
  let s = frame(ox, oy, S);
  // 背景色塊（讓取樣點有東西可看）
  s += `<rect x="${P(0)}" y="${Q(0)}" width="${48 * S}" height="${60 * S}" fill="${C.red}" fill-opacity="0.75"/>`;
  s += `<rect x="${P(48)}" y="${Q(0)}" width="${48 * S}" height="${60 * S}" fill="${C.yellow}" fill-opacity="0.75"/>`;
  s += `<rect x="${P(96)}" y="${Q(0)}" width="${64 * S}" height="${60 * S}" fill="${C.green}" fill-opacity="0.75"/>`;
  s += `<rect x="${P(0)}" y="${Q(60)}" width="${80 * S}" height="${60 * S}" fill="${C.blue}" fill-opacity="0.75"/>`;
  s += `<rect x="${P(80)}" y="${Q(60)}" width="${80 * S}" height="${60 * S}" fill="${C.purple}" fill-opacity="0.7"/>`;
  // 預設取樣點：每列 10 點，兩列（9 點以上排兩列）
  let n = 0;
  for (const y of [32, 83]) for (const xp of [5, 15, 25, 35, 45, 55, 65, 75, 85, 95]) {
    n++;
    const x = P(xp * 1.6), yy = Q(y);
    s += `<g stroke="#fff" stroke-width="2.4"><line x1="${x - 7}" y1="${yy}" x2="${x + 7}" y2="${yy}"/><line x1="${x}" y1="${yy - 7}" x2="${x}" y2="${yy + 7}"/></g>`;
    s += `<g stroke="${C.ink}" stroke-width="1.2"><line x1="${x - 7}" y1="${yy}" x2="${x + 7}" y2="${yy}"/><line x1="${x}" y1="${yy - 7}" x2="${x}" y2="${yy + 7}"/></g>`;
    s += `<text x="${x}" y="${yy - 11}" text-anchor="middle" font-size="12" font-weight="700" fill="#fff" stroke="${C.ink}" stroke-width="3" paint-order="stroke">${n}</text>`;
  }
  return svg(480, 360, s, 'Color at points: sample points S1 to S20');
}

function figColorOrder() {
  const ox = 40, oy = 40, S = 2.5;
  const P = (x) => ox + x * S, Q = (y) => oy + y * S;
  let s = ARROW + frame(ox, oy, S);
  const blocks = [[C.blue, 18, 64, 160], [C.red, 46, 40, 0], [C.green, 76, 78, 80], [C.yellow, 106, 52, 40], [C.purple, 136, 70, 200]];
  blocks.forEach(([c, x, y, hue], i) => {
    s += `<rect x="${P(x - 10)}" y="${Q(y - 10)}" width="${20 * S}" height="${20 * S}" rx="6" fill="${c}"/>`;
    s += `<circle cx="${P(x)}" cy="${Q(y - 10) - 16}" r="12" fill="${C.ink}"/><text x="${P(x)}" y="${Q(y - 10) - 11}" text-anchor="middle" font-size="14" font-weight="700" fill="#fff">${i + 1}</text>`;
    s += `<text x="${P(x)}" y="${Q(y) + 6}" text-anchor="middle" font-size="15" font-weight="700" fill="#fff">${hue}</text>`;
  });
  s += `<line x1="${P(6)}" y1="${Q(112)}" x2="${P(154)}" y2="${Q(112)}" stroke="${C.mute}" stroke-width="2" marker-end="url(#ah)"/>`;
  s += `<text x="${P(6)}" y="${Q(112) - 8}" font-size="13" fill="${C.mute}">1 → 5</text>`;
  return svg(480, 380, s, 'Color order: blocks numbered left to right with their hue codes');
}

function figLine() {
  const ox = 40, oy = 34, S = 2.5;
  const P = (x) => ox + x * S, Q = (y) => oy + y * S;
  let s = ARROW + frame(ox, oy, S, '#e8ebe7');
  // 斜看前方：下寬上窄的黑線，往右上偏
  const xb = 64, xt = 104; // 線中心：底部、頂部（像素）
  const lx = (y) => xb + (xt - xb) * (1 - y / 120);
  s += `<polygon points="${P(xb - 12)},${Q(120)} ${P(xb + 12)},${Q(120)} ${P(xt + 6)},${Q(0)} ${P(xt - 6)},${Q(0)}" fill="#1d1f1e"/>`;
  s += `<line x1="${P(80)}" y1="${Q(0)}" x2="${P(80)}" y2="${Q(120)}" stroke="${C.mute}" stroke-width="1.2" stroke-dasharray="3 5"/>`;
  const rows = [[108, '近 near'], [64, '中 mid'], [14, '遠 far']];
  for (const [y, t] of rows) {
    s += `<line x1="${P(0)}" y1="${Q(y)}" x2="${P(160)}" y2="${Q(y)}" stroke="${C.brand}" stroke-width="2" stroke-dasharray="7 5"/>`;
    s += `<text x="${P(2)}" y="${Q(y) - 6}" font-size="13" font-weight="700" fill="${C.brand}">${t}</text>`;
    // 偏移箭頭：中心線 → 線中心
    s += `<line x1="${P(80)}" y1="${Q(y)}" x2="${P(lx(y))}" y2="${Q(y)}" stroke="#e8711e" stroke-width="4" stroke-linecap="round"/>`;
    s += `<circle cx="${P(lx(y))}" cy="${Q(y)}" r="5" fill="#e8711e" stroke="#fff" stroke-width="1.5"/>`;
  }
  // 角度：近→遠的連線與垂直線的夾角
  s += `<line x1="${P(lx(108))}" y1="${Q(108)}" x2="${P(lx(14))}" y2="${Q(14)}" stroke="#e8711e" stroke-width="1.5" stroke-dasharray="2 3"/>`;
  s += `<text x="${P(lx(60)) + 10}" y="${Q(60) + 22}" font-size="12" font-weight="700" fill="#b4530f">角度 angle</text>`;
  s += `<text x="${P(84)}" y="${Q(120) - 6}" font-size="12" fill="${C.mute}">0</text><text x="${P(4)}" y="${Q(120) - 6}" font-size="12" fill="${C.mute}">−100</text><text x="${P(146)}" y="${Q(120) - 6}" font-size="12" fill="${C.mute}">100</text>`;
  return svg(480, 370, s, 'Line following: near, middle and far scan rows with offsets and angle');
}

function hsl(h, sat = 82, l = 50) { return `hsl(${r1(h)},${sat}%,${l}%)`; }
function figHue() {
  const cx = 150, cy = 150, R = 110, r = 70;
  let s = '';
  const seg = 4; // 每段 4 個色相碼
  for (let c = 0; c < 240; c += seg) {
    const a0 = (c * 1.5 - 90) * Math.PI / 180, a1 = ((c + seg) * 1.5 - 90) * Math.PI / 180 + 0.004;
    const p = (rr, a) => `${r1(cx + rr * Math.cos(a))} ${r1(cy + rr * Math.sin(a))}`;
    s += `<path d="M${p(R, a0)} A${R} ${R} 0 0 1 ${p(R, a1)} L${p(r, a1)} A${r} ${r} 0 0 0 ${p(r, a0)} Z" fill="${hsl((c + seg / 2) * 1.5)}"/>`;
  }
  for (const c of [0, 40, 80, 120, 160, 200]) {
    const a = (c * 1.5 - 90) * Math.PI / 180;
    const tx_ = cx + (R + 18) * Math.cos(a), ty = cy + (R + 18) * Math.sin(a) + 5;
    s += `<line x1="${r1(cx + (R + 2) * Math.cos(a))}" y1="${r1(cy + (R + 2) * Math.sin(a))}" x2="${r1(cx + (R + 9) * Math.cos(a))}" y2="${r1(cy + (R + 9) * Math.sin(a))}" stroke="${C.ink}" stroke-width="1.5"/>`;
    s += `<text x="${r1(tx_)}" y="${r1(ty)}" text-anchor="middle" font-size="14" font-weight="700" fill="${C.ink}">${c === 0 ? '0 / 240' : c}</text>`;
  }
  s += `<text x="${cx}" y="${cy - 4}" text-anchor="middle" font-size="16" font-weight="700" fill="${C.ink}">0~240</text>`;
  s += `<text x="${cx}" y="${cy + 16}" text-anchor="middle" font-size="12" fill="${C.mute}">hue</text>`;
  // 241~255：白→灰、黑
  const bx = 330, by = 40, bh = 230, n = 15;
  for (let i = 0; i < n; i++) {
    const c = 241 + i;
    const g = c <= 248 ? Math.round(255 - (248 - c) * 14) : Math.round(70 - (c - 249) * 11);
    s += `<rect x="${bx}" y="${r1(by + i * bh / n)}" width="44" height="${r1(bh / n + 0.4)}" fill="rgb(${g},${g},${g})"/>`;
  }
  s += `<rect x="${bx}" y="${by}" width="44" height="${bh}" fill="none" stroke="${C.rule}" stroke-width="1.5"/>`;
  const lab = (c, t) => `<text x="${bx + 54}" y="${r1(by + (c - 241 + 0.5) * bh / n + 5)}" font-size="13" fill="${C.ink}">${t}</text>`;
  s += lab(241, '241') + lab(248, '248 white') + `<text x="${bx + 54}" y="${r1(by + 12 * bh / n + 5)}" font-size="13" fill="${C.ink}">249~255 black</text>`;
  s += `<line x1="${bx - 6}" y1="${r1(by + 8 * bh / n)}" x2="${bx + 44}" y2="${r1(by + 8 * bh / n)}" stroke="${C.ink}" stroke-width="1.5" stroke-dasharray="3 3"/>`;
  return svg(480, 300, s, 'Hue code 0-255: 0-240 hue ring, 241-248 white to gray, 249-255 black');
}

function figGrid() {
  const ox = 70, oy = 50, S = 2.25;
  const P = (x) => ox + x * S, Q = (y) => oy + y * S;
  let s = frame(ox, oy, S);
  for (let i = 1; i < 8; i++) s += `<line x1="${P(i * 20)}" y1="${Q(0)}" x2="${P(i * 20)}" y2="${Q(120)}" stroke="${C.rule}" stroke-width="1"/>`;
  for (let j = 1; j < 6; j++) s += `<line x1="${P(0)}" y1="${Q(j * 20)}" x2="${P(160)}" y2="${Q(j * 20)}" stroke="${C.rule}" stroke-width="1"/>`;
  // 目標在第 3 欄、上數第 2 列
  s += `<rect x="${P(40)}" y="${Q(20)}" width="${20 * S}" height="${20 * S}" fill="${C.red}" fill-opacity="0.25" stroke="${C.red}" stroke-width="2"/>`;
  s += `<circle cx="${P(50)}" cy="${Q(30)}" r="10" fill="${C.red}"/>`;
  for (let i = 0; i < 8; i++) s += `<text x="${P(i * 20 + 10)}" y="${oy - 10}" text-anchor="middle" font-size="13" font-weight="700" fill="${C.ink}">${i + 1}</text>`;
  for (let j = 0; j < 6; j++) {
    s += `<text x="${ox - 14}" y="${Q(j * 20 + 10) + 5}" text-anchor="middle" font-size="13" font-weight="700" fill="${C.blue}">${j + 1}</text>`;
    s += `<text x="${P(160) + 16}" y="${Q(j * 20 + 10) + 5}" text-anchor="middle" font-size="13" font-weight="700" fill="${C.ev3 || '#b11f1f'}">${6 - j}</text>`;
  }
  s += `<text x="${ox - 14}" y="${oy - 10}" text-anchor="middle" font-size="11" fill="${C.blue}">Y</text>`;
  s += `<text x="${P(160) + 16}" y="${oy - 10}" text-anchor="middle" font-size="11" fill="#b11f1f">EV3</text>`;
  s += `<g font-size="14" font-weight="700"><text x="${ox}" y="${Q(120) + 26}" fill="${C.blue}">SPIKE / MATRIX: 3×10 + 2 = 32</text><text x="${ox}" y="${Q(120) + 46}" fill="#b11f1f">EV3: 3×10 + 5 = 35</text></g>`;
  return svg(480, 380, s, '8 by 6 grid: X times 10 plus Y; EV3 counts Y from the bottom');
}

// ---------------------------------------------------------------- 寫出
for (const lang of ['zh', 'en']) {
  const base = lang === 'en' ? 'en/' : '';
  w.write(`${base}sensors/unicam/reference.md`, referencePage(lang));
  for (const [name, content] of [['unicam-counts', countsTable(lang)], ['unicam-hosts', hostTiles(lang)]]) {
    const r = w.patch(`${base}sensors/unicam/index.md`, name, content);
    if (r === null) { console.log(`${base}sensors/unicam/index.md 缺 <!-- gen:${name} -->…<!-- /gen -->`); process.exitCode = 1; }
  }
}
const FIGS = { 'color-track': figColorTrack, 'sample-points': figSamplePoints, 'color-order': figColorOrder, 'line-follow': figLine, 'hue-code': figHue, 'grid-8x6': figGrid };
for (const [name, fn] of Object.entries(FIGS)) w.write(`images/sensors/unicam/${name}.svg`, fn());

if (untranslated.size) {
  console.log(`英文對照還有中文沒翻（${untranslated.size}）：請在 EN_RULES 補規則`);
  [...untranslated].slice(0, 40).forEach((u) => console.log('  ' + u));
  process.exitCode = 1;
}
if (srcDiff && CHECK) process.exitCode = 1;
w.done();
