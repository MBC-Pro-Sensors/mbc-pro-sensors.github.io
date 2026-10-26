// Arduino I2C v2 片段：單檔小幫手寫法 mbcRead(位址, 編號)／mbcReadMany／mbcWrite（I2C欄位定案 §1、§6）
import { L, T, hex, valueName } from '../../gen-lib.mjs';

const CODE = 'cpp';

export function available(product) { return !!(product.hosts && product.hosts.i2c); }

// 讀一個值
export function value(product, v, lang) {
  if (!v.i2c) return null;
  const name = valueName(v, lang);
  const unit = v.unit ? ` ${v.unit}` : '';
  if (v.count) {
    const code = `int16_t ${v.key}[${v.count}];\n` +
      `mbcReadMany(${product.addr}, ${hex(v.id)}, ${v.count}, ${v.key});   // ${name}：${v.range}${unit}\n` +
      `for (uint8_t i = 0; i < ${v.count}; i++) { Serial.print(${v.key}[i]); Serial.print(' '); }\n` +
      `Serial.println();`;
    return { code, codeLang: CODE, note: T(lang, `編號 ${valueIdRangeText(v)} 連號，一次 \`mbcReadMany\` 讀完保證同一瞬間。`, `Ids ${valueIdRangeText(v)} are consecutive: one \`mbcReadMany\` call returns them from the same 10 ms snapshot.`) };
  }
  const code = `int16_t ${v.key} = mbcRead(${product.addr}, ${hex(v.id)});   // ${name}：${v.range}${unit}`;
  // 用滿 16 位元的值（0~65535、−32768~32767）本身就可能等於 −32768，例如掌控者只按 ■ 時十六鍵 = 0x8000
  if (fullRange(v)) return { code, codeLang: CODE, note: T(lang,
    `這個值本身就可能是 −32768（最高位元 = 1，例如 \`0x8000\`），跟讀不到時回的 \`MBC_NONE\` 一樣，不要拿 −32768 判斷有沒有讀到：要分辨就改用 \`mbcReadMany(${product.addr}, ${hex(v.id)}, 1, &${v.key})\`，回傳 \`false\` 才是讀不到。`,
    `This value can itself be −32768 (top bit set, e.g. \`0x8000\`), the same as \`MBC_NONE\` for no answer, so do not test for −32768: use \`mbcReadMany(${product.addr}, ${hex(v.id)}, 1, &${v.key})\` instead; only a \`false\` return means no answer.`) };
  return { code, codeLang: CODE, note: T(lang, '讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。', 'Returns `MBC_NONE` (−32768) when the module does not answer.') };
}
function fullRange(v) {
  const m = String(v.range || '').match(/^(-?\d+)~\+?(-?\d+)$/);
  return !!m && (Number(m[1]) <= -32768 || Number(m[2]) >= 32768);
}

function valueIdRangeText(v) {
  const base = parseInt(v.id, 16);
  return `${hex(v.id)}~${hex((base + v.count - 1).toString(16))}`;
}

// 寫入命令
export function command(product, c, lang) {
  if (!c.i2c) return null;
  const args = (c.params || []).map((p) => `${L(p.name, lang)}${p.range ? ` ${p.range}` : ''}`);
  const sample = (c.params || []).map((p, i) => sampleArg(p, i));
  // 2 個參數、第 1 個是 0：字面 0 也能當空指標，會跟陣列版 mbcWrite 撞名，要轉型
  if (sample.length === 2 && Number(sample[0]) === 0) sample[0] = `(uint8_t)${sample[0]}`;
  let code = `mbcWrite(${product.addr}, ${hex(c.id)}${sample.length ? ', ' + sample.join(', ') : ''});   // ${L(c.name, lang)}${args.length ? `（${args.join(T(lang, '、', ', '))}）` : ''}`;
  // 「…開始」有對應的「…結束」命令（IMU 校準 E1／E2、E3／E4）：旗標要等送了結束命令才會清掉，不能在這裡等它，會卡死
  const end = /Start$/.test(c.key || '') && (product.commands || []).find((x) => x.key === c.key.replace(/Start$/, 'End'));
  if (c.busyFlag != null && end) {
    code += `\n// ${T(lang, `校準中 F3 bit${c.busyFlag} = 1。照產品說明做完動作後送「${L(end.name, lang)}」：mbcWrite(${product.addr}, ${hex(end.id)}); bit${c.busyFlag} 才會變 0`, `F3 bit${c.busyFlag} = 1 while calibrating. Do the moves in the product guide, then send "${L(end.name, lang)}": mbcWrite(${product.addr}, ${hex(end.id)}); only then bit${c.busyFlag} clears`)}`;
  } else if (c.busyFlag != null) {
    code += `\nwhile (mbcRead(${product.addr}, 0xF3) & (1 << ${c.busyFlag})) delay(10);   // ${T(lang, `等 F3 bit${c.busyFlag} 歸零 = 做完`, `wait until F3 bit${c.busyFlag} clears = done`)}`;
  }
  return { code, codeLang: CODE, note: '' };
}
function sampleArg(p, i) {
  const m = String(p.range || '').match(/^(-?\d+)~(-?\d+)$/);
  if (m) return Math.round((Number(m[1]) + Number(m[2])) / 2);
  if (/^0x[0-9A-Fa-f]+$/.test(p.range || '')) return p.range;
  return [0x20, 0x40, 0x60][i] || 0;
}

// 狀態旗標
export function status(product, lang) {
  const bits = (product.flags || []).map((f) => `bool ${f.key} = st & (1 << ${f.bit});   // ${L(f.name, lang)}`).join('\n');
  return { code: `int16_t st = mbcRead(${product.addr}, 0xF3);\n${bits}`, codeLang: CODE, note: '' };
}

// 單檔小幫手。前 3 行（#include、空行、MBC_NONE）要放程式最上面，其餘貼最下面：Arduino 只會自動往前宣告函式，
// Wire 和 MBC_NONE 放最下面的話 setup() 用到時還沒宣告，會編不過。網頁用 helpersTop()／helpersBottom() 分兩段給；
// helpers() 是完整原文（learn/ 的 Arduino 互動教學直接讀它並檢查前 3 行格式，這 3 行的寫法不要改）。
export function helpersTop(product, lang) { return helpers(product, lang).split('\n').slice(0, 3).join('\n'); }
export function helpersBottom(product, lang) { return helpers(product, lang).split('\n').slice(3).join('\n').replace(/^\n+/, ''); }
// 網頁用：兩段各一個程式框（Markdown）
export function helperParts(product, lang) {
  const f = (code) => '```cpp\n' + code.replace(/\s+$/, '') + '\n```';
  return [
    T(lang, '**① 放在程式最上面**（`setup()` 前面）', '**① At the top of your sketch** (before `setup()`)'), '', f(helpersTop(product, lang)), '',
    T(lang, '**② 貼在程式最下面**（`loop()` 後面）', '**② At the bottom of your sketch** (after `loop()`)'), '', f(helpersBottom(product, lang)),
  ].join('\n');
}
export function helpers(product, lang) {
  return `#include <Wire.h>

const int16_t MBC_NONE = -32768;   // ${T(lang, '讀不到／空號', 'no answer / undefined id')}

// ${T(lang, '從編號 id 起連續讀 k 個值（k ≤ 16），int16 小端', 'read k consecutive values (k ≤ 16) starting at id, int16 little-endian')}
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  if (Wire.endTransmission() != 0) return false;
  delay(2);                                        // ${T(lang, '一律等 2 ms（ESP32 產品不能時脈延展）', 'always wait 2 ms (ESP32 products cannot clock-stretch)')}
  if (Wire.requestFrom(addr, (uint8_t)(k * 2)) != (uint8_t)(k * 2)) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint8_t lo = Wire.read(), hi = Wire.read();
    out[i] = (int16_t)(lo | (hi << 8));
  }
  return true;
}

// ${T(lang, '讀 1 個值；讀不到回 MBC_NONE', 'read one value; MBC_NONE when nothing answers')}
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// ${T(lang, '寫入命令（0~3 個參數）', 'write a command (0~3 parameters)')}
// ${T(lang, '2 個參數、第 1 個是 0 時要寫 (uint8_t)0，例如 mbcWrite(ADDR, 0xF8, (uint8_t)0x00, 0xFF)，不然會跟下面陣列版的 mbcWrite 分不清而編不過', 'with 2 parameters whose first is 0, write (uint8_t)0, e.g. mbcWrite(ADDR, 0xF8, (uint8_t)0x00, 0xFF); otherwise it clashes with the array form below and will not compile')}
void mbcWrite(uint8_t addr, uint8_t id, const uint8_t *args, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(args[i]);
  Wire.endTransmission();
}
void mbcWrite(uint8_t addr, uint8_t id) { mbcWrite(addr, id, nullptr, 0); }
void mbcWrite(uint8_t addr, uint8_t id, uint8_t a) { mbcWrite(addr, id, &a, 1); }
void mbcWrite(uint8_t addr, uint8_t id, uint8_t a, uint8_t b) { uint8_t p[2] = {a, b}; mbcWrite(addr, id, p, 2); }
void mbcWrite(uint8_t addr, uint8_t id, uint8_t a, uint8_t b, uint8_t c) { uint8_t p[3] = {a, b, c}; mbcWrite(addr, id, p, 3); }

// ${T(lang, '等模組就緒（F3 bit0 = 1），最多 ms 毫秒', 'wait until ready (F3 bit0 = 1), at most ms milliseconds')}
bool mbcWaitReady(uint8_t addr, unsigned long ms) {
  unsigned long t0 = millis();
  while (millis() - t0 < ms) {
    int16_t st = mbcRead(addr, 0xF3);
    if (st != MBC_NONE && (st & 1)) return true;
    delay(20);
  }
  return false;
}`;
}

// 共用暫存器（F0~F5、F8、EF）
export function commonValue(common, cv, lang) {
  return { code: `int16_t ${cv.key} = mbcRead(ADDR, ${hex(cv.id)});   // ${L(cv.name, lang)}`, codeLang: CODE, note: '' };
}
export function commonCommand(common, cc, lang) {
  if (cc.id === 'F8') return { code: `mbcWrite(ADDR, 0xF8, 0x20, (uint8_t)~0x20);   // ${T(lang, '換到 0x20，重開機後生效', 'move to 0x20, effective after reboot')}\nmbcWrite(ADDR, 0xF8, (uint8_t)0x00, 0xFF);  // ${T(lang, '回出廠位址', 'back to the factory address')}`, codeLang: CODE, note: '' };
  if (cc.id === 'EF') return { code: `mbcWrite(ADDR, 0xEF, 0xA5);   // ${T(lang, '重新開機', 'reboot')}`, codeLang: CODE, note: '' };
  return { code: `mbcWrite(ADDR, ${hex(cc.id)});`, codeLang: CODE, note: '' };
}

// ---------------------------------------------------------------- 依軟體（gen-guides.mjs 軟體頁用；這個平台只有 ide）
export function bySoftware(sw, product, v, lang) { return sw === 'ide' ? value(product, v, lang) : null; }
export function commandBySoftware(sw, product, c, lang) { return sw === 'ide' ? command(product, c, lang) : null; }
