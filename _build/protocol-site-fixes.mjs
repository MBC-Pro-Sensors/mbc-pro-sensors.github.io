// 網站副本的讀法修正（sync-protocol.mjs 複製時套用；正本 85_共用資源\MBC_LegoUART\protocol\*.json 不動）
//
// 2026-11-07 掌控者 PS2／PS4 讀法（使用者定案：標準＝現行韌體，韌體與指令說明 PDF 不改，以網站為準；
//   清單 06_遙控器\按鍵順序_各上位機檢查_20261107.md 第 3～5 項）。依據：韌體
//   06_遙控器\01_PS2遙控接收器\01_韌體\MBC_PS2_stm32f030_PB6PB7\MBC_PS2_stm32f030_PB6PB7.ino（SPIKE 模式 0～9、EV3 模式 0～5）、
//   06_遙控器\02_PS4藍牙遙控\01_韌體\MBC_PS4_ESP32\MBC_PS4_ESP32.ino（同一套排法）。
//   同樣的內容已交辦「共用資源」補進正本（00_agent交辦\共用資源.md 2026-11-07）。正本改好之後這裡每一條都會變成「已經是這樣」，
//   sync-protocol --check 照樣一致；確認正本已有這些欄位後，可以把這個檔清空。
//
// 新欄位（產生器 snippets\ev3.mjs、spike.mjs 看得懂）：
//   bits: [shift, width]   這個值在讀到的那個數裡的位元（例：EV3 模式 3 方向鍵 [0,4]、形狀鍵 [4,4]、已連線 [8,1]）
//   stride: 2              count 個值隔一格放（EV3 模式 1 搖桿在 index 1、3、5、7；中間是按鍵）
//   app: 'lo4'｜'hi4'｜'x10'｜'keyNo'｜'raw'   官方 App 積木讀到的數怎麼拆（÷16 餘數／÷16 取整數／÷10 拆兩軸／單鍵編號／直接用）
//   neg: '■'｜'START'      App 讀成有號數：按住這個鍵時讀到負數（先加 256；raw 16 位元加 65536）

const val = (d, key) => (d.values || []).find((v) => v.key === key);
const ents = (v, host, pred) => ((v && v[host]) || []).filter(pred);
const set = (list, fields) => { for (const e of list) Object.assign(e, fields); return list.length; };
const hasEnt = (v, host, pred) => ents(v, host, pred).length > 0;
const m0 = (hub, byte) => (e) => e.mode === 0 && e.hub === hub && (byte == null || e.byte === byte);
const mode = (m) => (e) => e.mode === m;

const KEYNO_NOTE = '單鍵編號（L2=1 R2=2 L1=3 R1=4 SELECT=5 L3=6 R3=7 START=8，沒按 0；同時按只回編號最小的那個）';
const KEYNO_NOTE_PS4 = '單鍵編號（L2=1 R2=2 L1=3 R1=4 SHARE=5 L3=6 R3=7 OPTIONS=8，沒按 0；同時按只回編號最小的那個）';

function gamepad(d, keys, ps4) {
  const [kD, kS, kSh, kF] = keys;
  const dpad = val(d, kD), shape = val(d, kS), sh = val(d, kSh), fn = val(d, kF);
  const conn = val(d, 'connected'), b16 = val(d, 'buttons16'), stick = val(d, 'stick');
  // EV3 模式 3：1×int16 = 方向 | 形狀<<4 | 已連線<<8
  set(ents(dpad, 'ev3', mode(3)), { bits: [0, 4] });
  set(ents(shape, 'ev3', mode(3)), { bits: [4, 4] });
  set(ents(conn, 'ev3', mode(3)), { bits: [8, 1] });
  // EV3 搖桿：模式 1 [方向, LX, 形狀, LY, 肩, RX, 功能, RY]、模式 4 [方向, LX, 形狀, LY]；都是 0~99（50 = 中間）
  set(ents(stick, 'ev3', mode(1)), { index: 1, count: 4, stride: 2 });
  set(ents(stick, 'ev3', mode(4)), { index: 1, count: 2, stride: 2 });
  for (const e of ents(stick, 'ev3', (x) => !x.scale)) e.scale = '0~99（50 = 中間）';
  // SPIKE 模式 9：v0 = 方向 | 形狀<<4、v1 = 肩 | 功能<<4、v6 = 十六鍵（int16，■ 會變負數）
  set(ents(dpad, 'spike', mode(9)), { bits: [0, 4] });
  set(ents(shape, 'spike', mode(9)), { bits: [4, 4] });
  set(ents(sh, 'spike', mode(9)), { bits: [0, 4] });
  set(ents(fn, 'spike', mode(9)), { bits: [4, 4] });
  set(ents(b16, 'spike', mode(9)), { bits: [0, 16] });
  // SPIKE 模式 4：宣告 2×int16、送 4 byte → Pybricks read(4) = (LX | LY<<8, RX | RY<<8)，各 1~9（韌體 dataBuf[4..7] = 0~99 ÷10，0 補 1）
  for (const e of ents(stick, 'spike', mode(4))) {
    delete e.byte; delete e.format;
    Object.assign(e, { v: 0, count: 2, scale: '1~9（5 = 中間）', note: 'v0 = LX | LY<<8、v1 = RX | RY<<8（各 1~9，5 = 中間）' });
  }
  // SPIKE 模式 8 早就改成裝置 ID（#67），不再送搖桿
  if (stick && stick.spike) stick.spike = stick.spike.filter((e) => !(e.mode === 8));
  // 官方 App（模式 0）
  set(ents(b16, 'spike', m0('edu', 6)), { app: 'raw', neg: '■' });
  set(ents(dpad, 'spike', m0('edu', 1)), { app: 'lo4' });
  set(ents(shape, 'spike', m0('edu', 1)), { app: 'hi4' });
  set(ents(sh, 'spike', m0('edu', 0)), { app: 'lo4', neg: ps4 ? 'OPTIONS' : 'START' });
  set(ents(fn, 'spike', m0('edu', 0)), { app: 'hi4', neg: ps4 ? 'OPTIONS' : 'START' });
  set(ents(dpad, 'spike', m0('toy', 0)), { app: 'lo4', neg: '■' });
  set(ents(shape, 'spike', m0('toy', 0)), { app: 'hi4', neg: '■' });
  // 玩具版原始藍（byte 6~7）＝ 肩 | 功能<<4（0~255）：韌體查表編碼，App 讀到的就是原數
  const blue = { mode: 0, hub: 'toy', byte: 6, format: 'u16', note: '原始藍 = 肩 | 功能<<4（0~255）' };
  for (const [v, app] of [[sh, 'lo4'], [fn, 'hi4']]) {
    if (!v) continue;
    v.spike = (v.spike || []).filter((e) => !(m0('toy', 6)(e) && /查表/.test(e.format || '')));
    if (!hasEnt(v, 'spike', m0('toy', 6))) v.spike.push({ ...blue, app });
    else set(ents(v, 'spike', m0('toy', 6)), { app });
    // 玩具版顏色（byte 1）＝ 單鍵編號 1~8
    if (!hasEnt(v, 'spike', m0('toy', 1))) v.spike.push({ mode: 0, hub: 'toy', byte: 1, format: 'u8', app: 'keyNo', note: ps4 ? KEYNO_NOTE_PS4 : KEYNO_NOTE });
    else set(ents(v, 'spike', m0('toy', 1)), { app: 'keyNo' });
  }
  // 搖桿：原始紅 = LX×10+LY、原始綠 = RX×10+RY（教育版、玩具版一樣；玩具版韌體查表編碼，App 讀到的就是原數）
  for (const hub of ['edu', 'toy']) {
    for (const e of ents(stick, 'spike', m0(hub, 2))) {
      Object.assign(e, { count: 2, format: 'u16', app: 'x10', scale: '1~9（5 = 中間）' });
      if (hub === 'toy') e.note = '原始紅 = LX×10+LY、原始綠 = RX×10+RY（各 1~9，5 = 中間）';
    }
  }
}

export const FIXES = {
  ps2(d) { gamepad(d, ['dpad', 'shape', 'shoulder', 'func'], false); },
  ps4(d) { gamepad(d, ['dpad', 'shapeKeys', 'shoulderKeys', 'funcKeys'], true); },
};

// sync-protocol 用：src 字串 → 套用後的字串（沒有修正的產品原樣回傳）
export function applyFixes(src, id) {
  if (!FIXES[id]) return src;
  const d = JSON.parse(src);
  FIXES[id](d);
  return JSON.stringify(d, null, 2) + '\n';
}
