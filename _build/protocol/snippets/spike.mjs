// SPIKE 片段：Pybricks PUPDevice.read(mode) 原生讀法＋ MBC 函式庫方法（SPIKE欄位定案）
import { L, T, valueName } from '../../gen-lib.mjs';

export function available(product) { return !!(product.hosts && product.hosts.spike); }

const PY = 'python';

// 各產品 Pybricks 函式庫：模組名、類別名、值 key → 方法（沒有的值就只給原生讀法）
export const LIB = {
  line8: { module: 'MBC_line8_obj_Lib', cls: 'MBC_LINE8', obj: 'line', methods: { linePos: 'pos8()', linePosFine: 'pos100()', lineWidth: 'width()', groupCount: 'junction()', bitmap: 'bin_raw()', light: 'ir_calibration()' }, file: '/examples/line8/pybricks/For%20firmware%204.x/MBC_line8_obj_Lib.py' },
  line16: { module: 'MBC_line16_obj_Lib', cls: 'MBC_LINE16', obj: 'line', methods: { linePos: 'pos16()', linePosFine: 'pos100()', lineWidth: 'width()', groupCount: 'junction()', bitmap: 'bin_raw()', light: 'ir_calibration()' }, file: '/examples/line16/pybricks/For%20firmware%204.x/MBC_line16_obj_Lib.py' },
  // 2026-10-22 H7 定稿（函式庫 v2 新方法：模式 3／6／8）
  tof2: { module: 'MBC_laser2_obj_Lib', cls: 'MBC_LASER2', obj: 'laser', methods: { leftCm: 'left()', rightCm: 'right()', nearCm: 'min_distance()', leftMm: 'left_mm()', rightMm: 'right_mm()', avgMm: 'average_mm()', initOk: 'is_ok(1)', fault: 'is_ok(1)' }, file: '/examples/tof2/pybricks/For%20firmware%204.x/MBC_laser2_obj_Lib.py' },
  tof8: { module: 'MBC_laser8_obj_Lib', cls: 'MBC_LASER8', obj: 'laser', methods: { nearCm: 'min_distance()', nearDir: 'min_direction()', farCm: 'max_distance()', farDir: 'max_direction()', nearMm: 'nearest_mm()', frontMm: 'front_mm()', frontCm: 'front_mm() // 10', distCm: 'distances()', initOk: 'is_ok(1)', fault: 'is_ok(1)' }, file: '/examples/tof8/pybricks/For%20firmware%204.x/MBC_laser8_obj_Lib.py' },
  ps2: { module: 'MBC_gamepad_Lib', cls: 'MBC_GAMEPAD', obj: 'pad', pre: 'pad.update()', methods: { buttons16: 'buttons_psx()', dpad: "pressed('UP')", shape: "pressed('TRIANGLE')", shoulder: "pressed('L1')", func: "pressed('START')", connected: 'connected()', battery: 'battery()', padType: 'kind()', stick: "stick('LX')" }, file: '/examples/ps2/pybricks/MBC_gamepad_Lib.py' },
  ps4: { module: 'MBC_gamepad_Lib', cls: 'MBC_GAMEPAD', obj: 'pad', pre: 'pad.update()', methods: { buttons16: 'buttons_psx()', dpad: "pressed('UP')", shapeKeys: "pressed('TRIANGLE')", shoulderKeys: "pressed('L1')", funcKeys: "pressed('START')", connected: 'connected()', battery: 'battery()', padType: 'kind()', stick: "stick('LX')" }, file: '/examples/ps4/pybricks/MBC_gamepad_Lib.py' },
  cam_school: { module: 'MBC_cam_school_Lib', cls: 'MBC_CAM_SCHOOL', obj: 'cam', methods: { hue: 'hue()', saturation: 'saturation()', value: 'brightness()', motion: 'motion()', blobX: 'blob_x()', blobY: 'blob_y()', blobW: 'blob_w()', blobH: 'blob_h()', blobArea: 'area()', found: 'found()' }, file: '/examples/esp32cam/pybricks/MBC_cam_school_Lib.py' },
  cam_wro: { module: 'MBC_hue12_Lib', cls: 'HUE12Sensor', obj: 'cam', pre: 'cam.update()', methods: { spotColor: 'get_slot(1)[1]', spotVivid: 'get_slot(1)[2]' }, file: '/examples/esp32cam/pybricks/MBC_hue12_Lib.py' },
};

function head(lang) {
  return `from pybricks.iodevices import PUPDevice\nfrom pybricks.parameters import Port\n\ndev = PUPDevice(Port.A)   # ${T(lang, '感應器插 A 孔', 'sensor on port A')}`;
}

function nativeLine(v, e, lang) {
  const off = e.offset ? ` - ${e.offset}` : '';
  const name = valueName(v, lang);
  if (e.v != null && e.part) {
    const w = `w = dev.read(${e.mode})[${e.v}]`;
    const pick = e.part === 'lo' ? '(w & 0xFF)' : '((w >> 8) & 0xFF)';
    return `${w}\n${v.key} = ${pick}${off}   # ${name}（${T(lang, e.part === 'lo' ? '低 byte' : '高 byte', e.part + ' byte')}）`;
  }
  if (e.v != null && e.count) {
    return `words = dev.read(${e.mode})[${e.v}:${e.v + e.count}]\n${v.key} = []\nfor w in words:\n    ${v.key}.append(w & 0xFF)          # ${T(lang, '奇數路', 'odd channel')}\n    ${v.key}.append((w >> 8) & 0xFF)   # ${T(lang, '偶數路', 'even channel')}`;
  }
  if (e.v != null) return `${v.key} = dev.read(${e.mode})[${e.v}]${off}   # ${name}`;
  if (e.byte != null && /nibble/.test(e.format || '')) return `raw = dev.read(${e.mode})[${e.byte}:${e.byte + 4}]   # ${name}：${T(lang, '每 byte 兩路 4-bit', 'two 4-bit channels per byte')}`;
  if (e.byte != null) return `${v.key} = dev.read(${e.mode})[${e.byte}]${off}   # ${name}`;
  return `${v.key} = dev.read(${e.mode})   # ${name}`;
}

export function value(product, v, lang) {
  if (!v.spike || !v.spike.length) {
    return { text: T(lang, 'SPIKE 模式沒有送這個值（已鎖住的模式不再改）。需要時改用 Arduino I2C／mBlock。', 'Not sent in any SPIKE mode (locked modes stay as they are). Use Arduino I2C / mBlock if you need it.') };
  }
  const lib = LIB[product.id];
  const m = lib && lib.methods[v.key];
  // 原生讀法：優先模式 9（整包）其次第一筆
  const pref = v.spike.find((e) => e.mode === 9) || v.spike[0];
  const code = `${head(lang)}\n\n${nativeLine(v, pref, lang)}`;
  const extra = [];
  if (m) {
    extra.push({ title: T(lang, 'MBC 函式庫', 'MBC library'), code: `from pybricks.parameters import Port\nfrom ${lib.module} import ${lib.cls}\n\n${lib.obj} = ${lib.cls}(Port.A)\n${lib.pre ? lib.pre + '   # ' + T(lang, '每圈先更新一次', 'refresh once per loop') + '\n' : ''}${v.key} = ${lib.obj}.${m}${v.count ? '   # ' + T(lang, `list，第 1 路 = [0]`, 'list, channel 1 = [0]') : ''}`, codeLang: PY });
  }
  const locked = (product.lego && product.lego.spikeLocked) || [];
  const where = v.spike.map((e) => `${T(lang, '模式', 'mode')} ${e.mode}${e.hub ? ` (${e.hub})` : ''}${e.v != null ? ` v[${e.v}]${e.part ? ' ' + e.part : ''}` : e.byte != null ? ` byte ${e.byte}` : ''}${e.offset ? ` −${e.offset}` : ''}${locked.includes(e.mode) ? ' 🔒' : ''}`).join(T(lang, '；', '; '));
  return { code, codeLang: PY, extra, note: where + (v.spike.some((e) => e.note) ? T(lang, '。備註：', '. Note: ') + v.spike.filter((e) => e.note).map((e) => e.note).join('; ') : '') };
}

// SPIKE 命令通道（方案丙，common.spikeCmd）：產品 lego.spikeCmd = true、命令 spike = true 才能送，用 MBC 函式庫 cmd()＋wait_done()
// wait_done() 預設最多等 10 秒；比較久的命令另外給（循行者 E7 掃 5 秒、PS4 重新配對最多 30 秒）
const WAIT_MS = { calibAuto: 8000, rePair: 35000 };
export const canCmd = (product, c) => !!(product.lego && product.lego.spikeCmd && c.spike);
function sampleArg(p) {
  const m = String(p.range || '').match(/^(-?\d+)~(-?\d+)$/);
  return m ? Math.round((Number(m[1]) + Number(m[2])) / 2) : 0;
}
function cmdCode(product, c, lang) {
  const lib = LIB[product.id];
  const args = (c.params || []).map((p) => `${L(p.name, lang)}${p.range ? ` ${p.range}` : ''}`);
  const sample = (c.params || []).map(sampleArg);
  const ms = WAIT_MS[c.key];
  return `from pybricks.parameters import Port
from MBC_sensors import ${lib.cls}

${lib.obj} = ${lib.cls}(Port.A)
` +
    `${lib.obj}.cmd(0x${c.id}${sample.length ? ', ' + sample.join(', ') : ''})   # ${L(c.name, lang)}${args.length ? `（${args.join(T(lang, '、', ', '))}）` : ''}
` +
    `ok = ${lib.obj}.wait_done(${ms || ''})   # ${T(lang, 'True＝做完；False＝失敗／拒絕／逾時', 'True = done; False = failed / rejected / timed out')}`;
}
export const CMD_INTRO = (lang) => T(lang, [
  '下命令要用 **MBC 函式庫**（2026-10 以後的版本）和 **2026-10 以後的感應器韌體**：`cmd(命令碼, 參數…)` 送出，`wait_done()` 等它做完（回 True＝完成；False＝失敗、拒絕或逾時）。感應器回的狀態：0 完成、1 執行中、2 失敗（例如參數超出範圍）、3 拒絕（不認得的命令，或校準中又送別的命令）。',
  '',
  '- 舊韌體呼叫 `cmd()` 會顯示「韌體太舊」，讀值照常。',
  '- 同一個命令＋同樣參數再呼叫不會重做（放在迴圈裡也只做一次），要再做一次寫 `cmd(…, again=True)`。',
  '- `wait_done()` 每次多 40～60 ms，只在校準這類「停下來做」的命令用，不要放在循線迴圈裡。',
  '- 官方 SPIKE App 3、Pybricks 內建 ColorSensor 積木送不了命令。',
].join('\n'), [
  'Commands need the **MBC library** (2026-10 or later) and **sensor firmware from 2026-10 or later**: send with `cmd(code, params…)`, then `wait_done()` waits for it (True = done; False = failed, rejected or timed out). Sensor states: 0 done, 1 busy, 2 failed (e.g. parameter out of range), 3 rejected (unknown command, or another command sent during calibration).',
  '',
  '- On older firmware `cmd()` reports "firmware too old"; readings keep working.',
  '- The same command with the same parameters is not repeated (even inside a loop); pass `cmd(…, again=True)` to run it again.',
  '- `wait_done()` costs 40–60 ms each time: use it for stop-and-do commands such as calibration, not inside a line-following loop.',
  "- The official SPIKE App 3 and Pybricks' built-in ColorSensor blocks cannot send commands.",
].join('\n'));
export function command(product, c, lang) {
  if (canCmd(product, c) && LIB[product.id]) return { code: cmdCode(product, c, lang), codeLang: PY, note: T(lang, 'Pybricks＋MBC 函式庫才送得了（官方 App、內建 ColorSensor 積木不行）；要 2026-10 以後的感應器韌體。', 'Pybricks + the MBC library only (not the official app or built-in ColorSensor blocks); needs sensor firmware from 2026-10 or later.') };
  return { text: T(lang, 'SPIKE 接頭沒有寫入命令；校準用感應器上的按鈕，設定用 Arduino I2C／mBlock。', 'The SPIKE port carries no write commands: calibrate with the button on the sensor, change settings over Arduino I2C / mBlock.') };
}

export function status(product, lang) {
  return { code: `${head(lang)}\n\nid_word, proto, fw, seq = dev.read(8)   # ${T(lang, '模式 8：[ID | hw0<<8, 協議版 2, 主版×100+次版, 更新序號]', 'mode 8: [ID | hw0<<8, protocol 2, major×100+minor, counter]')}\naddr = id_word & 0xFF   # ${product.addr}\nhw0 = id_word >> 8      # ${product.hw0}`, codeLang: PY, note: T(lang, 'SPIKE 沒有 F3 旗標；模式 8 回裝置 ID／版本。官方 App 讀不到模式 8。', 'SPIKE has no F3 flags; mode 8 returns ID / version. The official app cannot read mode 8.') };
}

// ---------------------------------------------------------------- 依軟體（gen-guides.mjs 軟體頁用）
// sw = official（樂高官方 App：模式 0 教育版／玩具版 byte → 顏色感應器積木）｜ mbcblocks（MBC 積木網站：模式 9）｜ pybricks
// 讀不到回 null（軟體頁會把它列進「這個軟體讀不到」一行）
const RGB = [{ zh: '原始紅', en: 'raw red' }, { zh: '原始綠', en: 'raw green' }, { zh: '原始藍', en: 'raw blue' }];
// 教育版：byte0 = 顏色、byte1 = 反射光；玩具版（Robot Inventor）兩個對調（循行者 RI 頁：顏色 = 線寬、反射光 = 位置）
function officialBlock(hub, b, lang) {
  if (b === 0) return hub === 'toy' ? T(lang, '反射光', 'reflected light') : T(lang, '顏色', 'color');
  if (b === 1) return hub === 'toy' ? T(lang, '顏色', 'color') : T(lang, '反射光', 'reflected light');
  if (b >= 2 && b <= 7) return L(RGB[(b - 2) >> 1], lang);
  return null;
}
function officialRows(e, v, lang) {
  const f = e.format || 'u8';
  const wide = /16/.test(f) && !/查表/.test(f);
  const step = e.count && wide ? 2 : 1;
  const n = e.count || 1;
  const rows = [];
  for (let i = 0; i < n; i++) {
    const b = e.byte + i * step;
    const blk = officialBlock(e.hub, b, lang);
    if (!blk) return null; // byte 8 以後官方 App 讀不到
    let how;
    if (b <= 1 || (wide && b % 2 === 0)) how = T(lang, '讀到的數', 'the number it returns');
    else if (/nibble/.test(f)) how = T(lang, '讀到的數：每個數字含 4 路、每路 4 bit', 'each number packs 4 channels of 4 bits');
    else if (b % 2 === 0) how = T(lang, '讀到的數 ÷ 256 取餘數', 'the number mod 256');
    else how = T(lang, '讀到的數 ÷ 256 取整數（無條件捨去）', 'floor(the number ÷ 256)');
    if (e.offset) how += T(lang, `，再減 ${e.offset}`, `, minus ${e.offset}`);
    if (/查表/.test(f)) how += T(lang, '（玩具版查表編碼，解法見上方安裝與設定）', ' (Robot Inventor look-up encoding, see setup above)');
    const label = n > 1 ? T(lang, `第 ${i + 1} 組`, `group ${i + 1}`) : valueName(v, lang);
    rows.push([e.hub === 'toy' ? T(lang, 'Robot Inventor（玩具版）', 'Robot Inventor') : T(lang, 'SPIKE Prime（教育版）', 'SPIKE Prime'), label, `「${blk}」`, how]);
  }
  return rows;
}
export function bySoftware(sw, product, v, lang) {
  const list = v.spike || [];
  if (!list.length) return null;
  if (sw === 'pybricks') return value(product, v, lang);
  if (sw === 'official') {
    const rows = [];
    for (const e of list.filter((x) => x.mode === 0 && x.hub)) {
      const r = officialRows(e, v, lang);
      if (r) rows.push(...r);
    }
    if (!rows.length) return null;
    const head = `| ${T(lang, '主機', 'Hub')} | ${T(lang, '值', 'Value')} | ${T(lang, '積木（更多感應器 → 顏色感應器）', 'Block (More Sensors → Color Sensor)')} | ${T(lang, '換算', 'Decode')} |\n| :--- | :--- | :--- | :--- |\n`;
    const notes = list.filter((x) => x.mode === 0 && x.note).map((x) => x.note);
    return { html: head + rows.map((r) => `| ${r.join(' | ')} |`).join('\n'), note: notes.length ? T(lang, '韌體備註：', 'Firmware note: ') + notes.join(T(lang, '；', '; ')) : '' };
  }
  if (sw === 'mbcblocks') {
    const e = list.find((x) => x.mode === 9);
    if (!e) return null;
    return {
      text: T(lang, `在 MBC 積木網站加入這款感應器的積木，用「${valueName(v, lang)}」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：`, `Add this sensor's blocks on the MBC Blocks website and use the "${valueName(v, lang)}" block; on the hub it runs this Pybricks code (mode 9):`),
      code: `${head(lang)}\n\n${nativeLine(v, e, lang)}`, codeLang: PY,
    };
  }
  return null;
}
// Pybricks 用 MBC 函式庫 cmd()；官方 App、MBC 積木網站（命令積木還沒部署）送不了
export function commandBySoftware(sw, product, c, lang) { return sw === 'pybricks' && canCmd(product, c) && LIB[product.id] ? command(product, c, lang) : null; }
