// EV3 片段：Pybricks v4 LUMPDevice.read(mode)[index] ＋ clev3r Sensor.ReadRawValue（EV3欄位定案）
import { L, T, valueName } from '../../gen-lib.mjs';

export function available(product) { return !!(product.hosts && product.hosts.ev3); }

const PY = 'python';

function pyHead(lang) {
  return `from pybricks.iodevices import LUMPDevice\nfrom pybricks.parameters import Port\n\nsensor = LUMPDevice(Port.S1)   # ${T(lang, '感應器插 1 號孔', 'sensor on port 1')}`;
}

export function value(product, v, lang) {
  if (!v.ev3 || !v.ev3.length) {
    return { text: T(lang, 'EV3 模式沒有送這個值（EV3 封包以已鎖住的模式為準）。需要時改用 Arduino I2C／mBlock。', 'Not sent in any EV3 mode (the locked EV3 modes stay as they are). Use Arduino I2C / mBlock if you need it.') };
  }
  const name = valueName(v, lang);
  const parts = v.ev3.map((e) => {
    const off = e.offset ? ` - ${e.offset}` : '';
    let py, basic;
    if (e.count) {
      py = `${v.key} = sensor.read(${e.mode})[${e.index}:${e.index + e.count}]${off ? '   # ' + T(lang, `每筆減 ${e.offset}`, `subtract ${e.offset} from each`) : ''}   # ${name}`;
      basic = `Sensor.SetMode(1, ${e.mode})\n${v.key}1 = Sensor.ReadRawValue(1, ${e.index})   ' ${T(lang, '第 1 筆；第 N 筆改成', 'first; for the N-th use')} ${e.index} + N - 1`;
    } else {
      py = `${v.key} = sensor.read(${e.mode})[${e.index}]${off}   # ${name}：${v.range}${v.unit ? ' ' + v.unit : ''}`;
      basic = `Sensor.SetMode(1, ${e.mode})\n${v.key} = Sensor.ReadRawValue(1, ${e.index})${off}`;
    }
    const noteParts = [T(lang, `模式 ${e.mode}`, `mode ${e.mode}`) + (e.format ? ` · ${e.format}` : '') + (e.note ? ` · ${e.note}` : '')];
    return { py, basic, note: noteParts.join('') };
  });
  const code = `${pyHead(lang)}\n\n${parts.map((p) => p.py).join('\n')}`;
  const basic = `' clev3r (BASIC)\n${parts.map((p) => p.basic).join('\n')}`;
  const locked = (product.lego && product.lego.ev3Locked) || [];
  const notes = parts.map((p) => p.note).join(T(lang, '；', '; ')) +
    (v.ev3.some((e) => locked.includes(e.mode)) ? T(lang, '（已鎖住的模式，韌體與主機端都不再改）', ' (locked mode: firmware and hosts will not change it)') : '');
  return { code, codeLang: PY, extra: [{ code: basic, codeLang: 'basic' }], note: notes };
}

export function command(product, c, lang) {
  return { text: T(lang, 'EV3 接頭沒有寫入命令；校準用感應器上的按鈕，設定用 Arduino I2C／mBlock。', 'The EV3 port carries no write commands: calibrate with the button on the sensor, change settings over Arduino I2C / mBlock.') };
}

export function status(product, lang) {
  return { code: `${pyHead(lang)}\n\nid_word, fw = sensor.read(5)   # ${T(lang, '模式 5：[ID | hw0<<8, 主版×100+次版]', 'mode 5: [ID | hw0<<8, major×100+minor]')}\naddr = id_word & 0xFF             # ${product.addr}\nhw0 = id_word >> 8                # ${product.hw0}`, codeLang: PY, note: T(lang, 'EV3 沒有 F3 旗標；模式 5 回裝置 ID 與韌體版本（ID 格式 B）。', 'EV3 has no F3 flags; mode 5 returns the device ID and firmware version (ID format B).') };
}

// ---------------------------------------------------------------- 依軟體（gen-guides.mjs 軟體頁用）
// sw = ev3g｜classroom（原廠積木：紅外線 type 33 模式 0~2、陀螺儀 type 32 模式 0／1／3）｜ pybricks｜ clev3r；讀不到回 null
function stockBlock(product, e, i, sw, lang) {
  const type = product.lego && product.lego.ev3Type;
  const en = sw === 'classroom' || lang === 'en'; // Classroom 沒有繁中介面：積木名用英文
  const n = e.index + i;
  if (type === 33) {
    if (e.mode === 0 && n === 0) return en ? 'Infrared Sensor → Proximity' : '紅外線感應器 →「近接」';
    if (e.mode === 1 && n < 8) {
      const ch = (n >> 1) + 1;
      if (n % 2 === 0) return en ? `Infrared Sensor → Beacon heading, channel ${ch}` : `紅外線感應器 →「信標」頻道 ${ch} 的方向`;
      return en ? `Infrared Sensor → Beacon proximity, channel ${ch}` : `紅外線感應器 →「信標」頻道 ${ch} 的近接`;
    }
    if (e.mode === 2 && n < 4) return en ? `Infrared Sensor → Remote, channel ${n + 1}` : `紅外線感應器 →「遙控」頻道 ${n + 1}`;
    return null;
  }
  if (type === 32) {
    if (e.mode === 0 && n === 0) return en ? 'Gyro Sensor → Angle' : '陀螺儀 →「角度」';
    if (e.mode === 1 && n === 0) return en ? 'Gyro Sensor → Rate' : '陀螺儀 →「速率」';
    if (e.mode === 3 && n < 2) return n === 0 ? (en ? 'Gyro Sensor → Angle and Rate (angle)' : '陀螺儀 →「角度與速率」的角度') : (en ? 'Gyro Sensor → Angle and Rate (rate)' : '陀螺儀 →「角度與速率」的速率');
    return null;
  }
  return undefined; // 不是模擬樂高原廠感應器：要用 MBC 自訂積木
}
export function bySoftware(sw, product, v, lang) {
  const list = v.ev3 || [];
  if (!list.length) return null;
  if (sw === 'pybricks' || sw === 'clev3r') {
    const r = value(product, v, lang);
    return sw === 'pybricks' ? { code: r.code, codeLang: r.codeLang, note: r.note } : { code: r.extra[0].code, codeLang: 'basic', note: r.note };
  }
  if (sw === 'ev3g' || sw === 'classroom') {
    const rows = [];
    let custom = false;
    for (const e of list) {
      const n = e.count || 1;
      for (let i = 0; i < n; i++) {
        const b = stockBlock(product, e, i, sw, lang);
        if (b === undefined) { custom = true; continue; }
        if (!b) continue;
        rows.push([n > 1 ? T(lang, `第 ${i + 1} 個`, `#${i + 1}`) : valueName(v, lang), b, e.offset ? T(lang, `讀到的數 − ${e.offset}`, `value − ${e.offset}`) : T(lang, '讀到的數', 'the value'), T(lang, `模式 ${e.mode}`, `mode ${e.mode}`)]);
      }
      if (rows.length) break; // 第一個讀得到的模式就夠（換模式要等 0.5 秒）
    }
    if (custom && !rows.length) return { text: T(lang, '這款不是模擬樂高原廠感應器，原廠積木讀不到；請用範例裡附的 MBC 自訂積木（見上方安裝與設定）。', 'This product does not emulate a stock LEGO sensor, so stock blocks cannot read it; use the MBC custom block included with the examples (see setup above).') };
    if (!rows.length) return null;
    const head = `| ${T(lang, '值', 'Value')} | ${T(lang, '積木', 'Block')} | ${T(lang, '換算', 'Decode')} | ${T(lang, '模式', 'Mode')} |\n| :--- | :--- | :--- | :--- |\n`;
    return { html: head + rows.map((r) => `| ${r.join(' | ')} |`).join('\n') };
  }
  return null;
}
export function commandBySoftware() { return null; } // EV3 接頭沒有寫入命令
