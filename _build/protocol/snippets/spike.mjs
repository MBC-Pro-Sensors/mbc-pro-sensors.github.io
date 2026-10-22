// SPIKE 片段：Pybricks PUPDevice.read(mode) 原生讀法＋ MBC 函式庫方法（SPIKE欄位定案）
import { L, T, valueName } from '../../gen-lib.mjs';

export function available(product) { return !!(product.hosts && product.hosts.spike); }

const PY = 'python';

// 各產品 Pybricks 函式庫：模組名、類別名、值 key → 方法（沒有的值就只給原生讀法）
export const LIB = {
  line8: { module: 'MBC_line8_obj_Lib_v400', cls: 'MBC_LINE8', obj: 'line', methods: { linePos: 'pos8()', linePosFine: 'pos100()', lineWidth: 'width()', groupCount: 'junction()', bitmap: 'bin_raw()', light: 'ir_calibration()' }, file: '/examples/line8/pybricks/For%20firmware%204.0.0/MBC_line8_obj_Lib_v400.py' },
  line16: { module: 'MBC_line16_obj_Lib_v400', cls: 'MBC_LINE16', obj: 'line', methods: { linePos: 'pos16()', linePosFine: 'pos100()', lineWidth: 'width()', groupCount: 'junction()', bitmap: 'bin_raw()', light: 'ir_calibration()' }, file: '/examples/line16/pybricks/For%20firmware%204.0.0/MBC_line16_obj_Lib_v400.py' },
  // 2026-10-22 H7 定稿（函式庫 v2 新方法：模式 3／6／8）
  tof2: { module: 'MBC_laser2_obj_Lib', cls: 'MBC_LASER2', obj: 'laser', methods: { leftCm: 'left()', rightCm: 'right()', nearCm: 'min_distance()', leftMm: 'left_mm()', rightMm: 'right_mm()', avgMm: 'average_mm()', initOk: 'is_ok(1)', fault: 'is_ok(1)' }, file: '/examples/tof2/pybricks/For%20firmware%204.0.0/MBC_laser2_obj_Lib.py' },
  tof8: { module: 'MBC_laser8_obj_Lib', cls: 'MBC_LASER8', obj: 'laser', methods: { nearCm: 'min_distance()', nearDir: 'min_direction()', farCm: 'max_distance()', farDir: 'max_direction()', nearMm: 'nearest_mm()', frontMm: 'front_mm()', frontCm: 'front_mm() // 10', distCm: 'distances()', initOk: 'is_ok(1)', fault: 'is_ok(1)' }, file: '/examples/tof8/pybricks/For%20firmware%204.0.0/MBC_laser8_obj_Lib.py' },
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

export function command(product, c, lang) {
  return { text: T(lang, 'SPIKE 接頭沒有寫入命令；校準用感應器上的按鈕，設定用 Arduino I2C／mBlock。', 'The SPIKE port carries no write commands: calibrate with the button on the sensor, change settings over Arduino I2C / mBlock.') };
}

export function status(product, lang) {
  return { code: `${head(lang)}\n\nid_word, proto, fw, seq = dev.read(8)   # ${T(lang, '模式 8：[ID | hw0<<8, 協議版 2, 主版×100+次版, 更新序號]', 'mode 8: [ID | hw0<<8, protocol 2, major×100+minor, counter]')}\naddr = id_word & 0xFF   # ${product.addr}\nhw0 = id_word >> 8      # ${product.hw0}`, codeLang: PY, note: T(lang, 'SPIKE 沒有 F3 旗標；模式 8 回裝置 ID／版本。官方 App 讀不到模式 8。', 'SPIKE has no F3 flags; mode 8 returns ID / version. The official app cannot read mode 8.') };
}
