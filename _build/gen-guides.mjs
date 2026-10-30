// 說明頁三層導覽產生器（產品 → 主機平台 → 軟體；設計見 _build/guides/DESIGN.md）
//   sensors/<p>/<host>/index.md        平台頁：買哪個版本、接線圖、② 選你用的軟體（<!-- sw-grid -->）
//   sensors/<p>/<host>/<sw>.md         軟體頁：安裝與設定（build 時 include）→ 讀值 → 命令 → 範例（build 時從範例頁抽）→ 常見問題 → 下載
//   英文到 en/sensors/…；_build/generated/guides-index.json（build.mjs 的切換列、格子、側欄、首頁選擇器、舊頁 moved 對照）
//   舊頁（spike-hub、ev3-official、arduino-i2c…）頁首補 <!-- moved: 新頁 --> 標記與一行提示（不刪內容）
//
// 用法：node _build/gen-guides.mjs              產生全部
//       node _build/gen-guides.mjs --only line8 只產某幾款（逗號分隔；guides-index.json 一律全產）
//       node _build/gen-guides.mjs --check      重產比對，不一致時結束碼 1
//
// 「安裝與設定」與「範例」不在這裡展開：頁面只放 <!-- include: … --> / <!-- examples: … --> 指令，由 build.mjs 在建站時展開，
// 所以 _build/guides/software/*.md、_build/guides/product/**、*-examples.md 改了不用重跑這支（--check 也不會因此不一致）。
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath, pathToFileURL } from 'node:url';
import { ROOT, BUILD, loadProducts, loadSiteProducts, makeWriter, siteKey, L, T, table, pageHref, valueName } from './gen-lib.mjs';
import { AI_NOTE, fence } from './protocol/pages/common.mjs';
import { FLAGS } from './site-flags.mjs';
import * as spike from './protocol/snippets/spike.mjs';
import * as ev3 from './protocol/snippets/ev3.mjs';
import * as i2c from './protocol/snippets/i2c.mjs';
import * as mblock from './protocol/snippets/mblock.mjs';
import * as matrix from './protocol/snippets/matrix.mjs';
import * as microbit from './protocol/snippets/microbit.mjs';

export const GUIDES = JSON.parse(fs.readFileSync(path.join(BUILD, 'guides.json'), 'utf8'));
export const HOST_BY_ID = Object.fromEntries(GUIDES.hosts.map((h) => [h.id, h]));
const SNIP = { spike, ev3, i2c, mblock, matrix, microbit };
const LANGS = ['zh', 'en'];
const exists = (rel) => fs.existsSync(path.join(ROOT, rel.replace(/^\//, '')));

// ---------------------------------------------------------------- 可用性：產品 × 主機 × 軟體
// products.json routes[host] 要有；有協定 JSON 的產品再看 hosts（matrix 看 ch22／big／mvision）；EXP6／Servo6 照 guides.json overrides
function hostOk(p, h) {
  const hs = p.hosts || {};
  if (h.protocolHost === 'matrix') return !!(hs.matrix || hs.mvision);
  return !!hs[h.protocolHost];
}
function reqOk(p, req) {
  const hs = p.hosts || {};
  if (req === 'matrix.ch22') return !!(hs.matrix && hs.matrix.ch22);
  if (req === 'matrix.big') return !!(hs.matrix && hs.matrix.big);
  if (req === 'mvision') return !!hs.mvision;
  return true;
}
export function availability(siteProducts, protos) {
  const byKey = {};
  for (const p of protos) (byKey[siteKey(p)] = byKey[siteKey(p)] || []).push(p);
  const out = {};
  for (const [key, sp] of Object.entries(siteProducts)) {
    const ov = (GUIDES.overrides || {})[key] || {};
    const ps = byKey[key] || [];
    const hosts = [];
    for (const h of GUIDES.hosts) {
      if (!(sp.routes && sp.routes[h.id])) continue;
      let sws;
      if (ov[h.id]) sws = ov[h.id];
      else {
        if (!ps.length || !ps.some((p) => hostOk(p, h))) continue;
        sws = h.software.filter((s) => !(s.hideWhen && FLAGS[s.hideWhen]) && (!s.requires || ps.some((p) => reqOk(p, s.requires)))).map((s) => s.id);
      }
      if (sws.length) hosts.push({ id: h.id, sws });
    }
    if (hosts.length) out[key] = { hosts, protos: ps.map((p) => p.id) };
  }
  return out;
}

// ---------------------------------------------------------------- 網址
const pre = (lang) => (lang === 'en' ? 'en/' : '');
export const hostMd = (lang, key, host) => `/${pre(lang)}sensors/${key}/${host}/index.md`;
export const swMd = (lang, key, host, sw) => `/${pre(lang)}sensors/${key}/${host}/${sw}.md`;
const productMd = (lang, key) => `/${pre(lang)}sensors/${key}/index.md`;
const swDef = (host, sw) => HOST_BY_ID[host].software.find((s) => s.id === sw);
const shortName = (sp, lang) => L(sp.name, lang).replace(/\s*[（(][^)）]*[)）]\s*$/, '').trim();

// 舊頁 → 新頁（md 路徑）；不是舊頁回 null。rel 例：sensors/line8/spike-hub.md、en/sensors/esp32cam/ev3-examples-wro.md
export function legacyTarget(rel, avail) {
  const m = rel.replace(/\\/g, '/').match(/^(en\/)?sensors\/([^/]+)\/([^/]+?)(-wro)?\.md$/);
  if (!m) return null;
  const [, en, key, base] = m;
  const lang = en ? 'en' : 'zh';
  const a = avail[key];
  if (!a) return null;
  if ((GUIDES.legacyProduct || []).includes(base)) return productMd(lang, key);
  for (const h of GUIDES.hosts) {
    if (!(base in (h.legacy || {}))) continue;
    const ah = a.hosts.find((x) => x.id === h.id);
    if (!ah) return productMd(lang, key);
    const sw = h.legacy[base];
    if (sw && ah.sws.includes(sw)) return swMd(lang, key, h.id, sw);
    if (!sw && ah.sws.length === 1) return swMd(lang, key, h.id, ah.sws[0]);
    return hostMd(lang, key, h.id);
  }
  return null;
}
// 舊頁標記：<!-- moved: … --> ＋一行提示（build.mjs 換成頁首提示條）
export function movedLines(target, label, lang) {
  return [`<!-- moved: ${target} -->`, `<p class="moved-note">${T(lang, '這頁內容已整合到新的說明頁：', 'This page has moved into the new guide: ')}<a href="${target}">${label}</a></p>`];
}
// 把標記放進內容：在開頭的 HTML 註解行之後；已有舊標記就換掉（gen-reference.mjs 也用這支，兩邊產出一致）
export function withMoved(content, target, label, lang) {
  const nl = content.includes('\r\n') ? '\r\n' : '\n';
  // 先拿掉舊標記（含提示後面那一行空行），再放新的；提示是 HTML 區塊，後面一定要空一行，否則下一行的 # 標題會被吃掉
  let lines = content.replace(/^<!-- moved: .* -->\r?\n(?:<p class="moved-note">.*<\/p>\r?\n(?:\r?\n)?)?/m, '').split(/\r?\n/);
  if (target) {
    let i = 0;
    while (i < lines.length && /^<!--.*-->\s*$/.test(lines[i])) i++;
    lines = [...lines.slice(0, i), ...movedLines(target, label, lang), '', ...lines.slice(i)];
  }
  return lines.join(nl);
}
export function targetLabel(target, siteProducts) {
  const m = target.match(/^\/(en\/)?sensors\/([^/]+)\/(?:([^/]+)\/)?([^/]+)\.md$/);
  if (!m) return target;
  const lang = m[1] ? 'en' : 'zh';
  const sp = siteProducts[m[2]];
  const parts = [shortName(sp, lang)];
  if (m[3]) {
    parts.push(HOST_BY_ID[m[3]].name[lang]);
    if (m[4] !== 'index') parts.push(L(swDef(m[3], m[4]).name, lang));
  }
  return parts.join(' › ');
}

// ---------------------------------------------------------------- 頁面
const genNote = (what) => `<!-- 本頁由 _build/gen-guides.mjs 產生（${what}），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->`;
const fill = (s, vars) => s.replace(/\{\{(\w+)\}\}/g, (a, k) => (vars[k] != null ? vars[k] : a));

function editionText(key, host, lang, vars) {
  const ov = ((GUIDES.overrides || {})[key] || {}).edition || {};
  return fill(L(ov[host] || HOST_BY_ID[host].edition, lang), vars);
}
function wiringFig(key, host, name, lang) {
  if (((GUIDES.overrides || {})[key] || {}).noWiringFigure) return '';
  const rel = `images/figures/${key}/wiring-${HOST_BY_ID[host].wiring}.svg`;
  return exists(rel) ? `<figure class="ref-fig"><img src="/${rel}" alt="${name} × ${HOST_BY_ID[host].fullName[lang]}" loading="lazy"></figure>` : '';
}
// 舊的手寫說明頁（安裝說明還沒搬到 _build/guides/ 之前的暫時連結）
function legacyPage(key, host, sw, lang) {
  for (const base of swDef(host, sw).legacyPages || []) {
    const md = `sensors/${key}/${base}.md`;
    if (lang === 'en' && exists('en/' + md)) return '/en/' + md;
    if (exists(md)) return '/' + md;
  }
  return null;
}
// 下載：照檔名找這個軟體會用到的檔
function listDir(rel, re) {
  const d = path.join(ROOT, rel);
  return fs.existsSync(d) ? fs.readdirSync(d).filter((f) => re.test(f)).sort().map((f) => `/${rel}/${f}`) : [];
}
function downloads(key, host, sw, lang, protos) {
  const items = [];
  const K = key.toUpperCase();
  const add = (href, label) => { if (href && !items.some((x) => x[0] === href)) items.push([href, label]); };
  const base = (h) => decodeURIComponent(h.split('/').pop());
  if (host === 'spike' && sw === 'pybricks') {
    for (const p of protos) { const lib = spike.LIB[p.id]; if (lib && exists(decodeURIComponent(lib.file))) add(lib.file, `${T(lang, 'Pybricks 函式庫', 'Pybricks library')} ${base(lib.file)}`); }
    // 共用函式庫 MBC_sensors.py（2026-10-25 起跟各產品函式庫放同一個資料夾）
    for (const p of protos) { const lib = spike.LIB[p.id]; const f = lib && lib.file.replace(/[^/]+$/, 'MBC_sensors.py'); if (f && exists(decodeURIComponent(f))) add(f, `${T(lang, '共用函式庫（新專案用，一個檔適用全部 MBC 感應器）', 'Shared library (for new projects, one file for every MBC sensor)')} MBC_sensors.py`); }
    // 3.6.1 已不再更新（2026-10-25）：說明頁只列 4.x，3.6.1 zip 只放在下載專區的「舊版」區
    // v4.0.0 檔名的 zip 只是舊連結相容用的複本（內容＝v4.x），不列出
    for (const f of listDir(`examples/${key}`, /pybricks_spike.*\.zip$/i)) if (!/v3\.6\.1|v4\.0\.0/.test(f)) add(f, base(f));
    for (const f of listDir('downloads', new RegExp(`^MBC_${K}_Pybricks_Lib.*\\.zip$`, 'i'))) add(f, base(f));
  }
  if (host === 'spike' && sw === 'official') {
    for (const f of listDir(`examples/${key}/spike`, /\.(llsp3|lms)$/i)) add(f, base(f));
    for (const f of listDir('downloads', new RegExp(`^MBC_${K}_Official_App_Lib.*\\.zip$`, 'i'))) add(f, base(f));
  }
  if (host === 'spike' && sw === 'mbcblocks') add(swDef('spike', 'mbcblocks').url, T(lang, 'MBC SPIKE 積木網站（線上，不用安裝）', 'MBC SPIKE Blocks website (online, nothing to install)'));
  if (host === 'ev3' && sw === 'pybricks') {
    for (const f of listDir(`examples/${key}`, /pybricks_ev3.*\.zip$/i)) add(f, base(f));
    for (const f of listDir(`examples/${key}/ev3`, /\.py$/i)) add(f, base(f));
  }
  if (host === 'arduino') for (const f of listDir(`examples/${key}/${lang === 'en' && exists(`examples/${key}/arduino-en`) ? 'arduino-en' : 'arduino'}`, /\.zip$/i)) add(f, `${T(lang, '全部範例', 'All examples')} ${base(f)}`);
  if (host === 'mblock') for (const f of listDir('downloads', /mBlock5.*\.mext$/i).reverse()) add(f, `${base(f)}${/v1\./.test(f) ? T(lang, '（舊版，2026-10 以前出貨的感應器）', ' (legacy, units shipped before 2026-10)') : ''}`);
  if (host === 'matrix' && sw === 'bigchannel') for (const f of listDir('downloads', /^MiniR4ColorSensorExt.*\.zip$/i)) add(f, base(f));
  if (host === 'microbit') add(microbit.EXT_URL, T(lang, 'MakeCode 擴充 pxt-mbc-sensors（在 MakeCode「擴充功能」貼上這個網址）', 'MakeCode extension pxt-mbc-sensors (paste this URL in MakeCode Extensions)'));
  add(pageHref(lang, '/downloads.md'), T(lang, '下載中心（全部產品的函式庫、韌體、型錄）', 'Download centre (libraries, firmware, catalogue for every product)'));
  return items.map(([h, l]) => `- [${l}](${h})`).join('\n');
}

// 讀值段開頭的說明（每個軟體一句）
function readIntro(host, sw, p, lang) {
  const cat = mblock.category(p, lang);
  const M = {
    'spike/official': T(lang, '官方 App 會把感應器顯示成「顏色感應器」。用「更多感應器」分類裡的顏色、反射光、原始紅／綠／藍積木讀，再照表換算。', 'The official app shows the sensor as a Color Sensor. Read it with the color, reflected light and raw red / green / blue blocks under More Sensors, then decode as shown.'),
    'spike/mbcblocks': T(lang, '主機要先換成 Pybricks 韌體。MBC 積木網站的感應器積木讀的是感應器的模式 9（一次讀完全部數值）。', 'The hub needs Pybricks firmware. The MBC Blocks website reads mode 9 of the sensor (all values in one read).'),
    'spike/pybricks': T(lang, '程式假設感應器插 A 孔。「MBC 函式庫」寫法要先把函式庫 .py 一起匯入 Pybricks。', 'Code assumes the sensor is on port A. For the MBC library form, import the library .py into Pybricks too.'),
    'ev3/ev3g': T(lang, '感應器在 EV3 會顯示成樂高原廠感應器，用原廠感應器積木讀就好。', 'The EV3 sees a stock LEGO sensor, so read it with the stock sensor block.'),
    'ev3/classroom': T(lang, '感應器在 EV3 會顯示成樂高原廠感應器，用 Classroom 的原廠感應器積木讀（Classroom 沒有繁中介面，積木名稱用英文）。', 'The EV3 sees a stock LEGO sensor; read it with the stock Classroom sensor blocks.'),
    'ev3/pybricks': T(lang, '程式假設感應器插 1 號孔，用 `LUMPDevice` 讀原始模式；EV3 MicroPython v2 寫法相同。', 'Code assumes port 1 and reads raw modes with `LUMPDevice`; EV3 MicroPython v2 works the same way.'),
    'ev3/clev3r': T(lang, '程式假設感應器插 1 號孔：`Sensor.SetMode` 選模式、`Sensor.ReadRawValue` 讀第幾個值。', 'Code assumes port 1: `Sensor.SetMode` picks the mode, `Sensor.ReadRawValue` reads the n-th value.'),
    'arduino/ide': T(lang, `下面的寫法都用到 \`mbcRead()\` 小幫手：展開「小幫手程式」，第 ① 段（2 行）放在程式最上面、第 ② 段貼在程式最下面，位址 \`${p.addr}\`。`, `Every snippet uses the \`mbcRead()\` helper: expand "Helper code", put part ① (2 lines) at the top of your sketch and paste part ② at the bottom; address \`${p.addr}\`.`),
    'mblock/mblock5': T(lang, `裝好 MBC Pro Sensors 擴展後，在「${cat}」分類找積木，連接埠選感應器實際插的孔。`, `After adding the MBC Pro Sensors extension, find the blocks in the "${cat}" category and pick the port the sensor is plugged into.`),
    'matrix/matrixblock': T(lang, '用 MATRIXblock 原廠「顏色感應器」積木的 R／G／B／C／M／Y／K 讀（不要用 H／S／V 與顏色辨識積木）；Arduino 模式寫法一併列出。', 'Use the stock MATRIXblock colour-sensor R / G / B / C / M / Y / K blocks (not H / S / V or colour ID); the Arduino-mode line is shown too.'),
    'matrix/bigchannel': T(lang, '換上大通道函式庫後，用下面的 `bigU8()`／`bigU16()` 小幫手讀第幾個 byte（展開「小幫手程式」）。', 'With the big-channel library installed, read byte n with the `bigU8()` / `bigU16()` helpers (expand "Helper code").'),
    'matrix/mvision': T(lang, '神攝手在 MATRIX 走串口，用 MATRIXblock 內建的 mVision 攝影機積木讀，不用 I2C。', 'On MATRIX the camera talks serial; read it with the built-in mVision camera blocks, not I2C.'),
    'microbit/makecode': T(lang, `MakeCode 加入 pxt-mbc-sensors 擴充後，在「MBC 感應器」分類找積木；JavaScript 寫法一併列出，位址 \`${p.addr}\`。`, `After adding pxt-mbc-sensors, find the blocks in the MBC Sensors category; the JavaScript form is shown too, address \`${p.addr}\`.`),
  };
  return M[`${host}/${sw}`] || '';
}
const SNIP_OF = { spike: 'spike', ev3: 'ev3', arduino: 'i2c', mblock: 'mblock', matrix: 'matrix', microbit: 'microbit' };
function readOf(host, sw, p, v, lang) { return SNIP[SNIP_OF[host]].bySoftware(sw, p, v, lang); }
function cmdOf(host, sw, p, c, lang) { return SNIP[SNIP_OF[host]].commandBySoftware(sw, p, c, lang); }

function renderResult(r) {
  const parts = [];
  if (r.text) parts.push(r.text);
  if (r.html) parts.push(r.html);
  if (r.code) parts.push(fence(r.code, r.codeLang || 'text'));
  for (const x of r.extra || []) { if (x.title) parts.push(`**${x.title}**`); parts.push(fence(x.code, x.codeLang || 'text')); }
  if (r.note) parts.push(`<p class="ref-note">${r.note.replace(/`([^`]+)`/g, '<code>$1</code>')}</p>`);
  return parts.join('\n\n');
}

// 一款協定產品（神攝手 WRO 版另一節）的讀值＋命令
function readingsBlock(key, host, sw, p, lang, sp, avail, heading, idPre) {
  const out = [];
  const hName = HOST_BY_ID[host].name[lang];
  out.push(`## ${heading}`, '');
  const intro = readIntro(host, sw, p, lang);
  if (intro) out.push(intro, '');
  if (host === 'arduino') out.push(`<details class="helper">\n<summary>${T(lang, '小幫手程式（① 放最上面、② 貼最下面）', 'Helper code (① at the top, ② at the bottom)')}</summary>\n\n${i2c.helperParts(p, lang)}\n\n</details>`, '');
  if (host === 'matrix' && sw === 'bigchannel') out.push(`<details class="helper">\n<summary>${T(lang, '小幫手程式', 'Helper code')}</summary>\n\n${fence(matrix.bigHelpers(lang), 'cpp')}\n\n</details>`, '');
  const missing = [];
  for (const v of p.values) {
    const r = readOf(host, sw, p, v, lang);
    if (!r) { missing.push(v); continue; }
    out.push(`<section id="${idPre}${v.id.toLowerCase()}" class="ref-item">`, '');
    out.push(`### ${valueName(v, lang)} <small>${v.key}</small>`, '');
    const rel = `images/figures/${key}/value-${v.key}.svg`;
    if (v.figure && v.figure.tpl && exists(rel)) out.push(`<figure class="ref-fig"><img src="/${rel}" alt="${valueName(v, lang)}" loading="lazy"></figure>`, '');
    // EV3 有些模式送的是換算值（protocol scale），範圍欄要講清楚，不然會照值本身的範圍去用
    const sc = host === 'ev3' ? (v.ev3 || []).filter((e) => e.scale) : [];
    const scNote = sc.length ? T(lang, `（EV3 ${sc.map((e) => `模式 ${e.mode}`).join('、')} 讀到的是換算值：${sc[0].scale}）`, ` (EV3 ${sc.map((e) => `mode ${e.mode}`).join(', ')} ${sc.length > 1 ? 'give' : 'gives'} a scaled value: ${ev3.scaleText(sc[0].scale, 'en')})`) : '';
    out.push(table([T(lang, '範圍', 'Range'), T(lang, '意思', 'Meaning')], [[`\`${v.range}\`${v.unit ? ` ${v.unit}` : ''}${scNote}`, L(v.desc, lang)]]), '');
    out.push(renderResult(r), '', '</section>', '');
  }
  // mBlock 擴展另外做的布林積木（循行者第 N 路在線上？、掌控者按鈕被按下？），不在值表裡
  if (host === 'mblock') for (const x of mblock.extraReporters(p, lang)) {
    out.push(`<section id="${idPre}x-${x.key.toLowerCase()}" class="ref-item">`, '', `### ${x.name} <small>${x.key}</small>`, '');
    out.push(table([T(lang, '範圍', 'Range'), T(lang, '意思', 'Meaning')], [[T(lang, '是／否', 'true/false'), x.desc]]), '');
    out.push(x.html, '', '</section>', '');
  }
  if (missing.length) {
    // 改用：同平台讀得到的其他軟體；都沒有就建議 Arduino I2C／mBlock／micro:bit（這款有的）
    const ah = avail[key].hosts.find((x) => x.id === host);
    const alt = ah.sws.filter((s) => s !== sw && missing.some((v) => readOf(host, s, p, v, lang)));
    const altLinks = alt.map((s) => `[${L(swDef(host, s).name, lang)}](${swMd(lang, key, host, s)})`);
    if (!altLinks.length) for (const h of ['arduino', 'mblock', 'microbit']) { const x = avail[key].hosts.find((y) => y.id === h); if (x && h !== host) altLinks.push(`[${HOST_BY_ID[h].name[lang]}](${x.sws.length === 1 ? swMd(lang, key, h, x.sws[0]) : hostMd(lang, key, h)})`); }
    out.push(`<p class="ref-missing"><b>${T(lang, '這個軟體讀不到', 'Not readable here')}</b>：${missing.map((v) => valueName(v, lang)).join(T(lang, '、', ', '))}${altLinks.length ? T(lang, `。要用這些值請改用 ${altLinks.join('／')}。`, `. Use ${altLinks.join(' / ')} for these.`) : T(lang, '。', '.')}</p>`.replace(/\[([^\]]+)\]\(([^)]+)\)/g, '<a href="$2">$1</a>'), '');
  }
  // 命令
  const cmds = p.commands || [];
  const extraCmds = host === 'mblock' ? mblock.extraCommands(p, lang) : [];   // mBlock 每類一顆「更新全部數值」
  if (cmds.length || extraCmds.length) {
    out.push(`## ${T(lang, '命令', 'Commands')}${idPre === 'w' ? T(lang, '（WRO 版）', ' (WRO edition)') : ''}`, '');
    const ok = cmds.map((c) => [c, cmdOf(host, sw, p, c, lang)]).filter(([, r]) => r);
    if (!ok.length && !extraCmds.length) {
      const spikeCmd = host === 'spike' && p.lego && p.lego.spikeCmd;   // 方案丙：SPIKE 有命令通道，但只有 Pybricks＋MBC 函式庫送得了
      const pyMd = spikeCmd && avail[key].hosts.find((x) => x.id === host).sws.includes('pybricks') ? swMd(lang, key, host, 'pybricks') : '';
      out.push(spikeCmd
        ? T(lang, `這個軟體送不了命令（官方 SPIKE App 3、Pybricks 內建 ColorSensor 積木都不行）。要下命令請用 ${pyMd ? `[Pybricks＋MBC 函式庫](${pyMd})` : 'Pybricks＋MBC 函式庫'}；校準也可以用感應器上的按鈕。`, `This software cannot send commands (neither the official SPIKE App 3 nor Pybricks' built-in ColorSensor blocks can). To send commands use ${pyMd ? `[Pybricks + the MBC library](${pyMd})` : 'Pybricks + the MBC library'}; you can also calibrate with the button on the sensor.`)
        : host === 'spike' || host === 'ev3'
        ? T(lang, `${hName} 接頭是唯讀的，這個軟體不能送命令：校準用感應器上的按鈕，切黑白線、門檻等設定改用 Arduino I2C、mBlock 或 micro:bit。`, `The ${hName} port is read-only, so this software cannot send commands: calibrate with the button on the sensor and change settings over Arduino I2C, mBlock or micro:bit.`)
        : T(lang, '這個軟體不能送命令；需要時改用 Arduino I2C、mBlock 或 micro:bit。', 'This software cannot send commands; use Arduino I2C, mBlock or micro:bit if you need them.'), '');
    } else {
      if (host === 'spike' && sw === 'pybricks' && ok.length) out.push(SNIP.spike.CMD_INTRO(lang), '');
      for (const [c, r] of ok) {
        out.push(`<section id="${idPre === 'w' ? 'wc' : 'c'}${c.id.toLowerCase()}" class="ref-item ref-cmd">`, '');
        out.push(`### ${L(c.name, lang)} <small>${c.key}</small>`, '', L(c.desc, lang), '');
        out.push(renderResult(r), '', '</section>', '');
      }
      for (const x of extraCmds) {
        out.push(`<section id="${idPre === 'w' ? 'wc' : 'c'}x-${x.key.toLowerCase()}" class="ref-item ref-cmd">`, '');
        out.push(`### ${x.name} <small>${x.key}</small>`, '', x.desc, '', x.html, '', '</section>', '');
      }
      const no = cmds.filter((c) => !ok.some(([x]) => x === c));
      if (no.length) out.push(`<p class="ref-missing"><b>${T(lang, '這個軟體送不了', 'Not available here')}</b>：${no.map((c) => L(c.name, lang)).join(T(lang, '、', ', '))}</p>`, '');
    }
  }
  return out;
}

// 範例：指令給 build.mjs 展開（英文頁沒有英文範例時用中文頁並加一行說明）
function examplesBlock(key, host, sw, lang, heading, base) {
  const def = swDef(host, sw);
  if (!def.examplesFrom) return [];
  const md = `sensors/${key}/${def.examplesFrom}${base || ''}.md`;
  let src = null;
  let note = '';
  if (lang === 'en' && exists('en/' + md)) src = 'en/' + md;
  else if (exists(md)) { src = md; if (lang === 'en') note = '> [!NOTE]\n> The English examples page is not ready yet; the examples below are in Chinese (code and pictures still apply).'; }
  if (!src) return [];
  const out = [`## ${heading}`, ''];
  if (note) out.push(note, '');
  out.push(`<!-- examples: ${src} host=${host} sw=${sw}${def.examplesWhole ? ' whole' : ''} -->`, '');
  return out;
}

// 「安裝與設定」內文的佔位：{{product}} {{edition}} {{port}} {{lib}} {{libUrl}} {{addr}}；skipWiring = 通用「接線」節不用（EXP6 的 Arduino 走 UART，專屬檔自己寫接線）
const EDITION_RE = { spike: /SPIKE/, ev3: /EV3/, arduino: /I2C/, microbit: /I2C/, mblock: /MakeBlock/i, matrix: /MATRIX/ };
const EDITION_DEF = { spike: ['SPIKE 版', 'SPIKE'], ev3: ['EV3 版', 'EV3'], arduino: ['通用 I2C 版', 'Universal I2C'], microbit: ['通用 I2C 版', 'Universal I2C'], mblock: ['MakeBlock 版', 'MakeBlock'], matrix: ['MATRIX 版', 'MATRIX'] };
function setupVars(key, host, sw, lang, sp, protos) {
  const p0 = protos[0];
  const ed = ((sp[lang] && sp[lang].editions) || []).find(([label]) => EDITION_RE[host].test(label) && !/App|Pybricks/.test(label));
  let edition = ed ? ed[0] : EDITION_DEF[host][lang === 'en' ? 1 : 0];
  if (lang === 'en' && !/edition/i.test(edition)) edition += ' edition';
  // 建議孔與函式庫：照範例頁（SPIKE「→ C 埠」出現最多的孔；EV3「1 號孔」）
  const ex = (base) => { const f = path.join(ROOT, `sensors/${key}/${base}.md`); return fs.existsSync(f) ? fs.readFileSync(f, 'utf8') : ''; };
  let port = HOST_BY_ID[host].port;
  if (host === 'spike') {
    const cnt = {};
    for (const m of ex('spike-examples').matchAll(/→ ([A-F]) 埠/g)) cnt[m[1]] = (cnt[m[1]] || 0) + 1;
    const best = Object.entries(cnt).sort((a, b) => b[1] - a[1])[0];
    if (best) port = best[0];
  }
  if (host === 'ev3') { const m = ex('ev3-examples').match(/([1-4]) 號孔/); if (m) port = m[1]; }
  let lib = '', libUrl = '';
  if (host === 'spike') {
    const m = ex('spike-examples').match(/\[函式庫\]\((\/[^)\s]+\.py)\)/);
    if (m) { libUrl = m[1]; lib = decodeURIComponent(m[1].split('/').pop()); }
    else if (p0 && spike.LIB[p0.id]) { libUrl = spike.LIB[p0.id].file; lib = decodeURIComponent(libUrl.split('/').pop()); }
  }
  const v = { product: L(sp.name, lang), edition, port, lib, libUrl, addr: p0 ? p0.addr : '' };
  if (key === 'exp6' && host === 'arduino') { v.skipWiring = true; v.edition = T(lang, '序列埠（UART）連接', 'serial (UART) connection'); }
  return v;
}

function swPage(key, host, sw, lang, sp, protos, avail) {
  const h = HOST_BY_ID[host];
  const def = swDef(host, sw);
  const name = L(sp.name, lang);
  const hName = h.name[lang];
  const swName = L(def.name, lang);
  const ah = avail[key].hosts.find((x) => x.id === host);
  const p0 = protos[0];
  const vars = setupVars(key, host, sw, lang, sp, protos);
  const out = [AI_NOTE, genNote(`${key} × ${host} × ${sw}`), `<!-- guide: ${key}/${host}/${sw} -->`];
  out.push(`# ${T(lang, `${name} × ${hName} × ${swName}`, `${name} × ${hName} × ${swName}`)}`, '');
  out.push(T(lang,
    `這一頁只講 **${L(def.longName, lang)}**：在 ${h.fullName[lang]} 上用它讀${name}的安裝步驟、每個值怎麼讀${protos.length ? '、能用的命令' : ''}與範例。要換產品、主機或軟體，用上方的切換列。`,
    `This page covers **${L(def.longName, lang)}** only: how to set it up for the ${name} on ${h.fullName[lang]}, how to read every value${protos.length ? ', the commands it supports' : ''} and examples. Use the switcher above to change product, host or software.`), '');
  // 這個平台只有一種軟體：沒經過平台頁，版本與接線放這裡
  if (ah.sws.length === 1) {
    out.push(`## ${T(lang, '要買哪個版本、怎麼接', 'Which edition and how to connect')}`, '', editionText(key, host, lang, vars), '');
    const fig = wiringFig(key, host, name, lang);
    if (fig) out.push(fig, '');
  }
  // 安裝與設定（build.mjs 展開；兩個檔都還沒有時顯示暫時說明）
  const inc = JSON.stringify(vars);
  const legacy = legacyPage(key, host, sw, lang);
  out.push(`## ${T(lang, '安裝與設定', 'Setup')}`, '');
  // 通用內文（### 你需要 → 安裝軟體 → 讓軟體認得感應器 → 接線 → 第一次測試 → 常見問題）＋產品專屬內文（插在「接線」之後、「第一次測試」之前）
  out.push(`<!-- setup: _build/guides/software/${host}-${sw}.${lang}.md + _build/guides/product/${key}/${host}-${sw}.${lang}.md ${inc} -->`);
  out.push('<!-- include-fallback -->');
  out.push(`> [!NOTE]\n> ${T(lang, '這個軟體的安裝步驟正在搬到這一頁。', 'Setup steps for this software are being moved onto this page.')}${legacy ? T(lang, `先看原本的說明：[${name}：${L(def.longName, lang)} 說明](${legacy})。`, ` Meanwhile see the previous guide: [${name}: ${L(def.longName, lang)}](${legacy}).`) : ''}`);
  out.push('<!-- /include-fallback -->', '');
  // 讀值＋命令（有協定 JSON 的產品）
  const cam = protos.length > 1;
  for (const p of protos) {
    const wro = /_wro$/.test(p.id);
    const ed = cam ? L(GUIDES.editions[wro ? 'camWro' : 'camSchool'], lang) : '';
    out.push(...readingsBlock(key, host, sw, p, lang, sp, avail, T(lang, `讀值${ed ? `（${ed}）` : ''}`, `Reading values${ed ? ` (${ed})` : ''}`), wro ? 'w' : 'v'));
  }
  // 範例
  const ex = cam
    ? [...examplesBlock(key, host, sw, lang, T(lang, `範例（${L(GUIDES.editions.camSchool, lang)}）`, `Examples (${L(GUIDES.editions.camSchool, lang)})`), ''),
      ...examplesBlock(key, host, sw, lang, T(lang, `範例（${L(GUIDES.editions.camWro, lang)}）`, `Examples (${L(GUIDES.editions.camWro, lang)})`), '-wro')]
    : examplesBlock(key, host, sw, lang, T(lang, '範例', 'Examples'), '');
  // 沒有範例頁的軟體（MATRIX、即將推出的產品）：保留「範例」節並說明，不讓讀者以為漏了
  if (!ex.length) {
    out.push(`## ${T(lang, '範例', 'Examples')}`, '');
    out.push(protos.length
      ? T(lang, `<p class="no-examples">這個軟體還沒有整支的範例程式。上面「讀值」${protos.some((p) => (p.commands || []).length) ? '和「命令」' : ''}每一項都附了可以直接照著做的寫法，照順序組起來就能用；需要整支範例請 <a href="/contact.html">聯絡我們</a>。</p>`,
        `<p class="no-examples">There is no full example program for this software yet. Each item under “Reading values”${protos.some((p) => (p.commands || []).length) ? ' and “Commands”' : ''} above comes with code you can use as is; put them together in order, or <a href="/en/contact.html">contact us</a> for a full example.</p>`)
      : T(lang, '<p class="no-examples">這個軟體的範例程式還在準備中，會跟產品一起上線；需要的話請先 <a href="/contact.html">聯絡我們</a>。</p>',
        '<p class="no-examples">Example programs for this software are being prepared and will go live with the product; <a href="/en/contact.html">contact us</a> if you need one sooner.</p>'), '');
  } else out.push(...ex);
  // 常見問題
  out.push(`## ${T(lang, '其他問題', 'More questions')}`, '');
  // overrides.<產品>.faqAnswer：{ "問題中文": {zh, en} } 換掉這個產品的答案（例如 Servo6 在官方 App 顯示成超音波感應器）
  const faqAns = ((GUIDES.overrides || {})[key] || {}).faqAnswer || {};
  out.push((h.faq || []).filter((f) => !(f.hideWhen && FLAGS[f.hideWhen])).map((f) => `- **${L(f.q, lang)}** ${L(faqAns[f.q.zh] || f.a, lang)}`).join('\n'), '');
  out.push(T(lang, `還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。`, `Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.`), '');
  // 下載
  out.push(`## ${T(lang, '下載', 'Downloads')}`, '', downloads(key, host, sw, lang, protos), '');
  // 本頁依據
  out.push(`## ${T(lang, '本頁依據', 'Sources')}`, '');
  out.push(`- ${T(lang, '主機與軟體清單', 'Host and software list')}：\`_build/guides.json\``);
  if (protos.length) out.push(`- ${T(lang, '值與命令', 'Values and commands')}：${protos.map((p) => `\`_build/protocol/${p.id}.json\``).join('、')}${T(lang, '（單一真相來源）', ' (single source of truth)')}`);
  out.push(`- ${T(lang, '安裝與設定', 'Setup')}：\`_build/guides/software/${host}-${sw}.${lang}.md\`、\`_build/guides/product/${key}/${host}-${sw}.${lang}.md\``);
  if (def.examplesFrom) out.push(`- ${T(lang, '範例', 'Examples')}：\`sensors/${key}/${def.examplesFrom}${cam ? '(-wro)' : ''}.md\``);
  return out.join('\n') + '\n';
}

function hostPage(key, host, lang, sp, protos, avail) {
  const h = HOST_BY_ID[host];
  const name = L(sp.name, lang);
  const ah = avail[key].hosts.find((x) => x.id === host);
  const p0 = protos[0];
  const out = [AI_NOTE, genNote(`${key} × ${host}`), `<!-- guide: ${key}/${host} -->`];
  // 只有一種軟體的平台：產品頁直接連軟體頁，平台頁只給切換列用，不收錄
  if (ah.sws.length === 1) out.push('<!-- noindex -->');
  out.push(`# ${name} × ${h.fullName[lang]}`, '');
  out.push(T(lang, `${name}用在 ${h.fullName[lang]}：先確認要買哪個版本、怎麼接，再選你用的軟體，之後整頁都只講那個軟體。`, `Using the ${name} with ${h.fullName[lang]}: check the edition and the wiring, then pick your software; its page covers that software only.`), '');
  // 平台頁頂圖：images/illust/host-<主機>.webp（沒有就 svg）
  const top = [h.illust && h.illust.replace(/\.svg$/, '.webp'), h.illust].find((x) => x && exists(x));
  if (top) out.push(`<figure class="guide-hero"><img src="${top}" alt="${h.fullName[lang]}"></figure>`, '');
  out.push(`## ${T(lang, '要買哪個版本', 'Which edition')}`, '', editionText(key, host, lang, { addr: p0 ? p0.addr : '', product: name }), '');
  const fig = wiringFig(key, host, name, lang);
  if (fig) out.push(`## ${T(lang, '怎麼接', 'Wiring')}`, '', fig, '');
  out.push(`## ${T(lang, '② 選你用的軟體', '② Pick your software')}`, '', '<!-- sw-grid -->', '');
  out.push(`## ${T(lang, '其他資料', 'More')}`, '');
  out.push(`- [${T(lang, `${name}產品介紹、規格與校準`, `${name}: overview, specs and calibration`)}](${productMd(lang, key)})`);
  out.push(`- [${T(lang, '下載中心', 'Download centre')}](${pageHref(lang, '/downloads.md')})`);
  return out.join('\n') + '\n';
}

// ---------------------------------------------------------------- 主程式
function main() {
  const argv = process.argv.slice(2);
  const CHECK = argv.includes('--check');
  const onlyIdx = argv.indexOf('--only');
  const ONLY = onlyIdx > -1 ? argv[onlyIdx + 1].split(',') : null;
  const siteProducts = loadSiteProducts();
  const protos = loadProducts(null);
  const avail = availability(siteProducts, protos);
  const w = makeWriter({ check: CHECK, label: 'gen-guides' });
  const protoById = Object.fromEntries(protos.map((p) => [p.id, p]));
  let nHost = 0, nSw = 0;

  for (const [key, a] of Object.entries(avail)) {
    if (ONLY && !ONLY.includes(key)) continue;
    const sp = siteProducts[key];
    const ps = a.protos.map((id) => protoById[id]);
    for (const lang of LANGS) {
      for (const ah of a.hosts) {
        w.write(hostMd(lang, key, ah.id).slice(1), hostPage(key, ah.id, lang, sp, ps, avail));
        nHost++;
        for (const sw of ah.sws) { w.write(swMd(lang, key, ah.id, sw).slice(1), swPage(key, ah.id, sw, lang, sp, ps, avail)); nSw++; }
      }
    }
  }

  // 舊頁：補 moved 標記（gen-reference 產的頁由 gen-reference 自己加，內容一樣）
  const moved = {};
  for (const lang of LANGS) {
    const dir = path.join(ROOT, pre(lang), 'sensors');
    for (const key of fs.readdirSync(dir)) {
      const d = path.join(dir, key);
      if (!fs.statSync(d).isDirectory()) continue;
      for (const f of fs.readdirSync(d)) {
        if (!f.endsWith('.md')) continue;
        const rel = `${pre(lang)}sensors/${key}/${f}`;
        const target = legacyTarget(rel, avail);
        if (!target) continue;
        moved[rel] = target;
        if (ONLY && !ONLY.includes(key)) continue;
        const cur = fs.readFileSync(path.join(ROOT, rel), 'utf8');
        const next = withMoved(cur, target, targetLabel(target, siteProducts), lang);
        if (next !== cur) w.write(rel, next);
      }
    }
  }

  // build.mjs 用的索引
  const index = {
    _說明: '由 _build/gen-guides.mjs 產生，請勿手改',
    hosts: GUIDES.hosts.map((h) => ({ id: h.id, name: h.name, fullName: h.fullName, tagline: h.tagline, icon: h.icon, illust: h.illust, colorToken: h.colorToken,
      software: h.software.filter((s) => !(s.hideWhen && FLAGS[s.hideWhen])).map((s) => ({ id: s.id, name: s.name, longName: s.longName, tagline: s.tagline, icon: s.icon, summaryMatch: s.summaryMatch || null, colMatch: s.colMatch || null })) })),
    products: Object.fromEntries(Object.entries(avail).map(([key, a]) => [key, { name: siteProducts[key].name, short: { zh: shortName(siteProducts[key], 'zh'), en: shortName(siteProducts[key], 'en') }, image: siteProducts[key].image || null, color: siteProducts[key].color, hosts: a.hosts }])),
    moved,
  };
  w.write('_build/generated/guides-index.json', JSON.stringify(index, null, 1) + '\n');
  if (!CHECK) console.log(`平台頁 ${nHost} 頁、軟體頁 ${nSw} 頁（中英合計）`);
  w.done();
}

if (process.argv[1] && import.meta.url === pathToFileURL(path.resolve(process.argv[1])).href) main();
