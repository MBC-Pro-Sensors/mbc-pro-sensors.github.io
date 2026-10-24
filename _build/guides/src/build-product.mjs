// 組合產品專屬的「安裝與設定」片段 → _build/guides/product/<p>/<host>-<sw>.<lang>.md
import fs from 'fs';
import path from 'path';

const SCR = path.dirname(new URL(import.meta.url).pathname).replace(/^\/([A-Za-z]:)/, '$1');
const OUT = process.argv[2];
if (!OUT) throw new Error('usage: node build-product.mjs <repo>/_build/guides/product');

function loadFrags(lang) {
  const txt = fs.readFileSync(path.join(SCR, `frag.${lang}.md`), 'utf8').replace(/\r\n/g, '\n');
  const out = {};
  for (const part of txt.split(/^@@@ /m).slice(1)) {
    const nl = part.indexOf('\n');
    out[part.slice(0, nl).trim()] = part.slice(nl + 1).trim();
  }
  return out;
}
const FR = { zh: loadFrags('zh'), en: loadFrags('en') };

const V = {
  line8: { zh: { N: 8, NM1: 7, CHK: '2～3', IRMAX: 100, BINMAX: 255, EXON: '[4, 5]', NAME: '循行者 8 路', MODE1: '8 路校準後的反射值，每路 `0 ~ 100`（0＝最暗、100＝最亮），第 1 路在最左邊' },
           en: { N: 8, NM1: 7, CHK: '2~3', IRMAX: 100, BINMAX: 255, EXON: '[4, 5]', NAME: 'Pathfinder 8-Way', MODE1: '8 calibrated reflection values, each `0 ~ 100` (0 = darkest, 100 = brightest), channel 1 on the far left' } },
  line16: { zh: { N: 16, NM1: 15, CHK: '3～5', IRMAX: 15, BINMAX: 65535, EXON: '[7, 8, 9]', NAME: '循行者 16 路', MODE1: '16 路壓縮光值，相鄰兩路壓縮成一個兩位數（`11 ~ 99`）' },
            en: { N: 16, NM1: 15, CHK: '3~5', IRMAX: 15, BINMAX: 65535, EXON: '[7, 8, 9]', NAME: 'Pathfinder 16-Way', MODE1: '16 compressed light values, two neighbouring channels combined into one two-digit number (`11 ~ 99`)' } },
  ps2: { zh: {}, en: {} },
  ps4: { zh: { NAME: '掌控者 PS4／PS5', WHY: 'ESP32 藍牙接收，瞬間電流大' }, en: { NAME: 'Controller PS4 / PS5', WHY: 'ESP32 Bluetooth receiver, high peak current' } },
  tof8: { zh: { NAME: '測距者 8 路', WHY: '8 顆雷射同時量測，電流超過 3V 腳能給的' }, en: { NAME: 'Ranger 8-Way', WHY: '8 lasers ranging at once draw more than the 3V pin can supply' } },
  imu: { zh: {}, en: {} },
  esp32cam: { zh: { NAME: '神攝手', WHY: 'ESP32 相機＋螢幕＋補光燈' }, en: { NAME: 'Sharpshooter', WHY: 'ESP32 camera + screen + flash light' } },
  exp6: { zh: {}, en: {} },
  servo6: { zh: {}, en: {} },
};

const ALL12 = ['spike-official', 'spike-mbcblocks', 'spike-pybricks', 'ev3-ev3g', 'ev3-classroom', 'ev3-pybricks', 'ev3-clev3r',
  'arduino-ide', 'mblock-mblock5', 'matrix-matrixblock', 'matrix-bigchannel', 'microbit-makecode'];

const R = {};
for (const p of ['line8', 'line16']) {
  const ev3 = (sw, my, ex) => ['cal', 'ev3-prox', `ev3-beacon-${p}`, 'ev3-remote', my, ex, 'coach'];
  R[p] = {
    'spike-official': ['cal', `${p}-spike-official`, 'coach'],
    'spike-mbcblocks': ['cal'],
    'spike-pybricks': ['cal', 'line-spike-pybricks', 'coach'],
    'ev3-ev3g': ev3('ev3g', 'ev3-myblocks-ev3g', 'ev3-ex-ev3g'),
    'ev3-classroom': ev3('classroom', 'ev3-myblocks-classroom', 'ev3-ex-classroom'),
    'ev3-clev3r': ev3('clev3r', 'ev3-myblocks-clev3r', 'ev3-ex-clev3r'),
    'ev3-pybricks': ['cal', 'line-ev3-pybricks', 'coach'],
    'arduino-ide': ['cal', 'cal-arduino'],
    'mblock-mblock5': ['cal', 'cal-mblock', 'mblock-legacy'],
    'matrix-matrixblock': ['cal'],
    'matrix-bigchannel': ['cal', 'cal-matrixbig'],
    'microbit-makecode': ['cal', 'cal-microbit'],
  };
}
R.ps4 = Object.fromEntries(ALL12.map(sw => [sw, ['pair-ps4']]));
R.ps4['spike-official'].push('ps4-spike-official');
R.ps4['spike-pybricks'].push('pad-spike-pybricks');
R.ps4['matrix-bigchannel'].push('ps4-matrixbig');
R.ps4['microbit-makecode'].push('power5v-microbit');
R.ps2 = Object.fromEntries(ALL12.map(sw => [sw, ['pair-ps2']]));
R.ps2['spike-official'].push('ps2-spike-official');
R.ps2['spike-pybricks'].push('pad-spike-pybricks', 'ps2-pdf');
for (const sw of ['ev3-ev3g', 'ev3-classroom', 'ev3-pybricks', 'ev3-clev3r']) R.ps2[sw].push('ps2-pdf');
R.tof8 = Object.fromEntries(ALL12.map(sw => [sw, ['tof8-warm']]));
R.tof8['spike-official'].push('tof8-spike-official');
R.tof8['microbit-makecode'].push('power5v-microbit');
R.imu = {
  'ev3-ev3g': ['imu-ev3'], 'ev3-classroom': ['imu-ev3'], 'ev3-clev3r': ['imu-ev3'],
  'ev3-pybricks': ['imu-ev3-pybricks'], 'mblock-mblock5': ['imu-mblock'],
};
R.esp32cam = {
  'spike-official': ['cam-setup', 'cam-spike-official'],
  'spike-mbcblocks': ['cam-setup', ['cam-spike-pyb', { zh: { EXTRA: '' }, en: { EXTRA: '' } }]],
  'spike-pybricks': ['cam-setup', ['cam-spike-pyb', {
    zh: { EXTRA: ' Pybricks 寫法要匯入 `MBC_hue12_Lib.py`（學校版是 `MBC_cam_school_Lib.py`）。' },
    en: { EXTRA: ' With Pybricks, import `MBC_hue12_Lib.py` (school edition: `MBC_cam_school_Lib.py`).' } }]],
  'ev3-ev3g': ['cam-setup', 'cam-ev3-two', 'cam-ev3g-wro'],
  'ev3-classroom': ['cam-setup', 'cam-ev3-two', 'cam-classroom-wro'],
  'ev3-pybricks': ['cam-setup', 'cam-ev3-two'],
  'ev3-clev3r': ['cam-setup', 'cam-ev3-two'],
  'matrix-mvision': ['cam-setup-i2c', 'cam-mvision-wro'],
  'microbit-makecode': ['cam-setup-i2c', 'power5v-microbit', 'cam-microbit'],
  'arduino-ide': ['cam-setup-i2c', 'cam-arduino'],
  'mblock-mblock5': ['cam-setup-i2c', 'cam-mblock'],
};
R.exp6 = {
  'spike-official': ['exp6-power', 'exp6-official'],
  'spike-pybricks': ['exp6-power', 'exp6-pybricks'],
  'spike-mbcblocks': ['exp6-mbcblocks'],
  'arduino-ide': ['exp6-arduino'],
};
R.servo6 = {
  'spike-official': ['servo6-common', 'servo6-official'],
  'spike-pybricks': ['servo6-common', 'servo6-pybricks'],
};

let n = 0;
for (const [p, recipes] of Object.entries(R)) {
  fs.mkdirSync(path.join(OUT, p), { recursive: true });
  for (const [sw, list] of Object.entries(recipes)) {
    for (const lang of ['zh', 'en']) {
      const parts = list.map(item => {
        const [key, extra] = Array.isArray(item) ? item : [item, null];
        const frag = FR[lang][key];
        if (frag == null) throw new Error(`missing fragment ${key} (${lang})`);
        const vars = { P: p, ...V[p][lang], ...(extra ? extra[lang] : {}) };
        return frag.replace(/%%([A-Z0-9]+)%%/g, (m, k) => {
          if (!(k in vars)) throw new Error(`missing var ${k} in ${key} for ${p}`);
          return String(vars[k]);
        });
      });
      fs.writeFileSync(path.join(OUT, p, `${sw}.${lang}.md`), parts.join('\n\n') + '\n');
      n++;
    }
  }
}
console.log('wrote', n, 'files');
