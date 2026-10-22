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
