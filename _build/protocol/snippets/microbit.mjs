// micro:bit 片段：MakeCode 擴充 pxt-mbc-sensors 的真積木＋ JavaScript
//   用哪顆積木：protocol/*.json 的 microbit.js（JavaScript 呼叫，例 "mbc.linePosition()"；N = 第幾個）
//   積木文字：_build/microbit-blocks.json（node _build/sync-microbit.mjs 從擴充原始碼與 _locales/zh-TW 抽），跟 MakeCode 裡看到的一樣
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { L, T, hex, valueName } from '../../gen-lib.mjs';

export function available(product) { return !!(product.hosts && product.hosts.microbit); }

const TS = 'typescript';
export const EXT_URL = 'https://github.com/contactmbcrobot/pxt-mbc-sensors';
const EXT = JSON.parse(fs.readFileSync(path.join(path.dirname(fileURLToPath(import.meta.url)), '../../microbit-blocks.json'), 'utf8'));
export const EXT_VERSION = EXT.version;
const COLOR = '#00A4E4';   // 擴充 namespace 的顏色（mbc.ts //% color）

// "mbc.tof2Distance(MbcTofSide.Left, MbcDistUnit.Cm)" → { fn, b, args }；積木不存在就直接報錯（擴充改名時要改 JSON）
function parse(call) {
  const m = String(call).match(/^mbc\.(\w+)\((.*)\)$/);
  if (!m) throw new Error(`microbit.js 不是 mbc.<積木>(…) 呼叫：${call}`);
  const b = EXT.blocks[m[1]];
  if (!b) throw new Error(`擴充 v${EXT.version} 沒有積木 mbc.${m[1]}（擴充改名了？重跑 node _build/sync-microbit.mjs 再改 protocol JSON）`);
  const args = m[2].trim() ? m[2].split(',').map((s) => s.trim()) : [];
  const want = b.params.filter((p) => p !== 'handler');
  if (args.length !== want.length) throw new Error(`mbc.${m[1]} 要 ${want.length} 個參數（${want.join(', ')}），JSON 寫了 ${args.length} 個：${call}`);
  return { fn: m[1], b, args, params: want };
}
const enumLabel = (a, lang) => {
  const e = EXT.enums[a];
  if (!e) throw new Error(`擴充 v${EXT.version} 沒有選項 ${a}`);
  return lang === 'en' ? e.en : e.zh;
};
const isEnum = (a) => /^Mbc\w+\.\w+$/.test(a);
// scratchblocks 的標籤文字：括號、方括號、角括號要跳脫，不然會被當成輸入格
const esc = (s) => s.replace(/([()[\]<>])/g, '\\$1');

// 積木文字（表格用）：選單顯示選項文字，N 顯示 ［1~count］
function labelText(call, lang, count) {
  const { b, args, params } = parse(call);
  return (lang === 'en' ? b.en : b.zh).replace(/[%$](\w+)/g, (_, p) => {
    const a = args[params.indexOf(p)];
    if (a === undefined) return _;
    if (a === 'N') return T(lang, `［1~${count}］`, `[1~${count}]`);
    if (isEnum(a)) return T(lang, `［${enumLabel(a, lang)}］`, `[${enumLabel(a, lang)}]`);
    if (/^0x[0-9a-f]{6}$/i.test(a)) return `#${a.slice(2).toLowerCase()}`;
    return a;
  });
}
// scratchblocks 原始碼：( ) 讀值、< > 是／否、事件用 hat
function scratch(call, lang) {
  const { b, args, params } = parse(call);
  const src = lang === 'en' ? b.en : b.zh;
  let out = '';
  let last = 0;
  src.replace(/[%$](\w+)/g, (m, p, off) => {
    out += esc(src.slice(last, off));
    last = off + m.length;
    const a = args[params.indexOf(p)];
    if (a === undefined) out += esc(m);
    else if (a === 'N') out += '(1)';
    else if (isEnum(a)) out += `[${esc(enumLabel(a, lang))} v]`;
    else if (/^0x[0-9a-f]{6}$/i.test(a)) out += `[#${a.slice(2).toLowerCase()}]`;
    else if (/^0x[0-9a-f]+$/i.test(a)) out += `(${a} v)`;
    else out += `(${a})`;
    return m;
  });
  out += esc(src.slice(last));
  if (b.kind === 'boolean') return `<${out} :: ${COLOR}>`;
  if (b.kind === 'reporter') return `(${out} :: ${COLOR})`;
  if (b.kind === 'event') return `${out} :: ${COLOR} hat`;
  return `${out} :: ${COLOR}`;
}
const pre = (s) => `<pre class="blocks">${s}</pre>`;
const jsOf = (call, n) => call.replace(/\bN\b/, String(n));
const calls = (mb) => (mb && mb.js ? (Array.isArray(mb.js) ? mb.js : [mb.js]) : []);
// 事件積木的 JavaScript：補一個空的 function () {}
const stmt = (call) => (parse(call).b.kind === 'event' ? `${call.replace(/\)$/, '')}, function () {\n})` : call);

export function blockText(product, v, lang) {
  const c = calls(v.microbit);
  if (!c.length) return null;
  if (c.length === 1) return labelText(c[0], lang, v.count);
  // 每個一顆呼叫（搖桿 4 軸、陀螺儀 X／Y／Z）：同一顆積木，選單不同 → 把那個選單的選項串起來
  const ps = c.map(parse);
  const i = ps[0].args.findIndex((a, k) => ps.some((p) => p.args[k] !== a));
  if (i < 0) return labelText(c[0], lang, v.count);
  const opts = ps.map((p) => enumLabel(p.args[i], lang)).join(T(lang, '／', ' / '));
  const one = labelText(c[0], lang, v.count);
  const first = T(lang, `［${enumLabel(ps[0].args[i], lang)}］`, `[${enumLabel(ps[0].args[i], lang)}]`);
  return one.replace(first, T(lang, `［${opts}］`, `[${opts}]`));
}
export function commandText(product, c, lang) {
  const k = calls(c.microbit);
  return k.length ? labelText(k[0], lang) : null;
}
// 值表以外、擴充另外有的積木（是／否、事件），掛在相關的值底下：microbit.also
export function alsoText(product, item, lang) {
  return ((item.microbit && item.microbit.also) || []).map((a) => labelText(a, lang, item.count));
}

function also(item, lang) {
  return ((item.microbit && item.microbit.also) || []).map((a) => {
    const { fn, b } = parse(a);
    return {
      title: T(lang, `相關積木「${labelText(a, lang, item.count)}」`, `Related block "${labelText(a, lang, item.count)}"`),
      code: b.kind === 'boolean' || b.kind === 'reporter' ? `let ${fn} = ${jsOf(a, 1)}` : stmt(jsOf(a, 1)),
      codeLang: TS,
    };
  });
}
const missNote = (kind, lang) => (kind === 'boolean'
  ? T(lang, '感應器沒接好（讀不到）時回「否」。', 'Returns false when the sensor does not answer.')
  : T(lang, '感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。', 'Returns 0 when the sensor does not answer; check with "device at … is ready" first.'));

export function value(product, v, lang) {
  const c = calls(v.microbit);
  const comment = `${valueName(v, lang)}：${v.range}${v.unit ? ' ' + v.unit : ''}`;
  if (!c.length) {
    // 沒有專屬積木：JavaScript 用通用讀值（只能在 JavaScript 模式用）
    const see = v.microbit && v.microbit.see;
    const code = v.count
      ? `let ${v.key} = mbc.readValues(${product.addr}, ${hex(v.id)}, ${v.count})   // ${comment}\nlet ${v.key}1 = ${v.key}.getNumber(NumberFormat.Int16LE, 0)`
      : `let ${v.key} = mbc.readValue(${product.addr}, ${hex(v.id)})   // ${comment}`;
    return {
      text: T(lang, `擴充沒有這顆積木；在 JavaScript 用 \`mbc.readValue(位址, 編號)\` 讀（讀不到 = −32768）。${see ? `相關積木：「${labelText(see, lang)}」。` : ''}`,
        `No dedicated block; in JavaScript read it with \`mbc.readValue(address, id)\` (−32768 when nothing answers).${see ? ` Related block: "${labelText(see, lang)}".` : ''}`),
      code, codeLang: TS,
    };
  }
  const kind = parse(c[0]).b.kind;
  const html = pre(scratch(c[0], lang));
  let code;
  if (c.length > 1) code = `let ${v.key} = [${c.join(', ')}]   // ${comment}`;
  else if (v.count) code = `let ${v.key} = ${jsOf(c[0], 1)}   // ${comment}${T(lang, `（N = 1~${v.count}）`, ` (N = 1~${v.count})`)}`;
  else code = `let ${v.key} = ${c[0]}   // ${comment}`;
  return { html, code, codeLang: TS, extra: also(v, lang), note: missNote(kind, lang) };
}

export function command(product, c, lang) {
  const k = calls(c.microbit);
  if (!k.length) return { text: T(lang, `擴充沒有這顆積木；在 JavaScript 用 \`mbc.writeCmd(${product.addr}, ${hex(c.id)}, [參數])\`。`, `No dedicated block; in JavaScript use \`mbc.writeCmd(${product.addr}, ${hex(c.id)}, [params])\`.`), codeLang: TS };
  return { html: pre(scratch(k[0], lang)), code: `${k[0]}   // ${L(c.name, lang)}`, codeLang: TS, extra: also(c, lang), note: '' };
}

export function status(product, lang) {
  const ready = `mbc.deviceReady(${product.addr})`;
  return {
    html: pre(scratch(ready, lang)),
    code: `let ready = mbc.deviceReady(${product.addr})   // ${T(lang, 'F3 bit0；沒接 = false', 'F3 bit0; false when nothing answers')}\nlet st = mbc.status(${product.addr})   // F3`,
    codeLang: TS, note: '',
  };
}

// 供電（跟 microbit.md 總覽的供電表一致）：耗電大的一定外接 5 V，其他可以先用 micro:bit 的 3V 腳
export const EXT_5V = ['tof8', 'ps4', 'cam_school', 'cam_wro'];
export function powerText(product, lang) {
  return EXT_5V.includes(product.id)
    ? T(lang, '**一定要外接 5 V**（GND 要和 micro:bit 共地）', '**must use external 5 V** (share GND with the micro:bit)')
    : T(lang, 'micro:bit 3V（電池供電時建議外接 5 V）', 'micro:bit 3V (external 5 V recommended on batteries)');
}

// 第一支 JavaScript：等感應器就緒，再一直顯示第一個有專屬讀值積木的值
export function firstExample(product, lang) {
  const v = product.values.find((x) => calls(x.microbit).length && parse(calls(x.microbit)[0]).b.kind === 'reporter');
  const c = v ? jsOf(calls(v.microbit)[0], 1) : `mbc.readValue(${product.addr}, 0xF5)`;
  const wait = EXT_5V.includes(product.id) && product.id === 'tof8' ? 5000 : 3000;
  return `if (!(mbc.waitReady(${product.addr}, ${wait}))) {\n    basic.showIcon(IconNames.No)   // ${T(lang, '讀不到：檢查接線與電源', 'nothing answers: check wiring and power')}\n}\nbasic.forever(function () {\n    basic.showNumber(${c})   // ${v ? valueName(v, lang) : 'uptime'}\n    basic.pause(100)\n})`;
}

// ---------------------------------------------------------------- 依軟體（gen-guides.mjs 軟體頁用；這個平台只有 makecode）
export function bySoftware(sw, product, v, lang) { return sw === 'makecode' ? value(product, v, lang) : null; }
export function commandBySoftware(sw, product, c, lang) { return sw === 'makecode' ? command(product, c, lang) : null; }
